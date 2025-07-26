#include "Window.h"

#include "main.h"
#include "utils.h"

Window::Window(HINSTANCE hInstance, int width, int height, const wchar_t* title)
    : hInstance(hInstance), width(width), height(height), title(title), window(nullptr), input({}) {}

Window::~Window() {}

LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    Window* pThis = nullptr;

    if (uMsg == WM_NCCREATE) {
        CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
        pThis = (Window*)pCreate->lpCreateParams;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
        pThis->window = hwnd;
    } else {
        pThis = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    }

    if (pThis) {
        return pThis->HandleMessage(hwnd, uMsg, wParam, lParam);
    }
    
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT Window::HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;
    
    switch (uMsg)
    {
        case WM_SIZE: {
            RECT rect;
            GetClientRect(hwnd, &rect);
            render_state.width = rect.right - rect.left;
            render_state.height = rect.bottom - rect.top;

            int size = render_state.width * render_state.height * sizeof(u32);

            if (render_state.memory) VirtualFree(render_state.memory, 0, MEM_RELEASE);
            render_state.memory = VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

            render_state.bitmap_info.bmiHeader.biSize = sizeof(render_state.bitmap_info.bmiHeader);
            render_state.bitmap_info.bmiHeader.biWidth = render_state.width;
            render_state.bitmap_info.bmiHeader.biHeight = render_state.height;
            render_state.bitmap_info.bmiHeader.biPlanes = 1;
            render_state.bitmap_info.bmiHeader.biBitCount = 32;
            render_state.bitmap_info.bmiHeader.biCompression = BI_RGB;
        } break;
        case WM_CLOSE:
        case WM_DESTROY: {
            stop_running();
        } break;
        default:
            result = DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return result;
}

void Window::MessageLoop()
{
    MSG msg;
    
    while (PeekMessage(&msg, window, 0, 0, PM_REMOVE))
    {
        switch (msg.message) {
            case WM_KEYUP:
            case WM_KEYDOWN: {
                u32 vk_code = (u32)msg.wParam;
                bool is_down = ((msg.lParam & (1 << 31)) == 0);

                #define process_button(b, vk) \
                case (vk): { \
                    input.buttons[b].changed = is_down != input.buttons[b].is_down; \
                    input.buttons[b].is_down = is_down; \
                } break
                    
                switch (vk_code) {
                    process_button(BUTTON_F, 'F');
                    process_button(BUTTON_G, 'G');
                    process_button(BUTTON_H, 'H');
                    process_button(BUTTON_J, 'J');
                }
            } break;
            default: {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }
}

bool Window::Initialize()
{
    WNDCLASS wc = {};
    wc.lpfnWndProc = this->WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"qwru0905_rhythmgame";

    RegisterClass(&wc);

    window = CreateWindowEx(
        0,
        L"qwru0905_rhythmgame",
        title,
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        nullptr, nullptr, hInstance, this
    );

    if (!window) return false;
    hdc = GetDC(window);

    return true;
}

void Window::Render()
{
    if (render_state.memory) {
        StretchDIBits(hdc, 0, 0, render_state.width, render_state.height,
                      0, 0, render_state.width, render_state.height,
                      render_state.memory, &render_state.bitmap_info,
                      DIB_RGB_COLORS, SRCCOPY);
    }
}

void Window::ReSize(int width, int height)
{
    this->width = width;
    this->height = height;

    RECT rect;
    GetClientRect(window, &rect);
    rect.right = width;
    rect.bottom = height;

    AdjustWindowRect(&rect, GetWindowLong(window, GWL_STYLE), FALSE);
    
    SetWindowPos(window, nullptr, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER | SWP_NOMOVE);
}

void Window::RePos(int x, int y)
{
    SetWindowPos(window, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void Window::clear_screen(u32 color)
{
    u32* pixel = (u32*)render_state.memory;
    for (int y = 0; y < render_state.height; y++) {
        for (int x = 0; x < render_state.width; x++) {
            *pixel++ = color;
        }
    }
}


void Window::draw_rect(int x0, int y0, int x1, int y1, u32 color)
{
    x0 = clamp(0, x0, render_state.width);
    y0 = clamp(0, y0, render_state.height);
    x1 = clamp(0, x1, render_state.width);
    y1 = clamp(0, y1, render_state.height);
    for (int y = y0; y < y1; y++) {
        u32* pixel = (u32*)render_state.memory + x0 + y*render_state.width;
        for (int x = x0; x < x1; x++) {
            *pixel++ = color;
        }
    }
}