#pragma once
#include "net/message.hpp"
#include "net/server.hpp"
#include "net/client.hpp"
#include <array>
#include "scenes/menu.hpp"
#include "scenes/gameworld.hpp"
#include <KW_gui.h>

enum class message_types : uint32_t
{
        CONNECTION_REQUEST,
        CONNECTION_APPROVED,
        CONNECTION_DENIED,
        NEW_PLAYER,
        PLAYER_ACK,
        START_GAME,
        DISCONNECTION,
        NEW_RAIL,
        NEW_TRAIN,
};

using short_string = std::array<char, 20>;

struct ClientData
{
        uint8_t index;
};

struct Player
{
        short_string name;
        uint8_t id = 0;
        bool exists = false;
        bool local = false;
        KW_Widget* widget;
        KW_Widget* name_label;
        KW_Color color;
        uint32_t money = 100;//Ə
};

using Message = net::message<message_types>;

class Server : public net::server_interface<message_types, ClientData>
{
public:
        void AddLocalPlayer(short_string name) {AddPlayer(name, true);}
        void OnStart();
        Menu* menu;
        GameWorld* gameworld;
        std::array<Player, 256> player_list;
private:
        int AddPlayer(short_string name, bool is_local);
        void AddPlayer(short_string name, ENetPeer* peer);
        bool OnClientConnect(ENetPeer* peer);
        void OnClientDisconnect(ENetPeer* peer);
        void OnMessage(Message& msg, ENetPeer* peer);
};

class Client : public net::client_interface<message_types>
{
public:
        Menu* menu;
        GameWorld* gameworld;
        std::array<Player, 256> player_list;
private:
        void OnMessage(Message &msg);
};