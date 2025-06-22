// renderer.h
#pragma once
#include "utils.h"

void clear_screen(u32 color);
void draw_rect_in_pixels(int x0, int y0, int x1, int y1, u32 color);
void draw_rect(float x, float y, float half_size_x, float half_size_y, u32 color);
