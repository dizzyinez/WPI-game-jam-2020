#include "scenes/menu.hpp"
#include "core/game.hpp"
#include "core/net.hpp"
#include "Config.h"
#include <KW_gui.h>
#include <KW_frame.h>
#include <KW_label.h>
#include <KW_button.h>
#include <KW_scrollbox.h>
#include <KW_editbox.h>
#include "utils/upnp.hpp"
#include "scenes/gameworld.hpp"

Server* translation_unit_server;
Client* translation_unit_client;

KW_Widget* name_box;
KW_Widget* ip_box;
KW_Widget* tu_pf;
Menu* tu_this;

SDL_Texture* background;
void Menu::OnWindowResize(int w, int h)
{
        // int y_off = 0;
        // KW_Rect geom;
        // KW_GetWidgetGeometry(players_frame, &geom);
        // geom.x = ((float)w / 2.0);
        // KW_SetWidgetGeometry(players_frame, &geom);
}
void Menu::Init()
{
        Game::music = Mix_LoadMUS("assets/Audio/Scott_Elias_-_Drift_Locomotion_Commotion_OST.mp3");
        Mix_PlayMusic(Game::music, -1);
        Game::select = Mix_LoadWAV("assets/Audio/select2.wav");
        Game::rail = Mix_LoadWAV("assets/Audio/RailBuildSound.wav");
        Game::train = Mix_LoadWAV("TrainWhistle.wav");

        SDL_Surface* surface6 = IMG_Load("assets/TitleScreen.png");
        background = SDL_CreateTextureFromSurface(Game::renderer, surface6);
        SDL_FreeSurface(surface6);

        std::cout << "init\n";
        tu_this = this;
        server = new Server();
        client = new Client();
        translation_unit_server = server;
        translation_unit_client = client;
        server->menu = this;
        client->menu = this;
        KW_Rect geom = {20, 20, 220, 90};
        name_frame = KW_CreateFrame(Game::gui, NULL, &geom);

        geom = {20, 10, 180, 20};
        KW_Widget* name_label = KW_CreateLabel(Game::gui, name_frame, "Name:", &geom);
        KW_SetLabelAlignment(name_label, KW_LABEL_ALIGN_CENTER, 0, KW_LABEL_ALIGN_MIDDLE, 0);

        geom = {20, 30, 180, 50};
        name_box = KW_CreateEditbox(Game::gui, name_frame, "", &geom);



        geom = {20, 120, 220, 150};
        join_frame = KW_CreateFrame(Game::gui, NULL, &geom);

        geom = {20, 10, 180, 20};
        KW_Widget* ip_label = KW_CreateLabel(Game::gui, join_frame, "IP to connect to:", &geom);
        KW_SetLabelAlignment(ip_label, KW_LABEL_ALIGN_CENTER, 0, KW_LABEL_ALIGN_MIDDLE, 0);

        geom = {20, 30, 180, 50};
        ip_box = KW_CreateEditbox(Game::gui, join_frame, "", &geom);

        KW_Widget* join_label = KW_CreateLabel(Game::gui, NULL, "Join", NULL);
        KW_SetLabelAlignment(join_label, KW_LABEL_ALIGN_CENTER, 0, KW_LABEL_ALIGN_MIDDLE, 0);

        geom = {20, 90, 180, 40};
        KW_Widget* join_button = KW_CreateButton(Game::gui, join_frame, join_label, &geom);
        KW_AddWidgetMouseDownHandler(join_button, [](KW_Widget * widget, int b) {
                Mix_PlayChannel(0, Game::select, 0);
                const char* name_char = KW_GetEditboxText(name_box);
                const char* ip = KW_GetEditboxText(ip_box);
                int length = strlen(name_char);
                if (length > 0 && strlen(ip) > 0)
                {
                        if (!translation_unit_server->running && !translation_unit_client->IsConnected())
                        {
                                if (translation_unit_client->Connect(ip))
                                {
                                        std::cout << "ip: " << ip << std::endl;
                                        short_string name;
                                        strncpy(reinterpret_cast<char*>(&name), name_char, sizeof(short_string));
                                        char end = '\0';
                                        std::string ip;


                                        memcpy(&name + sizeof(short_string), &end, 1);
                                        translation_unit_server->AddLocalPlayer(name);
                                        Message msg;
                                        msg.header.id = message_types::CONNECTION_REQUEST;
                                        int version_major = VERSION_MAJOR;
                                        int version_minor = VERSION_MINOR;
                                        msg << name << version_major << version_minor;
                                        translation_unit_client->MessageServer(msg);
                                        translation_unit_client->flush();
                                }
                        }
                }
                else
                {
                        std::cout << "player name and IP can't be empty!" << std::endl;
                }
        });



        geom = {20, 280, 220, 180};
        host_frame = KW_CreateFrame(Game::gui, NULL, &geom);

        KW_Widget* upnp_label = KW_CreateLabel(Game::gui, NULL, "Open Port With UPNP (lag)", NULL);
        KW_SetLabelAlignment(upnp_label, KW_LABEL_ALIGN_CENTER, 0, KW_LABEL_ALIGN_MIDDLE, 0);

        geom = {20, 20, 180, 40};
        KW_Widget* upnp_button = KW_CreateButton(Game::gui, host_frame, upnp_label, &geom);
        KW_AddWidgetMouseDownHandler(upnp_button, [](KW_Widget * widget, int b) {
                Mix_PlayChannel(0, Game::select, 0);
                upnp::upnp_open(6743);
        });

        KW_Widget* host_label = KW_CreateLabel(Game::gui, NULL, "Host", NULL);
        KW_SetLabelAlignment(host_label, KW_LABEL_ALIGN_CENTER, 0, KW_LABEL_ALIGN_MIDDLE, 0);

        geom = {20, 70, 180, 40};
        KW_Widget* host_button = KW_CreateButton(Game::gui, host_frame, host_label, &geom);
        KW_AddWidgetMouseDownHandler(host_button, [](KW_Widget * widget, int b) {
                Mix_PlayChannel(0, Game::select, 0);
                const char* name_char = KW_GetEditboxText(name_box);
                int length = strlen(name_char);
                if (length > 0)
                {
                        if (!translation_unit_server->running && !translation_unit_client->IsConnected())
                        {
                                translation_unit_server->Start();
                                translation_unit_server->OnStart();
                                short_string name;
                                strncpy(reinterpret_cast<char*>(&name), name_char, sizeof(short_string));
                                char end = '\0';
                                memcpy(&name + sizeof(short_string), &end, 1);
                                translation_unit_server->AddLocalPlayer(name);
                        }
                }
                else
                {
                        std::cout << "player name cannot be empty!" << std::endl;
                }
        });

        KW_Widget* play_label = KW_CreateLabel(Game::gui, NULL, "Start Game", NULL);
        KW_SetLabelAlignment(upnp_label, KW_LABEL_ALIGN_CENTER, 0, KW_LABEL_ALIGN_MIDDLE, 0);

        geom = {20, 120, 180, 40};
        KW_Widget* play_button = KW_CreateButton(Game::gui, host_frame, play_label, &geom);
        KW_AddWidgetMouseDownHandler(play_button, [](KW_Widget * widget, int b) {
                Mix_PlayChannel(0, Game::select, 0);
                if (translation_unit_server->running)
                {
                        GameWorld* gw = new GameWorld;
                        Game::PushScene(gw);
                        translation_unit_server->gameworld = gw;
                        translation_unit_server->menu = nullptr;
                        gw->server = translation_unit_server;
                        gw->client = translation_unit_client;
                        gw->players_frame = tu_pf;
                        Game::PopScene(tu_this);
                        Message msg;
                        msg.header.id = message_types::START_GAME;
                        translation_unit_server->MessageAllClients(msg);
                }
        });

        geom = {13, 460, 235, 250};
        players_frame = KW_CreateScrollbox(Game::gui, NULL, &geom);
        KW_ScrollboxHideHorizontal(players_frame);
        tu_pf = players_frame;
}
void Menu::Update()
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
}
void Menu::Render(float alpha)
{
        SDL_RenderCopy(Game::renderer, background, NULL, NULL);
}
void Menu::Clean()
{
        // Game::RemoveWidget(players_frame);
        Game::RemoveWidget(host_frame);
        Game::RemoveWidget(name_frame);
        Game::RemoveWidget(join_frame);
}