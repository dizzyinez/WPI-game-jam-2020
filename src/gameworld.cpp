#include "scenes/gameworld.hpp"
#include "core/game.hpp"
#include "core/net.hpp"
#include <algorithm>
#include <KW_gui.h>
#include <KW_frame.h>
#include <KW_label.h>
#include <KW_button.h>
#include <KW_scrollbox.h>
#include <KW_editbox.h>
#include <string>

City* from_city = nullptr;
City* to_city = nullptr;
GameWorld* tu_gw;
Server* tu_server;
Client* tu_client;
std::array<Player, 256>* player_list;
uint8_t player_id;

int select_mode = 0;
KW_Widget* select_frame;
KW_Widget* line_end_button;
KW_Widget* players_frame;

KW_Widget* confirm_frame;
KW_Widget* confirm_button;
KW_Widget* confirm_label;

SDL_Texture * backgroundTexture;

void remove_frame()
{
        KW_Rect geom = {-999, -999, 0, 0};
        KW_SetWidgetGeometry(select_frame, &geom);
}

City* temp_city;
std::vector<City*> train_stops;
//the gui library was written in c, so I can't use lambda captures, so I have to do this -_-
City* get_city_from_button(KW_Widget* button)
{
        for (City* c : tu_gw->cities)
        {
                if (c->button == button)
                        return c;
        }
        return nullptr;
};


void GameWorld::Init()
{
        SDL_Surface * surfaceBack = IMG_Load("assets/Background.png");
        backgroundTexture = SDL_CreateTextureFromSurface(Game::renderer, surfaceBack);
        SDL_FreeSurface(surfaceBack);

        KW_Rect geom = {0, 0, 235, 250};
        KW_SetWidgetGeometry(players_frame, &geom);
        if (client->IsConnected())
        {
                player_list = &client->player_list;
        }
        else if (server->running)
        {
                player_list = &server->player_list;
        }
        for (Player p : *player_list)
        {
                if (p.exists && p.local)
                {
                        player_id = p.id;
                        break;
                }
        }

        tu_gw = this;
        tu_server = server;
        tu_client = client;
        SDL_Surface * surface1 = IMG_Load("assets/small city.png");
        City::city_icons[0] = SDL_CreateTextureFromSurface(Game::renderer, surface1);
        SDL_FreeSurface(surface1);
        SDL_Surface* surface2 = IMG_Load("assets/medium city.png");
        City::city_icons[1] = SDL_CreateTextureFromSurface(Game::renderer, surface2);
        SDL_FreeSurface(surface2);
        SDL_Surface* surface3 = IMG_Load("assets/large city.png");
        City::city_icons[2] = SDL_CreateTextureFromSurface(Game::renderer, surface3);
        SDL_FreeSurface(surface3);
        SDL_Surface* surface4 = IMG_Load("assets/rail.png");
        Rail::rail_tex = SDL_CreateTextureFromSurface(Game::renderer, surface4);
        SDL_FreeSurface(surface4);
        SDL_Surface* surface5 = IMG_Load("assets/train.png");
        Train::train_tex = SDL_CreateTextureFromSurface(Game::renderer, surface5);
        SDL_FreeSurface(surface5);

        geom = {-999, -999, 0, 0};
        select_frame = KW_CreateFrame(Game::gui, NULL, &geom);
        geom = {80, 0, 10, 10};
        KW_Widget* exit_label = KW_CreateLabel(Game::gui, NULL, "X", NULL);
        KW_Widget* exit_button = KW_CreateButton(Game::gui, select_frame, exit_label, &geom);
        KW_AddWidgetMouseDownHandler(exit_button, [](KW_Widget * widget, int b) {remove_frame();});
        geom = {5, 10, 80, 30};
        KW_Widget* track_label = KW_CreateLabel(Game::gui, NULL, "New Track", NULL);
        KW_Widget* track_button = KW_CreateButton(Game::gui, select_frame, track_label, &geom);
        KW_AddWidgetMouseDownHandler(track_button, [](KW_Widget * widget, int b) {
                remove_frame();
                select_mode = 1;
                if (temp_city != nullptr)
                {
                        City* city = temp_city;
                        if (from_city == nullptr)
                        {
                                from_city = city;
                                return;
                        }
                        from_city = nullptr;
                        to_city = nullptr;
                }
        });


        geom = {5, 45, 80, 30};
        KW_Widget* train_label = KW_CreateLabel(Game::gui, NULL, "New Train", NULL);
        KW_Widget* train_button = KW_CreateButton(Game::gui, select_frame, train_label, &geom);
        KW_AddWidgetMouseDownHandler(train_button, [](KW_Widget * widget, int b) {
                remove_frame();
                if (temp_city != nullptr)
                {
                        from_city = temp_city;
                        select_mode = 2;
                        train_stops.clear();
                        train_stops.emplace_back(temp_city);
                }
        });

        geom = {-999, -999, 80, 30};
        KW_Widget* line_end_label = KW_CreateLabel(Game::gui, NULL, "End Line", NULL);
        line_end_button = KW_CreateButton(Game::gui, NULL, line_end_label, &geom);
        KW_AddWidgetMouseDownHandler(line_end_button, [](KW_Widget * widget, int b) {
                KW_Rect geom = {-999, -999, 80, 30};
                KW_SetWidgetGeometry(line_end_button, &geom);
                select_mode = 0;
                from_city = nullptr;

                Message msg;
                msg.header.id = message_types::NEW_TRAIN;
                for (auto it = train_stops.rbegin(); it != train_stops.rend(); ++it)
                {
                        msg << (*it)->city_id;
                }
                msg << train_stops.size();
                if (tu_server->running)
                {
                        Train* train = tu_gw->create_train(player_id);

                        for (City * stop : train_stops)
                        {
                                train->stops.emplace_back(stop);
                        }
                        msg << player_id;
                        tu_server->MessageAllClients(msg);
                }
                else
                {
                        tu_client->MessageServer(msg);
                }
                train_stops.clear();
        });


        geom = {-999, -999, 120, 80};
        confirm_frame = KW_CreateFrame(Game::gui, NULL, &geom);
        KW_Widget* confirm_button_label = KW_CreateLabel(Game::gui, NULL, "Confirm", NULL);
        geom = {10, 40, 100, 30};
        confirm_button = KW_CreateButton(Game::gui, confirm_frame, confirm_button_label, &geom);
        confirm_label = KW_CreateLabel(Game::gui, NULL, "Buy?", NULL);;


        City::tileset = KW_LoadSurface(Game::kw_driver, "assets/city_tileset.png");
        cities.fill(nullptr);

        auto c1 = create_city(0, 0, 200, 600);
        auto c2 = create_city(1, 1, 700, 350);
        create_city(2, 2, 200, 250);
}
void GameWorld::Update()
{
        if (server->running)
        {
                server->Update();
        }
        else
        if (client->IsConnected())
        {
                client->Update();
        }
        for (Train* t : trains)
        {
                if (t->stops.size() > 1)
                {
                        if (t->curr_rail != nullptr)
                        {
                                t->distance += t->speed;
                                if (t->distance >= t->curr_rail->length)
                                {
                                        //arrived at next stop
                                        t->curr_stop += t->direction;
                                        t->curr_rail = nullptr;
                                        t->distance = 0;
                                }
                                else
                                {
                                        float percent = t->distance / t->curr_rail->length;
                                        t->position = t->stops.at(t->curr_stop)->center + ((t->stops.at(t->curr_stop + t->direction)->center - t->stops.at(t->curr_stop)->center) * percent) - v2d(14 * 0.75, 25 * 0.75);
                                }
                        }
                        else
                        {
                                t->stop_counter++;
                                if (t->stop_counter >= t->stop_time)
                                {
                                        //leaving stop
                                        if (t->curr_stop == t->stops.size() - 1)
                                                t->direction = -1;
                                        if (t->curr_stop == 0)
                                                t->direction = 1;
                                        t->curr_rail = find_rail(t->stops.at(t->curr_stop), t->stops.at(t->curr_stop + t->direction));
                                        t->stop_counter = 0;
                                }
                        }
                }
                else
                {
                        t->position = t->stops.at(t->curr_stop)->center - v2d(14 * 0.75, 25 * 0.75);
                }
        }
}
void GameWorld::Render(float alpha)
{
        SDL_RenderCopy(Game::renderer, backgroundTexture, NULL, NULL);
        for (Rail* r : rails)
        {
                r->Render(player_list->at(r->owner_id).color);
        }
        for (City* c : cities)
        {
                if (c != nullptr)
                        c->UpdateUI();
        }
        for (Entity* e : entities)
        {
                SDL_Rect dest = {int(e->position.x), int(e->position.y), int(e->size.x), int(e->size.y)};
                SDL_RenderCopy(Game::renderer, e->texture, NULL, &dest);
        }
        if (from_city != nullptr)
        {
                SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
                SDL_RenderDrawLine(Game::renderer, from_city->position.x + (from_city->size.x / 2), from_city->position.y + (from_city->size.y / 2), Game::mouse_coords.x, Game::mouse_coords.y);
        }
        for (Player p : *player_list)
        {
                if (p.exists)
                {
                        std::string text;
                        text += (char*)&p.name;
                        text += " | Ə";
                        text += std::to_string(p.money);
                        KW_SetLabelText(p.name_label, text.data());
                }
        }
}

void GameWorld::Clean()
{
        for (City * c : cities)
        {
                if (c != nullptr)
                        delete c;
        }
        for (Rail* r : rails)
        {
                delete r;
        }
}

City* GameWorld::create_city(uint8_t id, uint8_t city_size, float x_pos = 0, float y_pos = 0)
{
        City* city = new City(id, city_size);
        cities[id] = city;
        entities.emplace_back((Entity*)city);
        city->position = {x_pos, y_pos};
        KW_AddWidgetMouseDownHandler(city->button, [](KW_Widget * widget, int b) {
                City* city = get_city_from_button(widget);
                if (city != nullptr)
                {
                        if (select_mode == 0)
                        {
                                temp_city = city;
                                KW_Rect geom = {city->position.x + city->size.x, city->center.y, 90, 80};
                                KW_SetWidgetGeometry(select_frame, &geom);
                                return;
                        }
                        else if (select_mode == 1)
                        {
                                if (from_city != city)
                                {
                                        to_city = city;
                                        bool rail_exists = false;
                                        for (Rail* r : tu_gw->rails)
                                        {
                                                if ((r->connections.first == from_city || r->connections.second == from_city) &&
                                                    (r->connections.first == to_city || r->connections.second == to_city))
                                                {
                                                        rail_exists = true;
                                                        break;
                                                }
                                        }
                                        if (!rail_exists)
                                        {
                                                if (tu_client->IsConnected())
                                                {
                                                        Message msg;
                                                        msg.header.id = message_types::NEW_RAIL;
                                                        msg << to_city->city_id << from_city->city_id;
                                                        tu_client->MessageServer(msg);
                                                }
                                                else if (tu_server->running)
                                                {
                                                        tu_gw->create_rail(from_city, to_city, player_id);
                                                        Message msg;
                                                        msg.header.id = message_types::NEW_RAIL;
                                                        msg << to_city->city_id << from_city->city_id << player_id;
                                                        tu_server->MessageAllClients(msg);
                                                }
                                                from_city = nullptr;
                                                to_city = nullptr;
                                                select_mode = 0;
                                        }
                                }
                        }
                        else if (select_mode == 2)
                        {
                                if (city != train_stops.back())
                                {
                                        from_city = city;
                                        train_stops.emplace_back(city);
                                        KW_Rect geom = {city->position.x + city->size.x, city->center.y, 80, 30};
                                        KW_SetWidgetGeometry(line_end_button, &geom);
                                }
                        }
                }
        });
        return city;
}

Rail* GameWorld::create_rail(City* city1, City* city2, uint8_t owner_id)
{
        Rail* rail = new Rail(city1, city2, owner_id);
        rails.emplace_back(rail);
        return rail;
}

Rail* GameWorld::find_rail(City* city1, City* city2)
{
        for (Rail* r : rails)
        {
                if ((r->connections.first == city1 || r->connections.second == city1) &&
                    (r->connections.first == city2 || r->connections.second == city2))
                {
                        return r;
                }
        }
        return nullptr;
}

Train* GameWorld::create_train(uint8_t owner_id_)
{
        Train* train = new Train(owner_id_);
        // train->stops.emplace_back(starting_city);
        trains.emplace_back(train);
        entities.emplace_back((Entity*)train);
        train->stop_counter = 99999;
        train->position = v2d(-999, -999);
        return train;
}