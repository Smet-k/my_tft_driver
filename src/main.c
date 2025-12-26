#include "driver/spi_master.h"
#include "driver/spi_slave.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "tft.h"
#include "draw.h"

// TFT PARAMS
#define SCREEN_WIDTH 0x80
#define SCREEN_HEIGHT 0x80

// GPIO
#define GPIO_SCK GPIO_NUM_16
#define GPIO_SDA GPIO_NUM_17 // MOSI
#define GPIO_DC GPIO_NUM_18
#define GPIO_RESET GPIO_NUM_19
#define GPIO_CS GPIO_NUM_21

void app_main(void) {
    spi_bus_config_t buscfg = {
        .sclk_io_num=GPIO_SCK,
        .mosi_io_num=GPIO_SDA,
        .miso_io_num=-1,
        .max_transfer_sz=128 * 128 * 2
    };
    
    ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO));

    tft_handle_t tft;
    tft_config_t cfg = {
        .spi_host = SPI2_HOST,
        .clock_hz = 10 * 1000 * 1000,
        .gpio_dc = GPIO_DC,
        .gpio_cs = GPIO_CS,
        .gpio_reset = GPIO_RESET
    };

    tft_init(&tft, &cfg);

    tft_start(&tft);
    tft_fill(&tft, 0xFFFF, SCREEN_WIDTH, SCREEN_HEIGHT);
    vTaskDelay(pdMS_TO_TICKS(100));

    tft_set_full_window(&tft, SCREEN_WIDTH, SCREEN_HEIGHT);

    draw_gradient(&tft, SCREEN_WIDTH, SCREEN_HEIGHT);

    // draw_image(&tft, SCREEN_WIDTH, SCREEN_HEIGHT);
}
