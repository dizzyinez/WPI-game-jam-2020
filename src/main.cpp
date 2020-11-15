#include <common.hpp>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_image.h>
#include "core/game.hpp"
#include "scenes/menu.hpp"
#include <KW_gui.h>
#include <KW_frame.h>
#include <KW_label.h>
#include <KW_renderdriver_sdl2.h>
#include <enet/enet.h>
#include <utils/upnp.hpp>
#include <SDL_mixer.h>

// #define IMGUI_IMPLEMENTATION
// #include <misc/single_file/imgui_single_file.h>
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

        //Initialize SDL_mixer
        if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
        {
                std::cout << "couldn't initialize SDL_AUDIO" << std::endl;
                return -1;
        }

        //Create window
        Game::window = SDL_CreateWindow("Locomotion Commotion", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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

        //initialize enet
        if (enet_initialize() != 0)
        {
                std::cout << "unable to initialize enet!" << std::endl;
        }

        // ImGui::CreateContext();
        // ImGuiIO& io = ImGui::GetIO(); (void)io;
        // ImGui::StyleColorsLight();
        // SDL_GLContext gl_context = SDL_GL_CreateContext(Game::window);
        // SDL_GL_MakeCurrent(Game::window, gl_context);
        // ImGui_ImplSDL2_InitForOpenGL(Game::window, gl_context);

        Game::kw_driver = KW_CreateSDL2RenderDriver(Game::renderer, Game::window);
        KW_Surface* set = KW_LoadSurface(Game::kw_driver, "assets/tileset.png");
        Game::gui = KW_Init(Game::kw_driver, set);

        Game::PushScene(new Menu);

        double update_accumulator = 0.0;
        double render_accumulator = 0.0;
        Uint64 last_time = SDL_GetPerformanceCounter();
        while (Game::running && !SDL_QuitRequested())
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
                        Game::FlushWidgets();
                        KW_ProcessEvents(Game::gui);

                        // ImGui_ImplSDL2_NewFrame(Game::window);
                        // ImGui_ImplOpenGL3_NewFrame();
                        // ImGui::NewFrame();

                        SDL_SetRenderDrawColor(Game::renderer, 100, 149, 237, 255);
                        SDL_RenderClear(Game::renderer);
                        Game::Render(update_accumulator / SECONDS_PER_UPDATE);
                        KW_Paint(Game::gui);
                        SDL_RenderPresent(Game::renderer);
                        while (render_accumulator >= MIN_FRAME_TIME)
                                render_accumulator -= MIN_FRAME_TIME;
                }
        }
        if (upnp::upnp_initialized)
                upnp::upnp_close(6743);

        std::cout << "game exited" << std::endl;
        KW_Quit(Game::gui);
        KW_ReleaseSurface(Game::kw_driver, set);
        KW_ReleaseRenderDriver(Game::kw_driver);

        //Destroy Renderer
        SDL_DestroyRenderer(Game::renderer);
        //Destroy window
        SDL_DestroyWindow(Game::window);

        //Quit SDL subsystems
        SDL_Quit();

        return 0;
}