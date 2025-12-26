#include "tft.h"
#include "esp_check.h"

#define TAG "TFT"

#define CMD_SLPOUT 0x11
#define CMD_DISPOFF 0x28
#define CMD_DISPON 0x29
#define CMD_MADCTL 0x36
#define CMD_COLMOD 0x3A
#define CMD_CASET 0x2A
#define CMD_RASET 0x2B
#define CMD_RAMWR 0x2C

#define X_OFFSET  2
#define Y_OFFSET  1 

#define WAKE_TIME_MS 120

void tft_set_full_window(tft_handle_t* tft, uint16_t width, uint16_t height){
    uint16_t x0 = X_OFFSET;
    uint16_t y0 = Y_OFFSET;
    uint16_t x1 = X_OFFSET + width - 1;
    uint16_t y1 = Y_OFFSET + height - 1;
    
    uint8_t col[] = {
        x0 >> 8, x0 & 0xFF,
        x1 >> 8, x1 & 0xFF
    };

    uint8_t row[] = {
        y0 >> 8, y0 & 0xFF,
        y1 >> 8, y1 & 0xFF
    };

    tft_send_cmd(tft, CMD_CASET); 
    tft_send_data(tft, col, 4);

    tft_send_cmd(tft, CMD_RASET); 
    tft_send_data(tft, row, 4);
}

void tft_fill(tft_handle_t *tft, uint16_t color, uint16_t width, uint16_t height)
{
    uint8_t hi = color >> 8;
    uint8_t lo = color & 0xFF;

    tft_set_full_window(tft, width, height);

    uint8_t px_buf[width * 2];

    for (int i = 0;i < width; i++){
        px_buf[i*2] = hi;
        px_buf[i*2 + 1] = lo;
    }

    tft_send_cmd(tft, CMD_RAMWR); 

    for(int i = 0;i < height;i++){
        tft_send_data(tft, px_buf, width * 2);
    }
    
}

void tft_draw_row(tft_handle_t* tft, uint16_t color, uint16_t width){
    uint8_t hi = color >> 8;
    uint8_t lo = color & 0xFF;

    uint8_t px_buf[width * 2];
    for (int i = 0;i < width; i++){
        px_buf[i*2] = hi;
        px_buf[i*2 + 1] = lo;
    }

    tft_send_data(tft, px_buf, width * 2);
}

esp_err_t tft_init(tft_handle_t *tft, const tft_config_t *cfg){
    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = cfg->clock_hz,
        .mode = 0,
        .spics_io_num = cfg->gpio_cs,
        .queue_size = 7
    };

    ESP_RETURN_ON_ERROR(
        spi_bus_add_device(cfg->spi_host, &devcfg, &tft->spi),
        TAG, "SPI add device failed"
    );

    tft->gpio_dc = cfg->gpio_dc;
    tft->gpio_reset = cfg->gpio_reset;

    gpio_set_direction(tft->gpio_dc, GPIO_MODE_OUTPUT);
    gpio_set_direction(tft->gpio_reset, GPIO_MODE_OUTPUT);

    return ESP_OK;
}

void tft_start(tft_handle_t* tft){
    gpio_set_level(tft->gpio_reset, 0);
    vTaskDelay(pdMS_TO_TICKS(100));
    gpio_set_level(tft->gpio_reset, 1);
    vTaskDelay(pdMS_TO_TICKS(100));

    tft_send_cmd(tft, CMD_SLPOUT);
    vTaskDelay(pdMS_TO_TICKS(WAKE_TIME_MS));

    tft_send_cmd(tft, CMD_MADCTL);   
    uint8_t madctl = 0x08;
    tft_send_data(tft, &madctl, 1);

    tft_send_cmd(tft, CMD_COLMOD);   
    uint8_t mode = 0x5;        // RGB565
    tft_send_data(tft, &mode, 1);


    tft_send_cmd(tft, CMD_DISPON);
    vTaskDelay(pdMS_TO_TICKS(1000)); 

}

void tft_send_cmd(tft_handle_t* tft, uint8_t cmd){
    gpio_set_level(tft->gpio_dc, 0);

    spi_transaction_t t = {0};

    t.length = 8;
    t.tx_buffer = &cmd;

    ESP_ERROR_CHECK(spi_device_transmit(tft->spi, &t));
}

void tft_send_data(tft_handle_t* tft, const uint8_t *data, size_t len){
    gpio_set_level(tft->gpio_dc, 1);
    
    spi_transaction_t t = {0};
    t.length = len * 8;
    t.tx_buffer = data;

    ESP_ERROR_CHECK(spi_device_transmit(tft->spi, &t));
}

