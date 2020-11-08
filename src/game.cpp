#include "core/game.hpp"
#include <algorithm>

void Game::Update()
{
        scenes.back()->Update();
}

void Game::push_scene(Scene* scene)
{
        //add the scene to the list
        scenes.emplace_back(scene);
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