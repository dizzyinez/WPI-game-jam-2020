#include "core/net.hpp"
#include "config.h"
#include "core/game.hpp"
#include <KW_frame.h>
#include <KW_label.h>
#include "game/Train.hpp"

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
                        KW_Rect geom = {0, i*42, 205, 40};
                        player.widget = KW_CreateFrame(Game::gui, menu->players_frame, &geom);
                        geom = {0, 0, 205, 50};
                        player.name_label = KW_CreateLabel(Game::gui, player.widget, (char*)&name, &geom);
                        player.color = {0, 0, 0, 0};
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

        case message_types::NEW_RAIL:
        {
                uint8_t city_id1;
                uint8_t city_id2;
                int cost;
                msg >> cost >> city_id1 >> city_id2;
                City* city1 = gameworld->cities[city_id1];
                City* city2 = gameworld->cities[city_id2];

                bool rail_exists = false;
                for (Rail* r : gameworld->rails)
                {
                        if ((r->connections.first == city1 || r->connections.second == city1) &&
                            (r->connections.first == city2 || r->connections.second == city2))
                        {
                                rail_exists = true;
                                break;
                        }
                }
                if (!rail_exists)
                {
                        if (player_list.at(GetData(peer)->index).money >= cost)
                        {
                                gameworld->create_rail(city1, city2, GetData(peer)->index);
                                Message msg;
                                msg.header.id = message_types::NEW_RAIL;
                                msg << city1->city_id << city2->city_id << GetData(peer)->index;
                                MessageAllClients(msg);

                                player_list.at(GetData(peer)->index).money -= cost;
                                msg.header.id = message_types::MONEY_DELTA;
                                msg << cost << GetData(peer)->index;
                                MessageAllClients(msg);
                        }
                }
        }
        break;

        case message_types::NEW_TRAIN:
        {
                int cost;
                msg >> cost;
                if (player_list.at(GetData(peer)->index).money >= cost)
                {
                        Message out = msg;
                        out << GetData(peer)->index;
                        MessageAllClients(out);

                        std::size_t stop_count;
                        msg >> stop_count;
                        Train* train = gameworld->create_train(GetData(peer)->index);

                        for (int i = 0; i < stop_count; i++)
                        {
                                uint8_t city_id;
                                msg >> city_id;
                                train->stops.emplace_back(gameworld->cities.at(city_id));
                        }

                        player_list.at(GetData(peer)->index).money -= cost;
                        msg.header.id = message_types::MONEY_DELTA;
                        msg << cost << GetData(peer)->index;
                        MessageAllClients(msg);
                }
        }
        break;
        }
}

void Server::OnStart()
{
        // Player p;
        // p.exists = false;
        // p.local = false;
        // player_list.fill(p);
}