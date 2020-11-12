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

Server* translation_unit_server;
Client* translation_unit_client;

KW_Widget* players_frame;
KW_Widget* name_box;
KW_Widget* ip_box;

void Menu::Init()
{
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
        KW_Widget* join_frame = KW_CreateFrame(Game::gui, NULL, &geom);

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
                const char* name_char = KW_GetEditboxText(name_box);
                if (strlen(name_char) > 0)
                {
                        if (!translation_unit_server->running && !translation_unit_client->IsConnected())
                        {
                                if (translation_unit_client->Connect())
                                {
                                        short_string name;
                                        memcpy(&name, KW_GetEditboxText(name_box), strlen(name_char) < sizeof(short_string) ? strlen(name_char) : sizeof(short_string));
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
                        std::cout << "player name cannot be empty!" << std::endl;
                }
        });



        geom = {20, 280, 220, 110};
        KW_Widget* host_frame = KW_CreateFrame(Game::gui, NULL, &geom);

        KW_Widget* host_label = KW_CreateLabel(Game::gui, NULL, "Host", NULL);
        KW_SetLabelAlignment(host_label, KW_LABEL_ALIGN_CENTER, 0, KW_LABEL_ALIGN_MIDDLE, 0);

        geom = {20, 10, 180, 40};
        KW_Widget* host_button = KW_CreateButton(Game::gui, host_frame, host_label, &geom);
        KW_AddWidgetMouseDownHandler(host_button, [](KW_Widget * widget, int b) {
                const char* name_char = KW_GetEditboxText(name_box);
                if (strlen(name_char) > 0)
                {
                        if (!translation_unit_server->running && !translation_unit_client->IsConnected())
                        {
                                translation_unit_server->Start();
                                short_string name;
                                memcpy(&name, KW_GetEditboxText(name_box), strlen(name_char) < sizeof(short_string) ? strlen(name_char) : sizeof(short_string));
                                translation_unit_server->AddLocalPlayer(name);
                        }
                }
                else
                {
                        std::cout << "player name cannot be empty!" << std::endl;
                }
        });

        KW_Widget* upnp_label = KW_CreateLabel(Game::gui, NULL, "Open Port With UPNP (lag)", NULL);
        KW_SetLabelAlignment(upnp_label, KW_LABEL_ALIGN_CENTER, 0, KW_LABEL_ALIGN_MIDDLE, 0);

        geom = {20, 60, 180, 40};
        KW_Widget* upnp_button = KW_CreateButton(Game::gui, host_frame, upnp_label, &geom);
        KW_AddWidgetMouseDownHandler(upnp_button, [](KW_Widget * widget, int b) {
                upnp::upnp_open(6743);
        });


        geom = {260, 10, 280, 300};
        players_frame = KW_CreateScrollbox(Game::gui, NULL, &geom);
        KW_ScrollboxHideHorizontal(players_frame);
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
}
void Menu::Clean()
{
}