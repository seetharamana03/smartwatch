#include "touch.h"
#include "driver/i2c.h"
#include "esp_err.h"
#include <string.h>
#include <stdio.h>

#define FT6236U_ADDR 0x38 // I2C address of FT6236U
#define FT_INT_PIN 38
#define I2C_TOUCH_PORT I2C_NUM_1

static volatile bool touch_flag = false;

static void IRAM_ATTR ft6236u_isr(void *arg)
{
    touch_flag = true;
}

void ft6236u_init(void)
{
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = 23,
        .scl_io_num = 32,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 100000,
    };

    i2c_param_config(I2C_TOUCH_PORT, &conf);
    // i2c_driver_install(I2C_NUM_0, conf.mode, 0, 0, 0);
    i2c_driver_install(I2C_TOUCH_PORT, conf.mode, 0, 0, 0);


    // Reset the FT6236U
    gpio_set_direction(14, GPIO_MODE_OUTPUT);
    gpio_set_level(14, 0);
    vTaskDelay(pdMS_TO_TICKS(10));
    gpio_set_level(14, 1);
    vTaskDelay(pdMS_TO_TICKS(50));

    // Configure interrupt pin
    gpio_config_t io_conf = {
        .pin_bit_mask = 1ULL << FT_INT_PIN,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_NEGEDGE,
    };

    gpio_config(&io_conf);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(FT_INT_PIN, ft6236u_isr, NULL);

    uint8_t id = 0;

    // for (int i = 0; i < 5; i++)
    // {
    //     esp_err_t ret = ft6236u_read_bytes(0xA8, &id, 1);
    //     printf("FT6236U ID = 0x%02X (%s)\n", id, esp_err_to_name(ret));
    //     vTaskDelay(pdMS_TO_TICKS(50));
    // }
}

esp_err_t ft6236u_read_bytes(uint8_t start_reg, uint8_t *data, size_t len)
{
    esp_err_t ret = i2c_master_write_to_device(
        I2C_TOUCH_PORT,
        FT6236U_ADDR,
        &start_reg,
        1,
        pdMS_TO_TICKS(1000));

    if (ret != ESP_OK)
    {
        printf("I2C write error %s\n", esp_err_to_name(ret));
    }

    ret = i2c_master_read_from_device(I2C_TOUCH_PORT, FT6236U_ADDR, data, len, pdMS_TO_TICKS(1000));
    if (ret != ESP_OK) {
        printf("I2C read error %s\n", esp_err_to_name(ret));
    }

    return ret;
}

void ft6236u_get_coordinates(uint16_t *x, uint16_t *y)
{
    uint8_t buf[6];

    if (ft6236u_read_bytes(0x02, buf, 6) != ESP_OK)
    {
        *x = 0;
        *y = 0;
        return;
    }

    uint8_t touches = buf[0] & 0x0F;
    if (touches == 0)
    {
        *x = 0;
        *y = 0;
        return;
    }

    *x = ((buf[1] & 0x0F) << 8) | buf[2];
    *y = ((buf[3] & 0x0F) << 8) | buf[4];
}

void checkTouch(uint16_t *x, uint16_t *y)
{
    if (touch_flag)
    {
        touch_flag = false;
        ft6236u_get_coordinates(x, y);
        printf("Touch at (%d, %d)\n", *x, *y);
    }
}