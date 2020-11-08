#include <common.hpp>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include "core/game.hpp"
#include "scenes/gameworld.hpp"
constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;
constexpr int UPDATE_RATE = 60;
constexpr double SECONDS_PER_UPDATE = 1.0f / (double)UPDATE_RATE;
constexpr int MAX_FPS = 60;
constexpr double MIN_FRAME_TIME = 1.0f / (double)MAX_FPS;

int main(int argc, char *argv[])
{
        //Initialize SDL
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        {
                printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
                return -1;
        }
        if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
        {
                std::cout << "couldn't initialize SDL_IMAGE" << std::endl;
                return -1;
        }
        //Create window
        Game::window = SDL_CreateWindow("Train Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (Game::window == nullptr)
        {
                printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
                return -1;
        }

        //initialize renderer
        Game::renderer = SDL_CreateRenderer(Game::window, -1, SDL_RENDERER_ACCELERATED);
        if (Game::renderer == nullptr)
        {
                std::cout << "couldn't initialize renderer" << std::endl;
                return -1;
        }
        SDL_SetRenderDrawColor(Game::renderer, 100, 149, 237, 255);

        Game::push_scene(new GameWorld);

        double update_accumulator = 0.0;
        double render_accumulator = 0.0;
        Uint64 last_time = SDL_GetPerformanceCounter();
        while (Game::running)
        {
                Uint64 new_time = SDL_GetPerformanceCounter();
                double frame_time = (new_time - last_time) / (double)SDL_GetPerformanceFrequency();

                last_time = SDL_GetPerformanceCounter();
                update_accumulator += frame_time;
                render_accumulator += frame_time;

                while (update_accumulator >= SECONDS_PER_UPDATE)
                {
                        //handle events
                        Game::Update();
                        update_accumulator -= SECONDS_PER_UPDATE;
                }
                if (render_accumulator >= MIN_FRAME_TIME)
                {
                        SDL_RenderClear(Game::renderer);
                        Game::Render(update_accumulator / SECONDS_PER_UPDATE);
                        SDL_RenderPresent(Game::renderer);
                        while (render_accumulator >= MIN_FRAME_TIME)
                                render_accumulator -= MIN_FRAME_TIME;
                }
        }
        //Destroy Renderer
        SDL_DestroyRenderer(Game::renderer);
        //Destroy window
        SDL_DestroyWindow(Game::window);

        //Quit SDL subsystems
        SDL_Quit();

        return 0;
}