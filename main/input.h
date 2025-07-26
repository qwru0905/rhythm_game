#pragma once

struct Button_State
{
    bool is_down;
    bool changed;
};

enum
{
    BUTTON_F,
    BUTTON_G,
    BUTTON_H,
    BUTTON_J,

    BUTTON_COUNT // Should be the last item
};

struct Input
{
    Button_State buttons[BUTTON_COUNT];
};
