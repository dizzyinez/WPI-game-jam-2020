#pragma once
#include "net/common.hpp"
#include "net/message.hpp"

#include <enet/enet.h>
namespace net
{
template <typename T>
class client_interface
{
public:
        client_interface()
        {
                client = enet_host_create(NULL, 1, 2, 0, 0);
                if (client == NULL)
                {
                        std::cout << "An error occurred while trying to make a client" << std::endl;
                }
                else
                {
                        std::cout << "Client created successfully" << std::endl;
                }
        }
        ~client_interface() {}
        bool Connect()
        {
                enet_address_set_host_ip(&server_address, "73.14.41.127");
                server_address.port = 76743;
                server = enet_host_connect(client, &server_address, 2, 0);
                if (server == NULL)
                {
                        std::cout << "No available peer" << std::endl;
                }
                ENetEvent event;
                if (enet_host_service(client, &event, 1000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
                {
                        std::cout << "connection successful" << std::endl;
                        running = true;
                        return true;
                }
                std::cout << "connection unsuccessful" << std::endl;
                return false;
        }
        void Disconnect()
        {
                running = false;
        }
        bool IsConnected() {return running;}

        void Update()
        {
                ENetEvent event;
                while (enet_host_service(client, &event, 0) > 0)
                {
                        switch (event.type)
                        {
                        case ENET_EVENT_TYPE_CONNECT:
                                std::cout << "Client connected" << std::endl;
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
                                OnMessage(msg);
                                enet_packet_destroy(event.packet);
                        }
                        break;

                        case ENET_EVENT_TYPE_DISCONNECT:
                                event.peer->data = NULL;
                                break;
                        }
                }
        }

        void MessageServer(const message<T>& msg)
        {
                ENetPacket *packet = enet_packet_create(&msg.header, sizeof(message_header<T>), ENET_PACKET_FLAG_RELIABLE);
                if (msg.header.body_size > 0)
                {
                        enet_packet_resize(packet, msg.size());
                        memcpy(&packet->data[sizeof(message_header<T>)], msg.body.data(), msg.header.body_size);
                }
                enet_peer_send(server, 0, packet);
        }
        void flush()
        {
                enet_host_flush(client);
        }
private:
        virtual void OnMessage(message<T>& msg) = 0;
        ENetHost* client;
        ENetAddress server_address;
        ENetPeer* server;
        bool running = false;
};
}