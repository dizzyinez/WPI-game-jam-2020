#pragma once
#include "net/common.hpp"
#include "net/message.hpp"

namespace net
{
template <typename T, typename D>
class server_interface
{
public:
        server_interface()
        {
        }
        ~server_interface()
        {
        }

        void Update()
        {
                ENetEvent event;
                while (enet_host_service(server, &event, 0) > 0)
                {
                        switch (event.type)
                        {
                        case ENET_EVENT_TYPE_CONNECT:
                        {
                                std::cout << "Client connected" << std::endl;
                                // event.peer->data = malloc(constexpr (sizeof(D)));
                                event.peer->data = new D();
                                // D *client_data = new D;
                                // event.peer->data = reinterpret_cast<void*>(client_data);
                        }
                        break;

                        case ENET_EVENT_TYPE_RECEIVE:
                        {
                                message<T> msg;
                                memcpy(&msg.header, event.packet->data, sizeof(message_header<T>));
                                if (msg.header.body_size > 0)
                                {
                                        msg.body.resize(msg.header.body_size);
                                        memcpy(msg.body.data(), event.packet->data + sizeof(message_header<T>), msg.body.size());
                                }
                                OnMessage(msg, event.peer);
                                enet_packet_destroy(event.packet);
                        }
                        break;

                        case ENET_EVENT_TYPE_DISCONNECT:
                        {
                                delete (D*)event.peer->data;
                        }
                        break;
                        }
                }
        }

        bool Start()
        {
                if (!running)
                {
                        address.host = ENET_HOST_ANY;
                        address.port = 76743;
                        server = enet_host_create(&address, 32, 2, 0, 0);
                        if (server == NULL)
                        {
                                std::cout << "An error occurred while trying to make a server" << std::endl;
                        }
                        else
                        {
                                std::cout << "Server created" << std::endl;
                        }
                        running = true;
                        return true;
                }
                else
                {
                        std::cout << "server already running" << std::endl;
                }
                return false;
        }

        void Stop()
        {
                if (running)
                {
                }
                else
                {
                        std::cout << "server not running!" << std::endl;
                }
        }

        void MessageClient(const message<T>& msg, ENetPeer* peer)
        {
                ENetPacket *packet = enet_packet_create(&msg.header, sizeof(message_header<T>), ENET_PACKET_FLAG_RELIABLE);
                if (msg.header.body_size > 0)
                {
                        enet_packet_resize(packet, msg.size());
                        memcpy(&packet->data[sizeof(message_header<T>)], msg.body.data(), msg.header.body_size);
                }
                enet_peer_send(peer, 0, packet);
        }

        void MessageAllClients(const message<T>& msg)
        {
                ENetPacket *packet = enet_packet_create(&msg.header, sizeof(message_header<T>), ENET_PACKET_FLAG_RELIABLE);
                if (msg.header.body_size > 0)
                {
                        enet_packet_resize(packet, msg.size());
                        memcpy(&packet->data[sizeof(message_header<T>)], msg.body.data(), msg.header.body_size);
                }
                enet_host_broadcast(server, 0, packet);
        }

        void MessageAllClients(const message<T>& msg, ENetPeer* ignore)
        {
                ENetPacket *packet = enet_packet_create(&msg.header, sizeof(message_header<T>), ENET_PACKET_FLAG_RELIABLE);
                if (msg.header.body_size > 0)
                {
                        enet_packet_resize(packet, msg.size());
                        memcpy(&packet->data[sizeof(message_header<T>)], msg.body.data(), msg.header.body_size);
                }
                for (int i = 0; i < server->connectedPeers; i++)
                {
                        enet_peer_send(&server->peers[i], 0, packet);
                }
        }

        void flush()
        {
                enet_host_flush(server);
        }

        D* GetData(ENetPeer* peer)
        {
                // return reinterpret_cast<D*>(peer->data);
                return (D *)peer->data;
        }
        bool running = false;
private:
        virtual bool OnClientConnect(ENetPeer* peer)
        {
                return false;
        }
        virtual void OnClientDisconnect(ENetPeer* peer) = 0;
        virtual void OnMessage(message<T>& msg, ENetPeer* peer) = 0;
        ENetAddress address;
        ENetHost* server = nullptr;
};
}
