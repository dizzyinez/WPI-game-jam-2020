#include "core/net.hpp"
#include "core/game.hpp"
#include <KW_frame.h>
#include <KW_label.h>
#include "scenes/gameworld.hpp"

void Client::OnMessage(Message& msg)
{
        switch (msg.header.id)
        {
        case message_types::CONNECTION_APPROVED:
        {
                // std::cout << "connection approved" << std::endl;
                // Message out;
                // out.header.id = message_types::NEW_PLAYER;
                // out << short_string{"Client"};
                // MessageServer(out);
        }
        break;

        case message_types::START_GAME:
                std::cout << "Game Started" << std::endl;
                if (menu != nullptr)
                {
                        gameworld = new GameWorld;
                        gameworld->client = this;
                        gameworld->server = menu->server;
                        gameworld->players_frame = menu->players_frame;
                        Game::PushScene(gameworld);
                        Game::PopScene(menu);
                }
                // if (l_main_menu != nullptr && l_game == nullptr)
                // {
                //         l_game = new L_Game();
                //         l_game->client = l_main_menu->client; //pass client to game layer
                //         Game::PushLayer(l_game);
                //         Game::PopLayer(l_main_menu);
                // }
                // else
                // {
                //         std::cout << "Server tried to start the game, but the game was already started!" << std::endl;
                // }
                // l_main_menu = nullptr;
                break;

        case message_types::NEW_PLAYER:
        {
                uint8_t id;
                msg >> id;
                Player &player = player_list[id];
                player.id = id;
                player.exists = true;
                msg >> player.color;
                msg >> player.name;
                std::cout << "new player | name: " << (char*)&player.name << " | id: " << player.id << std::endl;

                KW_Rect geom = {0, player.id * 42, 205, 40};
                player.widget = KW_CreateFrame(Game::gui, menu->players_frame, &geom);
                geom = {0, 0, 205, 50};
                player.name_label = KW_CreateLabel(Game::gui, player.widget, (char*)&player.name, &geom);
                KW_SetLabelTextColor(player.name_label, player.color);

                break;
        }

        case message_types::NEW_RAIL:
        {
                uint8_t city_id1, city_id2, p_id;
                msg >> p_id >> city_id2 >> city_id1;
                City* city1 = gameworld->cities[city_id1];
                City* city2 = gameworld->cities[city_id2];
                gameworld->create_rail(city1, city2, p_id);
        }
        break;

        case message_types::NEW_TRAIN:
        {
                uint8_t id;
                msg >> id;

                std::size_t stop_count;
                msg >> stop_count;
                std::cout << "whahha " << static_cast<int>(id) << " " << static_cast<int>(stop_count) << std::endl;
                Train* train = gameworld->create_train(id);

                for (int i = 0; i < stop_count; i++)
                {
                        std::cout << i << std::endl;
                        uint8_t city_id;
                        msg >> city_id;
                        train->stops.emplace_back(gameworld->cities.at(city_id));
                }
        }

        case message_types::MONEY_DELTA:
        {
                if (msg.size() == 5)
                {
                        uint8_t id;
                        int cost;
                        msg >> id >> cost;
                        player_list.at(id).money -= cost;
                }
        }
        break;
        }
}