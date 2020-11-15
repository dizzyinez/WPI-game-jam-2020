#pragma once
#include "core/Game.hpp"
#include "game/City.hpp"
#include "game/Rail.hpp"
#include "utils/distance.hpp"
#include <vector>

struct Passenger
{
        City* destination;
        float ticket_cost;
};

struct Train : public Entity
{
        inline static SDL_Texture* train_tex;
        uint8_t owner_id;
        bool selected = false;
        Train(uint8_t owner_id_) : owner_id(owner_id_) {texture = train_tex; size = {14 * 1.5, 1.5 * 25};}
        std::vector<City*> stops;
        std::vector<Passenger> passengers;
        Rail* curr_rail = nullptr;
        int curr_stop = 0;
        int direction = 1;

        float speed = 0.5f;
        float distance = 0.0f;
        int stop_time = 5 * 60;
        int stop_counter = 0;

        // KW_Widget* button;
        void UpdateUI()
        {
        }
};