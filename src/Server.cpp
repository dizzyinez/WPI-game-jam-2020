#include "core/net.hpp"
#include "config.h"
#include "core/game.hpp"
#include <KW_frame.h>
#include <KW_label.h>

KW_Color colors[20] = {
        {0, 200, 200, 0},
        {200, 0, 200, 0},
        {200, 200, 0, 0},
        {200, 0, 0, 0},
        {0, 200, 0, 0},
        {0, 0, 200, 0},
        {150, 50, 0, 0},
        {0, 150, 50, 0},
        {50, 0, 150, 0},
        {100, 100, 50, 0},
        {50, 100, 100, 0},
        {100, 50, 100, 0},
};

int Server::AddPlayer(short_string name, bool is_local)
{
        for (int i = 0; i < 256; i++)
        {
                Player &player = player_list[i];
                if (!player.exists)
                {
                        player.id = i;
                        player.exists = true;
                        player.local = is_local;
                        player.name = name;
                        KW_Rect geom = {0, i*50, 270, 50};
                        player.widget = KW_CreateFrame(Game::gui, menu->players_frame, &geom);
                        geom = {0, 0, 270, 50};
                        player.name_label = KW_CreateLabel(Game::gui, player.widget, (char*)&name, &geom);
                        player.color = {
                                0, 0, 0, 0};
                        if (i < 20)
                        {
                                player.color = colors[i];
                        }
                        KW_SetLabelTextColor(player.name_label, player.color);
                        return i;
                }
        }
        std::cout << "no room for more players" << std::endl;
        return -1;
}

void Server::AddPlayer(short_string name, ENetPeer* peer)
{
        int index = AddPlayer(name, false);
        if (index != -1)
        {
                std::cout << "new player, index is " << index << std::endl;
                GetData(peer)->index = index;
        }
}

bool Server::OnClientConnect(ENetPeer* peer)
{
        return true;
}

void Server::OnClientDisconnect(ENetPeer* peer)
{
}

void Server::OnMessage(Message& msg, ENetPeer* peer)
{
        switch (msg.header.id)
        {
        case message_types::CONNECTION_REQUEST:
                int version_major;
                int version_minor;
                short_string name;
                msg >> version_minor >> version_major >> name;
                // std::cout << "connection requested, version: " << version_major << "." << version_minor << std::endl;
                if (version_major == VERSION_MAJOR && version_minor == VERSION_MINOR)
                {
                        Message out;
                        out.header.id = message_types::CONNECTION_APPROVED;
                        MessageClient(out, peer);
                        AddPlayer(name, peer);
                        for (int i = 0; i < 256; i++)
                        {
                                Player &player = player_list[i];
                                if (player.exists)
                                {
                                        Message msg;
                                        msg.header.id = message_types::NEW_PLAYER;
                                        msg << player.name << player.color << player.id;
                                        MessageClient(msg, peer);
                                }
                        }
                }
                else
                {
                        Message out;
                        out.header.id = message_types::CONNECTION_DENIED;
                        MessageClient(out, peer);
                }
                break;

        case message_types::NEW_PLAYER:
        {
                short_string name;
                msg >> name;
                std::cout << "new player connected" << std::endl;
                AddPlayer(name, peer);
                Message out;
                out.header.id = message_types::PLAYER_ACK;
                MessageClient(out, peer);
        }
        break;

        case message_types::DISCONNECTION:
                std::cout << "client disconnected" << std::endl;
                break;
        }
}