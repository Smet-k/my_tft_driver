#ifndef COLOR_H
#define COLOR_h
#include <stdint.h>
#include <stdlib.h>

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} color;

uint16_t rgb_to_rgb565(color c);
void convert_rgb565_to_bytes(const uint16_t* input, uint8_t* output, size_t pixel_count);
#endif