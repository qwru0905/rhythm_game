#pragma once

#include <windows.h>
#include "input.h"
#include "utils.h"

struct Render_State {
    int height, width;
    void* memory;
    BITMAPINFO bitmap_info;
};

class Window
{
    public:
        void MessageLoop();
        bool Initialize();
        void Render();
        void ReSize(int width, int height);
        void RePos(int x, int y);
        Window(HINSTANCE hInstance, int width, int height, const wchar_t* title);
        ~Window();
        Input input;
        Render_State render_state;
        void clear_screen(u32 color);
        void draw_rect(int x0, int y0, int x1, int y1, u32 color);


    private:
        static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        LRESULT HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

        HINSTANCE hInstance;
        HWND window;
        HDC hdc;
        int width;
        int height;
        const wchar_t* title;
};
