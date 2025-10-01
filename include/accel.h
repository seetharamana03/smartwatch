#pragma once
#include <Arduino.h>

#define BMA423_ADDR 0x19 //confirmed by scanner
#define BMA423_X_LSB 0x12
#define BMA423_X_MSB 0x13
#define BMA423_Y_LSB 0x14
#define BMA423_Y_MSB 0x15
#define BMA423_Z_LSB 0x16
#define BMA423_Z_MSB 0x17
#define BMA423_CHIP_ID_REG 0x00
#define BMA423_CHIP_ID 0x13 //confirmed by datasheet

class Accelerometer{
  public:
    Accelerometer();
    void printAccelerometer();
    void initAccelerometer();
    void readAccelerometer();
  
  private:
    static int16_t readAxis(uint8_t lsbReg, uint8_t msbReg);
    int16_t x;
    int16_t y;
    int16_t z;
};
