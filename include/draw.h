#ifndef DRAW_H
#define DRAW_H
#include "tft.h"

void draw_gradient(tft_handle_t* tft, uint8_t width, uint8_t height);
void draw_image(tft_handle_t* tft, uint8_t width, uint8_t height);
#endif