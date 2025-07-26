#include "Window.h"
#include "CSound.h"
#include "scene/game_scene.h"
#include "main.h"

struct scene;
bool running = true;

void stop_running()
{
    running = false;
}


int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    CSound::Init();
    Window window(hInstance, 1280, 720, L"Rhythm Game");

    if (!window.Initialize()) {
        return -1;
    }

    float delta_time = 0.016666f; // Assuming 60 FPS for simplicity
    LARGE_INTEGER frame_begin_time;
    QueryPerformanceCounter(&frame_begin_time);

    float performance_frequency;
    {
        LARGE_INTEGER perf;
        QueryPerformanceFrequency(&perf);
        performance_frequency = (float)perf.QuadPart;
    }

    scene* current_scene = new game_scene(window);
    bool is_scene_changed = true;
    
    while (running) {
        // Input
        window.MessageLoop();

        if (is_scene_changed) {
            current_scene->Init();
            is_scene_changed = false;
        }

        // Simulate
        current_scene->Update(delta_time);
        
        // Render
        window.Render();

        LARGE_INTEGER frame_end_time;
        QueryPerformanceCounter(&frame_end_time);
        delta_time = (float)(frame_end_time.QuadPart - frame_begin_time.QuadPart) / performance_frequency;
        frame_begin_time = frame_end_time;
    }
    
    CSound::Release();
    
    return 0;
}
