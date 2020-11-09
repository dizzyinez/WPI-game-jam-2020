#include "scenes/menu.hpp"
#include "core/game.hpp"
#include <KW_gui.h>
#include <KW_frame.h>
#include <KW_label.h>
#include <KW_button.h>

Server* translation_unit_server;
Client* translation_unit_client;


void Menu::Init()
{
        server = new Server();
        client = new Client();
        translation_unit_server = server;
        translation_unit_client = client;
        KW_Rect geom = {0, 0, 220, 200};
        KW_Widget* frame = KW_CreateFrame(Game::gui, NULL, &geom);

        KW_Widget* host_label = KW_CreateLabel(Game::gui, NULL, "Host", NULL);
        KW_SetLabelAlignment(host_label, KW_LABEL_ALIGN_CENTER, 0, KW_LABEL_ALIGN_MIDDLE, 0);

        geom = {20, 140, 80, 40};
        KW_Widget* host_button = KW_CreateButton(Game::gui, frame, host_label, &geom);
        KW_AddWidgetMouseDownHandler(host_button, [](KW_Widget * widget, int b) {
                translation_unit_server->Start();
        });

        KW_Widget* join_label = KW_CreateLabel(Game::gui, NULL, "Join", NULL);
        KW_SetLabelAlignment(host_label, KW_LABEL_ALIGN_CENTER, 0, KW_LABEL_ALIGN_MIDDLE, 0);

        geom = {120, 140, 80, 40};
        KW_Widget* join_button = KW_CreateButton(Game::gui, frame, join_label, &geom);
        KW_AddWidgetMouseDownHandler(join_button, [](KW_Widget * widget, int b) {
                translation_unit_client->Connect();
        });
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
void Menu::Render(float alpha) {}
void Menu::Clean() {}