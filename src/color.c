#include "color.h"
uint16_t rgb_to_rgb565(color c){
    uint8_t R5 = (c.r * 31) / 255;
    uint8_t G6 = (c.g * 63) / 255;
    uint8_t B5 = (c.b * 31) / 255;
    return (R5 << 11) | (G6 << 5) | B5;
}

void convert_rgb565_to_bytes(const uint16_t* input, uint8_t* output, size_t pixel_count) {
    for (size_t i = 0; i < pixel_count; i++) {
        output[i * 2]     = (uint8_t)(input[i] >> 8);   
        output[i * 2 + 1] = (uint8_t)(input[i] & 0xFF);  
    }
}