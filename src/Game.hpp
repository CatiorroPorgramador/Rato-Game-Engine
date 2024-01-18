#pragma once

#include "Engine.h"

namespace Engine {
    constexpr Uint8 BeginnersSound = 0;

    // Components
    class Player : public LuaComponent {
    public:
        Engine::AnimationManager *animation_manager;

        Player() {
            animation_manager = new Engine::AnimationManager(&this->SrcRect);
            animation_manager->CreateAnimation("Vorta", {0, 1, 2, 3, 4});
            animation_manager->Loop = true;
            animation_manager->Play("Vorta");
            animation_manager->SetAnimationSpeed(1.0f);
        }

        ~Player() {
            delete animation_manager;
        }

        void Init() override {
            this->LoadScript("data/scripts/Component.lua");
            this->ScriptInit();
        }

        void Update(float delta_time) override {
            this->animation_manager->Update(delta_time);
            this->ScriptUpdate(delta_time);
        }
    };

    // Scenes
    class GamePlay : public Engine::Scene {
        // Enviroment
        Group player_group, test_group;

        Player player;

        GameComponent test;

    public:
        GamePlay() {
            this->Pause = false;
            player.Init();

            player_group.Components.push_back(&player);
            test_group.Components.push_back(&test);
        }
        ~GamePlay() {}

        void Init() override {
            // Game Environment
            Engine::Musics.push_back(Mix_LoadMUS("data/sounds/Beginner's Sound.mp3"));
            Mix_VolumeMusic(2);

            test.Rect = SDL_Rect {400, 0, 50, 50};

            Engine::CurrentGroups["Player"] = player_group;
            Engine::CurrentGroups["Test"] = test_group;

            // ...
            Mix_PlayMusic(Engine::Musics[BeginnersSound], -1);
        }

        void InputDown(SDL_Keycode key_code) override {
            
        }

        void InputUp(SDL_Keycode action_up) override {
            
        }

        virtual void MouseMotion(SDL_MouseButtonEvent mouse) override {
            
        }

        virtual void MouseDown(SDL_MouseButtonEvent mouse) override {

        }

        virtual void MouseUp(SDL_MouseButtonEvent mouse) override {

        }

        void Update(float delta_time) override {
            if (!this->Pause) {
            player_group.Update(delta_time);
            test_group.Update(delta_time);
            }

        }

        void Render() override {
            player_group.Render();
            test_group.Render();
        }
    };

}