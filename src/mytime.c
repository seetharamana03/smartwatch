#include "mytime.h"
#include "driver/i2c.h"
#include "esp_err.h"
#include <string.h>

#define I2C_MASTER_SCL 22
#define I2C_MASTER_SDA 21
#define I2C_MASTER_PORT I2C_NUM_0
#define I2C_MASTER_FREQ_HZ 100000
#define PCF8563_ADDR 0x51 // I²C address of PCF8563

void mytime_init(void)
{
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA,
        .scl_io_num = I2C_MASTER_SCL,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };

    i2c_param_config(I2C_MASTER_PORT, &conf);
    // i2c_driver_install(I2C_MASTER_PORT, conf.mode, 0, 0, 0);
}

uint8_t decToBcd(uint8_t val)
{
    return (val / 10 * 16) + (val % 10);
}

uint8_t bcdToDec(uint8_t val)
{
    return (val / 16 * 10) + (val % 16);
}

esp_err_t pcf8563_write_bytes(uint8_t start_reg, uint8_t *data, size_t len)
{
    uint8_t buf[len + 1];
    buf[0] = start_reg;
    memcpy(&buf[1], data, len);
    return i2c_master_write_to_device(
        I2C_MASTER_PORT,
        PCF8563_ADDR,
        buf,
        len + 1,
        pdMS_TO_TICKS(1000));
}

esp_err_t pcf8563_read_bytes(uint8_t start_reg, uint8_t *data, size_t len)
{
    esp_err_t ret = i2c_master_write_to_device(I2C_MASTER_PORT, PCF8563_ADDR, &start_reg, 1, pdMS_TO_TICKS(1000));
    if (ret != ESP_OK)
    {
        printf("I2C read error %s\n", esp_err_to_name(ret));
    }
    return i2c_master_read_from_device(I2C_MASTER_PORT, PCF8563_ADDR, data, len, pdMS_TO_TICKS(1000));
}

void pcf8563_set_time(struct tm *timeinfo)
{
    uint8_t data[7];
    data[0] = decToBcd(timeinfo->tm_sec) & 0x7F;
    data[1] = decToBcd(timeinfo->tm_min) & 0x7F;
    data[2] = decToBcd(timeinfo->tm_hour) & 0x3F;
    data[3] = decToBcd(timeinfo->tm_mday) & 0x3F;
    data[4] = decToBcd(timeinfo->tm_wday) & 0x07;
    data[5] = decToBcd(timeinfo->tm_mon + 1) & 0x1F;
    data[6] = decToBcd(timeinfo->tm_year - 100); // years since 2000
    pcf8563_write_bytes(0x02, data, 7);
}

void pcf8563_get_time(struct tm *timeinfo)
{
    uint8_t data[7];
    pcf8563_read_bytes(0x02, data, 7);

    timeinfo->tm_sec = bcdToDec(data[0] & 0x7F);
    timeinfo->tm_min = bcdToDec(data[1] & 0x7F);
    timeinfo->tm_hour = bcdToDec(data[2] & 0x3F);
    timeinfo->tm_mday = bcdToDec(data[3] & 0x3F);
    timeinfo->tm_wday = bcdToDec(data[4] & 0x07);
    timeinfo->tm_mon = bcdToDec(data[5] & 0x1F) - 1;
    timeinfo->tm_year = bcdToDec(data[6]) + 100;
}

void syncTimeWithNTP()
{
    configTzTime("EST5EDT,M3.2.0,M11.1.0", "pool.ntp.org");
}