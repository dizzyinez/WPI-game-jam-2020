#pragma once
#include "core/Game.hpp"
#include "game/City.hpp"
#define PI 3.14159265
#include <cmath>
#include "utils/distance.hpp"

struct Rail
{
        inline static SDL_Texture* rail_tex;
        std::pair<City*, City*> connections;
        uint8_t owner_id;
        float length;
        v2d city1_center;
        v2d city2_center;
        Rail(City* city1, City* city2, uint8_t owner_id_)
                : owner_id(owner_id_)
        {
                connections = {city1, city2};
                city1_center = {city1->position.x + (float)city1->size.x / 2.0f, city1->position.y + (float)city1->size.y / 2.0f};
                city2_center = {city2->position.x + (float)city2->size.x / 2.0f, city2->position.y + (float)city2->size.y / 2.0f};
                length = utils::distance(city1_center.x, city1_center.y, city2_center.x, city2_center.y);
        }
        void UpdateUI()
        {
        }
        void Render(KW_Color color)
        {
                SDL_SetTextureColorMod(rail_tex, (255 + color.r) / 2, (255 + color.g) / 2, (255 + color.b) / 2);
                float track_scale = 2.0;
                City*& city1 = connections.first;
                City*& city2 = connections.second;

                SDL_Point center = {int(11.0 * track_scale / 2.0), 0};
                double angle = SDL_atan2(city1_center.x - city2_center.x, city2_center.y - city1_center.y) * (180.0 / PI);


                int tracks = ceil(length / (8.0 * track_scale - 0.5));
                double track_cos = (city2_center.x - city1_center.x) / (float)tracks;
                double track_sin = (city2_center.y - city1_center.y) / (float)tracks;
                for (int i = 1; i < tracks - 1; i++)
                {
                        SDL_Rect to = {int(city1_center.x) - int(11.0 * track_scale / 2.0) + int(i * track_cos),
                                       int(city1_center.y) + int(i * track_sin),
                                       int(11.0 * track_scale),
                                       int(8.0 * track_scale)};

                        SDL_RenderCopyEx(Game::renderer, rail_tex, NULL, &to, angle, &center, SDL_FLIP_NONE);
                }
        }
};