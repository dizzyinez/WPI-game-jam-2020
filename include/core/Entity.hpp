#pragma once
#include "math/v2d.hpp"
#include <SDL.h>

struct Entity
{
        v2d position;
        v2d size;
        SDL_Texture* texture;
};