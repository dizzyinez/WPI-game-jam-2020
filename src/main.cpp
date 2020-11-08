#include <common.hpp>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include "core/game.hpp"
#include "scenes/gameworld.hpp"
constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;
constexpr int UPDATE_RATE = 60;
constexpr double SECONDS_PER_UPDATE = 1.0f / (double)UPDATE_RATE;
constexpr int MAX_FPS = 200;
constexpr double MIN_FRAME_TIME = 1.0f / (double)MAX_FPS;

int main(int argc, char *argv[])
{
        SDL_Window* window = NULL;
        SDL_Surface* screenSurface = NULL;

        //Initialize SDL
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
                printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        }
        else
        {
                //Create window
                window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
                if (window == NULL)
                {
                        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
                }
                else
                {
                        //Get window surface
                        screenSurface = SDL_GetWindowSurface(window);

                        //Fill the surface white
                        SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));

                        //Update the surface
                        SDL_UpdateWindowSurface(window);
                        Game::push_scene(new GameWorld);

                        Game::Update();
                        //Wait two seconds
                        SDL_Delay(2000);
                }
        }
        //Destroy window
        SDL_DestroyWindow(window);

        //Quit SDL subsystems
        SDL_Quit();

        return 0;
}