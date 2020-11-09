#pragma once
#include "net/message.hpp"
#include "net/server.hpp"
#include "net/client.hpp"

enum class message_types : uint32_t
{
        CONNECTION_REQUEST,
        CONNECTION_APPROVED,
        CONNECTION_DENIED,
        NEW_PLAYER,
        PLAYER_ACK,
        START_GAME,
        DISCONNECTION,
};

using short_string = std::array<char, 20>;

struct ClientData
{
        int index;
};

struct Player
{
        short_string name;
        int id = 0;
        bool exists = false;
        bool local = false;
};

using Message = net::message<message_types>;

class Server : public net::server_interface<message_types, ClientData>
{
        void AddLocalPlayer(short_string name) {AddPlayer(name, true);}
private:
        int AddPlayer(short_string name, bool is_local);
        void AddPlayer(short_string name, ENetPeer* peer);
        bool OnClientConnect(ENetPeer* peer);
        void OnClientDisconnect(ENetPeer* peer);
        void OnMessage(Message& msg, ENetPeer* peer);
        std::array<Player, 256> player_list;
};

class Client : public net::client_interface<message_types>
{
public:
private:
        void OnMessage(Message &msg);
        std::array<Player, 256> player_list;
};