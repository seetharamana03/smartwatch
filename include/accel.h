#pragma once
#include <Arduino.h>
#include <SensorBMA423.hpp>

// Initializes the accelerometer and step counter
void accel_init();

// Returns the current step count (uint32_t)
uint32_t accel_get_steps();

// Optional: read raw accelerometer data (X, Y, Z)
void accel_read_raw(int16_t *x, int16_t *y, int16_t *z);