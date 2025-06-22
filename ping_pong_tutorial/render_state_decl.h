#pragma once
#include <windows.h>

struct Render_State {
    int height, width;
    void* memory;
    BITMAPINFO bitmap_info;
};

extern Render_State render_state;
