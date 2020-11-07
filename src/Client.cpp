// #include "core/net.hpp"
// #include "core/game.hpp"

// void Client::OnMessage(Message& msg)
// {
//         switch (msg.header.id)
//         {
//         case message_types::CONNECTION_APPROVED:
//         {
//                 std::cout << "connection approved" << std::endl;
//                 Message out;
//                 out.header.id = message_types::NEW_PLAYER;
//                 out << short_string{"Dondomin"};
//                 MessageServer(out);
//         }
//         break;

//         case message_types::START_GAME:
//                 std::cout << "Game Started" << std::endl;
//                 if (l_main_menu != nullptr && l_game == nullptr)
//                 {
//                         l_game = new L_Game();
//                         l_game->client = l_main_menu->client; //pass client to game layer
//                         Game::PushLayer(l_game);
//                         Game::PopLayer(l_main_menu);
//                 }
//                 else
//                 {
//                         std::cout << "Server tried to start the game, but the game was already started!" << std::endl;
//                 }
//                 // l_main_menu = nullptr;
//                 break;
//         }
// }