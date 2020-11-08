#pragma once
#include "core/Scene.hpp"
#include <vector>

class Game
{
public:
        static void Update();
        static void Render(float alpha);

        //add a scene to the top of the scene list
        static void push_scene(Scene* scene);
        static void pop_scene(Scene* scene);
private:
        //resizeable array of scene pointers
        inline static std::vector<Scene*> scenes;
};