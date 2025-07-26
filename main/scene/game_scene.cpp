#include "game_scene.h"
#include <iostream>

game_scene::game_scene(Window& window)
    : window(window), input(window.input) {}

void game_scene::Init()
{
    window.ReSize(500, 800);
    window.RePos(710, 100);
}

void game_scene::Update(float delta_time) {
    window.clear_screen(0xff5500);
    window.draw_rect(10, 30, 100, 100, 0x00ff00);
}
