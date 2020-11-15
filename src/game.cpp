#include "core/game.hpp"
#include <algorithm>
#include <SDL.h>
void Game::Update()
{
        update_queue();
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
                switch (event.type)
                {
                case SDL_QUIT:
                        std::cout << "game closed" << std::endl;
                        Game::running = false;
                        break;

                case SDL_WINDOWEVENT_SIZE_CHANGED:
                        scenes.back()->OnWindowResize(
                                event.window.data1,
                                event.window.data2);
                        break;

                case SDL_MOUSEMOTION:
                        mouse_coords = {event.motion.x, event.motion.y};
                        break;
                }
        }
        scenes.back()->Update();
        update_queue();
}

void Game::Render(float alpha)
{
        scenes.back()->Render(alpha);
}

void Game::PushScene(Scene* scene)
{
        add_queue.emplace_back(scene);
}

void Game::PopScene(Scene* scene)
{
        remove_queue.emplace_back(scene);
}

void Game::update_queue()
{
        for (Scene* scene : remove_queue)
                pop_scene(scene);
        for (Scene* scene : add_queue)
                push_scene(scene);
        remove_queue.clear();
        add_queue.clear();
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
                (*it)->Clean();
                //delete the object the pointer is pointing to
                delete (*it);
                //remove the pointer from our list
                scenes.erase(it);
        }
}

void Game::RemoveWidget(KW_Widget* widget)
{
        widgets_to_delete.emplace_back(widget);
}

void Game::FlushWidgets()
{
        for (KW_Widget* w : widgets_to_delete)
        {
                KW_Rect geom = {-99999, -99999, 0, 0};
                KW_SetWidgetGeometry(w, &geom);
                // KW_DestroyWidget(w, 1);
        }
        widgets_to_delete.clear();
}