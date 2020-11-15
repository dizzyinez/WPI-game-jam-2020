#pragma once
#include "core/Scene.hpp"
#include <vector>
#include <SDL.h>
#include <KW_gui.h>
#include "math/v2d.hpp"

class Game
{
public:
        static void Update();
        static void Render(float alpha);
        static void PushScene(Scene* scene);
        static void PopScene(Scene* scene);

        static void RemoveWidget(KW_Widget* widget);
        static void FlushWidgets();

        //add a scene to the top of the scene list
        inline static bool running = true;
        inline static SDL_Renderer* renderer;
        inline static SDL_Window* window;
        inline static KW_GUI* gui;
        inline static KW_RenderDriver* kw_driver;
        inline static v2d mouse_coords = {0, 0};
private:
        //resizeable array of scene pointers
        static void push_scene(Scene* scene);
        static void pop_scene(Scene* scene);
        static void update_queue();
        inline static std::vector<Scene*> scenes;
        inline static std::vector<Scene*> add_queue;
        inline static std::vector<Scene*> remove_queue;
        inline static std::vector<KW_Widget*> widgets_to_delete;
};