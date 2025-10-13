#pragma once

#include "esp_err.h"
#include <stddef.h>
#include <stdint.h>
#include <time.h>

#ifndef MYTIME_H
#define MYTIME_H

#ifdef __cplusplus
extern "C"
{
#endif

void syncTimeWithNTP();
void pcf8563_set_time(struct tm *timeinfo);
void pcf8563_get_time(struct tm *timeinfo);

uint8_t decToBcd(uint8_t val);
uint8_t bcdToDec(uint8_t val);

void mytime_init(void);
esp_err_t pcf8563_read_bytes(uint8_t start_reg, uint8_t *data, size_t len);
esp_err_t pcf8563_write_bytes(uint8_t start_reg, uint8_t *data, size_t len);

#ifdef __cplusplus
}
#endif

#endif



