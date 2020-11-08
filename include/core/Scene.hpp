#pragma once
#include "common.hpp"
#include "core/Entity.hpp"
#include <array>
class Scene
{
        //resizeable array of entities
        std::array<Entity, 1000> entities;
        // void update(float alpha);
public:
        virtual void Update() = 0;
        virtual void Render(float alpha) = 0;
};