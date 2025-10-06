#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <CST816S.h>

class TouchManager {
public:
    TouchManager(int sda=23, int scl=32, int rst=255, int irq=38);
    void begin();                // initialize touch
    bool touched();              // is screen touched?
    void getPoint(int &x, int &y); // retrieve coordinates

private:
    CST816S ts;
    bool _touched;
    int _x, _y;
};