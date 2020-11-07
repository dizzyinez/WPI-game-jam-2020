// #include "core/net.hpp"
// #include "config.h"
// #include "components/Player.hpp"
// #include "components/Position.hpp"

// int Server::AddPlayer(short_string name, bool is_local)
// {
//         for (int i = 0; i < 256; i++)
//         {
//                 Player &player = player_list[i];
//                 if (!player.exists)
//                 {
//                         player.id = i;
//                         player.exists = true;
//                         player.local = is_local;
//                         player.name = name;
//                         return i;
//                 }
//         }
//         std::cout << "no room for more players" << std::endl;
//         return -1;
// }

// void Server::AddPlayer(short_string name, ENetPeer* peer)
// {
//         int index = AddPlayer(name, false);
//         if (index != -1)
//         {
//                 std::cout << "new player, index is " << index << std::endl;
//                 GetData(peer)->index = index;
//         }
// }

// bool Server::OnClientConnect(ENetPeer* peer)
// {
//         return true;
// }

// void Server::OnClientDisconnect(ENetPeer* peer)
// {
// }

// void Server::OnMessage(Message& msg, ENetPeer* peer)
// {
//         switch (msg.header.id)
//         {
//         case message_types::CONNECTION_REQUEST:
//                 int version_major;
//                 int version_minor;
//                 msg >> version_minor >> version_major;
//                 std::cout << "connection requested, version: " << version_major << "." << version_minor << std::endl;
//                 if (version_major == VERSION_MAJOR && version_minor == VERSION_MINOR)
//                 {
//                         Message out;
//                         out.header.id = message_types::CONNECTION_APPROVED;
//                         MessageClient(out, peer);
//                 }
//                 else
//                 {
//                         Message out;
//                         out.header.id = message_types::CONNECTION_DENIED;
//                         MessageClient(out, peer);
//                 }
//                 break;

//         case message_types::NEW_PLAYER:
//         {
//                 short_string name;
//                 msg >> name;
//                 std::cout << "new player connected" << std::endl;
//                 AddPlayer(name, peer);
//                 Message out;
//                 out.header.id = message_types::PLAYER_ACK;
//                 MessageClient(out, peer);
//         }
//         break;

//         case message_types::DISCONNECTION:
//                 std::cout << "client disconnected" << std::endl;
//                 break;

//         case message_types::PLAYER_INPUT:
//         {
//                 Player &player = player_list[GetData(peer)->index];
//                 if (player.exists)
//                 {
//                         msg >> player.entity.GetComponent<C_PlayerInput>().buttons;
//                 }
//         }
//         break;
//         }
// }