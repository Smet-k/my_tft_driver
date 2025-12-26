#include "draw.h"
#include "color.h"
#include "image.h"

#define PROGRESSION 10

void draw_gradient(tft_handle_t* tft, uint8_t width, uint8_t height) {
    color col = {.r = 0xFF, .g = 0x0, .b = 0x0};
    
    tft_send_cmd(tft, 0x2C);
    for(int y = 0; y < height; y++){
        tft_draw_row(tft, rgb_to_rgb565(col), width);

        if(col.r == 255 && col.g < 255){
            col.g = (col.g <= 255 - PROGRESSION) ? col.g + PROGRESSION : 255;
        }
        else if(col.g == 255 && col.r > 0){
            col.r = (col.r >= PROGRESSION) ? col.r - PROGRESSION : 0;
        }
        else if(col.g == 255 && col.r == 0 && col.b < 255){
            col.b = (col.b <= 255 - PROGRESSION) ? col.b + PROGRESSION : 255;
        }
        else if(col.b == 255 && col.g > 0){
            col.g = (col.g >= PROGRESSION) ? col.g - PROGRESSION : 0;
        }
        else if(col.b == 255 && col.g == 0 && col.r < 255){
            col.r = (col.r <= 255 - PROGRESSION) ? col.r + PROGRESSION : 255;
        }
    }
}

void draw_image(tft_handle_t* tft, uint8_t width, uint8_t height){
    tft_send_cmd(tft, 0x2C);

    size_t pixels_per_row = width;
    size_t bytes_per_row = pixels_per_row * 2;
    uint8_t row_buffer[bytes_per_row];

    for (int y = 0; y < height; y++) {
    convert_rgb565_to_bytes(&image_data[y * pixels_per_row], row_buffer, pixels_per_row);
    
    tft_send_data(tft, row_buffer, bytes_per_row);
    }
}