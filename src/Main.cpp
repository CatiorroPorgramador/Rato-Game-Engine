#include "Game.hpp"

#include <ctime>
#include <string>

int main(int argc, char** argv) {
    SDL_Window* window = SDL_CreateWindow("window_name", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Engine::Init(window, renderer, 1);

    Engine::Scene *default_scene = static_cast<Engine::GamePlay*>(std::malloc(sizeof(Engine::GamePlay)));
    new (default_scene) Engine::GamePlay();

    default_scene->Init();
    
    // Game Stuff
    bool running = true;
    Uint32 last_time = SDL_GetPerformanceCounter();
    Uint32 current_time, frame_time;
    float delta_time = 0.0f;
    float fps = 0.0f;
    SDL_Event event;

    clock_t start = clock();

    while(running){
        current_time = SDL_GetPerformanceCounter();
        frame_time = current_time - last_time;

        delta_time = frame_time / (float) SDL_GetPerformanceFrequency();
        last_time = current_time;

        // Events
        while(SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            switch(event.type){
                case SDL_QUIT:
                    running = false;
                    break;
                
                case SDL_KEYDOWN:
                    default_scene->InputDown(event.key.keysym.sym);
                    break;
                case SDL_KEYUP:
                    default_scene->InputUp(event.key.keysym.sym);
                    break;

                case SDL_MOUSEMOTION:
                    default_scene->MouseMotion(event.button);
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    default_scene->MouseDown(event.button);
                    break;
                case SDL_MOUSEBUTTONUP:
                    default_scene->MouseUp(event.button);
                    break;
                default:
                    break;
            }
        }

        /* Update */

        default_scene->Update(delta_time);

        SDL_SetRenderDrawColor(renderer, 25, 25, 25, 255);
        SDL_RenderClear(renderer);
        
        default_scene->Render();

        // ImGui_ImplSDLRenderer2_NewFrame();
        // ImGui_ImplSDL2_NewFrame();
        // ImGui::NewFrame();

        // ImGui::Begin("SDL2 com Dear ImGui");
        // ImGui::Text("Isso é só um Olá Mundo básico");
        // ImGui::Button("This Button");
        // ImGui::End();

        // ImGui::Render();

        // ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());

        // Debbuug
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyWindow(window);

    free(default_scene);

    Engine::End();

    clock_t end = clock();
    std::cout << (end - start)/CLOCKS_PER_SEC << " Seconds.\n";

    return 0;
}