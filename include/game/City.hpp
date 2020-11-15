#pragma once
#include "core/Entity.hpp"
#include <KW_gui.h>
#include <KW_button.h>
#include "core/Game.hpp"

struct City : public Entity
{
        uint8_t city_id;
        uint8_t city_size;
        v2d center;
        inline static SDL_Texture* city_icons[3];
        inline static v2d sizes[3] = {
                {17, 9},
                {19, 22},
                {25, 46}
        };
        inline static KW_Surface* tileset;
        KW_Widget* button;
        City(uint8_t city_id_, uint8_t city_size_)
                : city_id(city_id_), city_size(city_size_)
        {
                texture = city_icons[city_size];
                size = sizes[city_size] * 2;
                KW_Rect geom = {10, 8, 32, 2};
                button = KW_CreateButton(Game::gui, NULL, NULL, &geom);
                KW_SetWidgetTilesetSurface(button, tileset);
        }
        ~City()
        {
                KW_DestroyWidget(button, 1);
        }
        void UpdateUI()
        {
                KW_Rect geom = {(int)position.x, (int)position.y, (int)size.x, (int)size.y};
                KW_SetWidgetGeometry(button, &geom);

                center = {position.x + (float)size.x / 2.0f, position.y + (float)size.y / 2.0f};
        }
};