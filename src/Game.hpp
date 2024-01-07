#pragma once

#include "Engine.h"

namespace Engine {
    constexpr Uint8 BeginnersSound = 0;

    class GamePlay : public Engine::Scene {
        // Enviroment
        Group player_group, test_group;

        LuaComponent player;

        GameComponent test;

    public:
        GamePlay() {
            player_group.Components.push_back(&player);
            test_group.Components.push_back(&test);
        }
        ~GamePlay() {}

        void Init() {
            // Game Environment
            Engine::Musics.push_back(Mix_LoadMUS("data/sounds/Beginner's Sound.mp3"));
            Mix_VolumeMusic(2);

            player.LoadScript("data/scripts/Component.lua");
            player.ScriptInit();

            test.Rect = SDL_Rect {400, 0, 50, 50};

            Engine::CurrentGroups["Player"] = player_group;
            Engine::CurrentGroups["Test"] = test_group;

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
            player_group.Update();
            test_group.Update();

            player.ScriptUpdate(delta_time);
        }

        void Render() {
            player_group.Render();
            test_group.Render();
        }
    };

}