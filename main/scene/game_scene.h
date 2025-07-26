#pragma once

#include "scene.h"
#include "../Window.h"

struct game_scene : scene
{
    explicit game_scene(Window& window);

    void Init() override;
    
    // Game-specific data and methods can be added here
    void Update(float delta_time) override;

    private:
        Window &window;
        Input &input;
};
