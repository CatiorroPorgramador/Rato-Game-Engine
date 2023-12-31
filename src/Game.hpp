#pragma once

#include "Engine.h"

namespace Engine {
    constexpr Uint8 BeginnersSound = 0;

    class GamePlay : public Engine::Scene {
        // Enviroment
        LuaComponent player;

    public:
        GamePlay() {

        }
        ~GamePlay() {}

        void Init() {
            // Game Environment
            Engine::Musics[0] = Mix_LoadMUS("data/sounds/Beginner's Sound.mp3");

            player.LoadScript("data/scripts/Component.lua");
            player.ScriptInit();

            // ...
            Mix_PlayMusic(Engine::Musics[BeginnersSound], -1);
        }

        void InputDown(SDL_Keycode key_code) {
            
        }

        void InputUp(SDL_Keycode action_up) {
                        
        }

        virtual void MouseMotion(SDL_MouseButtonEvent mouse) {
            
        }

        virtual void MouseDown(SDL_MouseButtonEvent mouse) {

        }

        virtual void MouseUp(SDL_MouseButtonEvent mouse) {

        }

        void Update(float delta_time) {
            player.ScriptUpdate(delta_time);
        }

        void Render() {
            SDL_SetRenderDrawColor(Engine::Renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(Engine::Renderer, &player.Rect);
        }
    };

}