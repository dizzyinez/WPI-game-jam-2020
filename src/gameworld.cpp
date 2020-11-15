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
#include "utils/distance.hpp"
#include <SDL_mixer.h>

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
KW_Widget* decline_button;
KW_Widget* confirm_label;
void remove_frame()
{
        KW_Rect geom = {-999, -999, 0, 0};
        KW_SetWidgetGeometry(select_frame, &geom);
}

int temp_cost;
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

void train_confirm(KW_Widget * widget, int b);
void train_decline(KW_Widget * widget, int b) {
        Mix_PlayChannel(0, Game::select, 0);
        KW_Rect geom = {-9999, 0, 135, 80}; KW_SetWidgetGeometry(confirm_frame, &geom);
        train_stops.clear();
        KW_RemoveWidgetMouseDownHandler(confirm_button, train_confirm);
        KW_RemoveWidgetMouseDownHandler(decline_button, train_decline);
}

void train_confirm(KW_Widget * widget, int b) {
        Mix_PlayChannel(2, Game::train, 0);
        KW_Rect geom = {-9999, 0, 135, 80}; KW_SetWidgetGeometry(confirm_frame, &geom);
        Message msg;
        msg.header.id = message_types::NEW_TRAIN;
        for (auto it = train_stops.rbegin(); it != train_stops.rend(); ++it)
        {
                msg << (*it)->city_id;
        }
        msg << train_stops.size();
        if (tu_server->running)
        {
                if (player_list->at(player_id).money >= temp_cost)
                {
                        Train* train = tu_gw->create_train(player_id);

                        for (City * stop : train_stops)
                        {
                                train->stops.emplace_back(stop);
                        }
                        msg << player_id;
                        tu_server->MessageAllClients(msg);

                        // player_list->at(player_id).money -= temp_cost;
                        // Message md;
                        // md.header.id = message_types::MONEY_DELTA;
                        // // md << temp_cost << player_id;
                        // md << temp_cost;
                        // md << player_id;
                        // tu_server->MessageAllClients(md);
                        // tu_server->flush();
                }
        }
        else if (tu_client->IsConnected())
        {
                msg << temp_cost;
                tu_client->MessageServer(msg);
        }
        train_stops.clear();
        KW_RemoveWidgetMouseDownHandler(confirm_button, train_confirm);
        KW_RemoveWidgetMouseDownHandler(decline_button, train_decline);
}

void rail_decline(KW_Widget * widget, int b);
void rail_confirm(KW_Widget * widget, int b) {
        Mix_PlayChannel(1, Game::rail, 0);
        KW_Rect geom = {-9999, 0, 135, 80}; KW_SetWidgetGeometry(confirm_frame, &geom);
        if (tu_client->IsConnected())
        {
                Message msg;
                msg.header.id = message_types::NEW_RAIL;
                msg << to_city->city_id << from_city->city_id << temp_cost;
                tu_client->MessageServer(msg);
        }
        else if (tu_server->running)
        {
                if (player_list->at(player_id).money >= temp_cost)
                {
                        tu_gw->create_rail(from_city, to_city, player_id);
                        Message msg;
                        msg.header.id = message_types::NEW_RAIL;
                        msg << to_city->city_id << from_city->city_id << player_id;
                        tu_server->MessageAllClients(msg);

                        player_list->at(player_id).money -= temp_cost;
                        Message md;
                        md.header.id = message_types::MONEY_DELTA;
                        md << temp_cost << player_id;
                        tu_server->MessageAllClients(md);
                }
        }
        from_city = nullptr;
        to_city = nullptr;

        KW_RemoveWidgetMouseDownHandler(confirm_button, rail_confirm);
        KW_RemoveWidgetMouseDownHandler(decline_button, rail_decline);
}
void rail_decline(KW_Widget * widget, int b) {
        Mix_PlayChannel(0, Game::select, 0);
        KW_Rect geom = {-9999, 0, 135, 80}; KW_SetWidgetGeometry(confirm_frame, &geom);
        from_city = nullptr;
        to_city = nullptr;
        KW_RemoveWidgetMouseDownHandler(confirm_button, rail_confirm);
        KW_RemoveWidgetMouseDownHandler(decline_button, rail_decline);
}



void GameWorld::Init()
{
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
        KW_AddWidgetMouseDownHandler(exit_button, [](KW_Widget * widget, int b) {
                Mix_PlayChannel(0, Game::select, 0); remove_frame();
        });
        geom = {5, 10, 80, 30};
        KW_Widget* track_label = KW_CreateLabel(Game::gui, NULL, "New Track", NULL);
        KW_Widget* track_button = KW_CreateButton(Game::gui, select_frame, track_label, &geom);
        KW_AddWidgetMouseDownHandler(track_button, [](KW_Widget * widget, int b) {
                Mix_PlayChannel(0, Game::select, 0);
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
                Mix_PlayChannel(0, Game::select, 0);
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
                Mix_PlayChannel(0, Game::select, 0);
                KW_Rect geom = {-999, -999, 80, 30};
                KW_SetWidgetGeometry(line_end_button, &geom);
                geom = {(1080 / 2) - (135 / 2), (780 / 2) - (80 / 2), 135, 80};
                KW_SetWidgetGeometry(confirm_frame, &geom);
                select_mode = 0;
                from_city = nullptr;

                int cost = 25;
                temp_cost = cost;
                std::string cnf;
                cnf += "Buy Train for Ə" + std::to_string(cost) + "?";
                KW_SetLabelText(confirm_label, cnf.data());


                KW_AddWidgetMouseDownHandler(confirm_button, train_confirm);
                KW_AddWidgetMouseDownHandler(decline_button, train_decline);
        });




        geom = {-999, 0, 135, 80};
        confirm_frame = KW_CreateFrame(Game::gui, NULL, &geom);

        geom = {5, 40, 60, 30};
        KW_Widget* decline_button_label = KW_CreateLabel(Game::gui, NULL, "Decline", NULL);
        decline_button = KW_CreateButton(Game::gui, confirm_frame, decline_button_label, &geom);
        KW_AddWidgetMouseDownHandler(decline_button, [](KW_Widget * widget, int b) {
                Mix_PlayChannel(0, Game::select, 0); KW_Rect geom = {-9999, 0, 135, 80}; KW_SetWidgetGeometry(confirm_frame, &geom);
        });

        geom = {65, 40, 60, 30};
        KW_Widget* confirm_button_label = KW_CreateLabel(Game::gui, NULL, "Confirm", NULL);
        confirm_button = KW_CreateButton(Game::gui, confirm_frame, confirm_button_label, &geom);

        geom = {0, 0, 125, 35};
        confirm_label = KW_CreateLabel(Game::gui, confirm_frame, "Buy Track for Ə100000?", &geom);
        KW_SetLabelAlignment(confirm_label, KW_LABEL_ALIGN_CENTER, 0, KW_LABEL_ALIGN_BOTTOM, 0);




        City::tileset = KW_LoadSurface(Game::kw_driver, "assets/city_tileset.png");
        cities.fill(nullptr);



        create_city(0, 0, 951, 295);
        create_city(1, 0, 830, 21);
        create_city(2, 0, 570, 657);
        create_city(3, 1, 273, 665);
        create_city(4, 1, 312, 642);
        create_city(5, 1, 648, 9);
        create_city(6, 1, 264, 507);
        create_city(7, 1, 339, 413);
        create_city(8, 2, 566, 510);
        create_city(9, 1, 735, 218);
        create_city(10, 0, 228, 580);
        create_city(11, 1, 448, 383);
        create_city(12, 2, 239, 600);
        create_city(13, 1, 866, 650);
        create_city(14, 0, 460, 687);
        create_city(15, 1, 826, 575);
        create_city(16, 0, 806, 211);
        create_city(17, 1, 415, 602);
        create_city(18, 2, 963, 316);
        create_city(19, 1, 397, 101);
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
                Mix_PlayChannel(0, Game::select, 0);
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
                                                KW_Rect geom = {(1080 / 2) - (135 / 2), (780 / 2) - (80 / 2), 135, 80};
                                                KW_SetWidgetGeometry(confirm_frame, &geom);
                                                int cost = floor(utils::distance(from_city->center.x, from_city->center.y, to_city->center.x, to_city->center.y));
                                                temp_cost = cost;
                                                std::string cnf;
                                                cnf += "Buy Rail for Ə" + std::to_string(cost) + "?";
                                                KW_SetLabelText(confirm_label, cnf.data());

                                                KW_AddWidgetMouseDownHandler(confirm_button, rail_confirm);
                                                KW_AddWidgetMouseDownHandler(decline_button, rail_decline);

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


