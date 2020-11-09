#pragma once
#include "core/Scene.hpp"
#include "core/net.hpp"

class Menu : public Scene
{
public:
        void Init();
        void Update();
        void Render(float alpha);
        void Clean();
private:
        Server* server;
        Client* client;
};