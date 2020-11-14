#pragma once
#include "core/Scene.hpp"
#include <SDL.h>
#include <SDL_image.h>
#include "game/City.hpp"
#include "game/Rail.hpp"
#include <array>

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
private:
        City* create_city(uint8_t id, float x_pos, float y_pos);
        Rail* create_rail(City* city1, City* city2);
        std::vector<Rail*> rails;
        std::array<City*, 256> cities;
};