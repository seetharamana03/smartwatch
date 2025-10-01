#include "accel.h"
#include <Wire.h>

Accelerometer::Accelerometer()
  : x(0), y(0), z(0) {}

void Accelerometer::printAccelerometer(){
  float x_g = x / 1024.0;
  float y_g = y / 1024.0;
  float z_g = z / 1024.0;
  Serial.print("X Axis: ");
  Serial.println(x_g);
  Serial.print("Y Axis: ");
  Serial.println(y_g);
  Serial.print("Z Axis: ");
  Serial.println(z_g);
}

void Accelerometer::initAccelerometer(){
  Wire.begin();
  // Check chip ID
  uint8_t id;
  Wire.beginTransmission(BMA423_ADDR);
  Wire.write(BMA423_CHIP_ID_REG);
  Wire.endTransmission(false);
  Wire.requestFrom(BMA423_ADDR, (uint8_t)1);
  id = Wire.read();

  Serial.print("BMA423 ID: 0x");
  Serial.println(id, HEX);

  if (id != BMA423_CHIP_ID) {
      Serial.println("BMA423 not detected!");
      return;
  }

  // --- Power up accelerometer ---
  Wire.beginTransmission(BMA423_ADDR);
  Wire.write(0x7D);    // Power Control
  Wire.write(0x04);    // Accelerometer enable
  Wire.endTransmission();

  // --- Configure accelerometer: 100 Hz, OSR2 filter ---
  Wire.beginTransmission(BMA423_ADDR);
  Wire.write(0x40);    // Accel config
  Wire.write(0xAC);    // 100 Hz, normal filter
  Wire.endTransmission();

  // --- Set range to ±2g ---
  Wire.beginTransmission(BMA423_ADDR);
  Wire.write(0x41);    // Accel range
  Wire.write(0x00);    // ±2g
  Wire.endTransmission();

  Serial.println("BMA423 initialized");
}

int16_t Accelerometer::readAxis(uint8_t lsbReg, uint8_t msbReg) {
  Wire.beginTransmission(BMA423_ADDR);
  Wire.write(lsbReg);
  Wire.endTransmission(false);
  Wire.requestFrom(BMA423_ADDR, (uint8_t)2);

  uint8_t lsb = Wire.read();
  uint8_t msb = Wire.read();

  int16_t new_data = ((int16_t)msb << 8) | lsb;
  new_data = new_data >> 4;
  return new_data;
}

void Accelerometer::readAccelerometer() {
  x = readAxis(BMA423_X_LSB, BMA423_X_MSB);
  y = readAxis(BMA423_Y_LSB, BMA423_Y_MSB);
  z = readAxis(BMA423_Z_LSB, BMA423_Z_MSB);
}