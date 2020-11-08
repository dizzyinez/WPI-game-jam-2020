#pragma once
#include "core/Scene.hpp"
#include <vector>
#include <SDL.h>

class Game
{
public:
        static void Update();
        static void Render(float alpha);

        //add a scene to the top of the scene list
        static void push_scene(Scene* scene);
        static void pop_scene(Scene* scene);
        inline static bool running = true;
        inline static SDL_Renderer* renderer;
        inline static SDL_Window* window;
private:
        //resizeable array of scene pointers
        inline static std::vector<Scene*> scenes;
};