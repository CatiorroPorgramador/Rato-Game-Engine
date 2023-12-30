@echo off

g++ -c include/Engine/Engine.cpp -o libs/Engine.o -Iinclude -Llibs libs/imgui.o libs/imgui_draw.o libs/imgui_impl_sdl2.o libs/imgui_impl_sdlrenderer2.o libs/imgui_tables.o libs/imgui_widgets.o -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -llua54
g++ -Iinclude -Iinclude/Engine -Llibs libs/imgui.o libs/imgui_draw.o libs/imgui_impl_sdl2.o libs/imgui_impl_sdlrenderer2.o libs/imgui_tables.o libs/imgui_widgets.o libs/Engine.o -o bin/Main src/Main.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -llua54