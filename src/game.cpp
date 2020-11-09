#include "core/game.hpp"
#include <algorithm>
#include <SDL.h>
void Game::Update()
{
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
                switch (event.type)
                {
                case SDL_QUIT:
                        std::cout << "game closed" << std::endl;
                        Game::running = false;
                        break;

                        // case SDL_WINDOWEVENT:
                        //         break;
                }
        }
        scenes.back()->Update();
}

void Game::Render(float alpha)
{
        scenes.back()->Render(alpha);
}

void Game::push_scene(Scene* scene)
{
        //add the scene to the list
        scenes.emplace_back(scene);
        scene->Init();
}
void Game::pop_scene(Scene* scene)
{
        //search for the scene we want to delete
        auto it = std::find(scenes.begin(), scenes.end(), scene);

        //if it exists within our list, delete it!
        if (it != scenes.end())
        {
                //delete the object the pointer is pointing to
                delete *it;
                //remove the pointer from our list
                scenes.erase(it);
        }
}