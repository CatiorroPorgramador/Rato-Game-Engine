#include "Game.hpp"

#include <ctime>

int main(int argc, char** argv) {
    SDL_Window* window = SDL_CreateWindow("window_name", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Engine::Init(window, renderer);

    Engine::Scene *default_scene = new Engine::GamePlay();
    default_scene->Name = "GamePlay";
    default_scene->Init();
    
    // Game Stuff
    bool running = true;
    Uint32 last_time = SDL_GetPerformanceCounter();
    Uint32 current_time, frame_time;
    float delta_time = 0.0f;
    float wait_time = 32.f;
    float fps = 0.0f;

    SDL_Event event;

    clock_t start = clock();

    while(running){
        current_time = SDL_GetPerformanceCounter();
        frame_time = current_time - last_time;

        delta_time = frame_time / (float) SDL_GetPerformanceFrequency();
        last_time = current_time;

        fps = 10000000.0f / static_cast<float>(frame_time);

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

        SDL_SetRenderDrawColor(renderer, 60, 255, 60, 255);
        SDL_RenderClear(renderer);

        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        default_scene->Render();

        ImGui::Begin("SDL2 com Dear ImGui");
        ImGui::SeparatorText("Framerate");
        ImGui::InputFloat("Wait Time", &wait_time);
        ImGui::Text("%d Fps", static_cast<int>(fps));
        ImGui::SeparatorText("Scene");
        ImGui::Text("Current Scene: %s", default_scene->Name.c_str());
        ImGui::Checkbox("Pause Scene", &default_scene->Pause);
        ImGui::End();

        ImGui::Render();

        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());

        // Debbuug
        SDL_RenderPresent(renderer);
        SDL_Delay(wait_time);
    }

    SDL_DestroyWindow(window);

    delete default_scene;

    Engine::End();

    clock_t end = clock();
    std::cout << (end - start)/CLOCKS_PER_SEC << " Seconds.\n";

    return 0;
}