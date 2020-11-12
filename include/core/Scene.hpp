#pragma once
#include "common.hpp"
#include "core/Entity.hpp"
#include <vector>
class Scene
{
protected:
        //resizeable array of entities
        std::vector<Entity*> entities;
        // void update(float alpha);
public:
        virtual void OnWindowResize(int w, int h) {};
        virtual void Init() {};
        virtual void Update() = 0;
        virtual void Render(float alpha) = 0;
        virtual void Clean() = 0;
};