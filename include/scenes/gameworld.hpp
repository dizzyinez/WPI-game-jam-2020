#pragma once
#include "core/Scene.hpp"
#include <SDL.h>
#include <SDL_image.h>

class GameWorld : public Scene
{
public:
        SDL_Texture* texture;
        void Init()
        {
                SDL_Surface* surface = IMG_Load("assets/test.png");
                texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
                SDL_FreeSurface(surface);
        }
        void Update() {}
        void Render(float alpha)
        {
                SDL_Rect dest = {0, 0, 500, 500};
                SDL_RenderCopy(Game::renderer, texture, NULL, NULL);
        }
};