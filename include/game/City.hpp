#pragma once
#include "core/Entity.hpp"
#include <KW_gui.h>
#include <KW_button.h>
#include "core/Game.hpp"

struct City : public Entity
{
        uint8_t city_id;
        inline static SDL_Texture* city_icon;
        inline static KW_Surface* tileset;
        KW_Widget* button;
        City(uint8_t city_id_)
                : city_id(city_id_)
        {
                texture = city_icon;
                KW_Rect geom = {.x = 10, .y = 8, .w = 32, .h = 32};
                button = KW_CreateButton(Game::gui, NULL, NULL, &geom);
                KW_SetWidgetTilesetSurface(button, tileset);
                KW_AddWidgetMouseDownHandler(button, [](KW_Widget * widget, int b) {std::cout << "pressed" << std::endl;});
        }
        void UpdateUI()
        {
                KW_Rect geom = {.x = (int)position.x, .y = (int)position.y, .w = (int)size.x, .h = (int)size.y};
                KW_SetWidgetGeometry(button, &geom);
        }
};