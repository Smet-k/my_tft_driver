#ifndef TFT_H
#define TFT_H
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

typedef struct {
    spi_host_device_t spi_host;
    int clock_hz;
    gpio_num_t gpio_dc;
    gpio_num_t gpio_reset;
    gpio_num_t gpio_cs;
} tft_config_t;

typedef struct {
    spi_device_handle_t spi;
    gpio_num_t gpio_dc;
    gpio_num_t gpio_reset;
} tft_handle_t;

esp_err_t tft_init(tft_handle_t *tft, const tft_config_t *cfg);
void tft_start(tft_handle_t* tft);

void tft_send_cmd(tft_handle_t* tft, uint8_t cmd);
void tft_send_data(tft_handle_t* tft, const uint8_t *data, size_t len);

void tft_set_full_window(tft_handle_t* tft, uint16_t width, uint16_t height);
void tft_fill(tft_handle_t *tft, uint16_t color, uint16_t width, uint16_t height);
void tft_draw_row(tft_handle_t* tft, uint16_t color, uint16_t width);

#endif