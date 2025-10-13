#pragma once
#ifndef TOUCH_H
#define TOUCH_H

#include <stdint.h>
#include <stdbool.h>
#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_attr.h"

#ifdef __cplusplus
extern "C" {
#endif

void ft6236u_init(void);
esp_err_t ft6236u_read_bytes(uint8_t start_reg, uint8_t *data, size_t len);
void ft6236u_get_coordinates(uint16_t *x, uint16_t *y);
void checkTouch(uint16_t *x, uint16_t *y);

#ifdef __cplusplus
}
#endif

#endif
