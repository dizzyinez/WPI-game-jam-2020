#pragma once
#include "core/Scene.hpp"
#include <SDL.h>
#include <SDL_image.h>
#include "game/City.hpp"
#include "game/Rail.hpp"
#include "game/Train.hpp"
#include <array>
#include <KW_gui.h>

class Client;
class Server;

class GameWorld : public Scene
{
public:
        void OnWindowResize(int w, int h) {}
        void Init();
        void Update();
        void Render(float alpha);
        void Clean();
        Server* server;
        Client* client;
        std::vector<Rail*> rails;
        std::vector<Train*> trains;
        std::array<City*, 256> cities;
        Train* create_train(uint8_t owner_id_);
        KW_Widget* players_frame;
private:
        friend class Server;
        friend class Client;
        City* create_city(uint8_t id, uint8_t city_size, float x_pos, float y_pos);
        Rail* create_rail(City* city1, City* city2, uint8_t owner_id);
        Rail* find_rail(City* city1, City* city2);
};