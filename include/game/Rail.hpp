#pragma once
#include "core/Game.hpp"
#include "game/City.hpp"

struct Rail
{
        std::pair<City*, City*> connections;
        Rail(City* city1, City* city2)
        {
                connections = {city1, city2};
        }
        void UpdateUI()
        {
        }
        void Render()
        {
                City*& city1 = connections.first;
                City*& city2 = connections.second;
                SDL_SetRenderDrawColor(Game::renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
                SDL_RenderDrawLine(Game::renderer,
                        city1->position.x + (float)city1->size.x / 2.0f,
                        city1->position.y + (float)city1->size.y / 2.0f,
                        city2->position.x + (float)city2->size.x / 2.0f,
                        city2->position.y + (float)city2->size.y / 2.0f
                        );
                // std::cout << "rendering line" << std::endl;
        }
};