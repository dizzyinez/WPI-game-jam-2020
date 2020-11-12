#pragma once
#include "core/Scene.hpp"
#include <KW_gui.h>
// #include "core/net.hpp"

class Client;
class Server;

class Menu : public Scene
{
public:
        void Init();
        void Update();
        void Render(float alpha);
        void Clean();
        KW_Widget* players_frame;
        KW_Widget* name_frame;
private:
        Server* server;
        Client* client;
};