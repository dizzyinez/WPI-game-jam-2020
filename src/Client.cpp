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
                int id;
                msg >> id;
                Player &player = player_list[id];
                player.id = id;
                player.exists;
                msg >> player.color;
                msg >> player.name;
                std::cout << "new player | name: " << (char*)&player.name << " | id: " << player.id << std::endl;

                KW_Rect geom = {0, player.id * 50, 270, 50};
                player.widget = KW_CreateFrame(Game::gui, menu->players_frame, &geom);
                geom = {0, 0, 270, 50};
                player.name_label = KW_CreateLabel(Game::gui, player.widget, (char*)&player.name, &geom);
                KW_SetLabelTextColor(player.name_label, player.color);

                break;
        }
        }
}