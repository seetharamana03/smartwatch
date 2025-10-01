#include "touch.h"

TouchManager::TouchManager(int sda, int scl, int rst, int irq) 
    : ts(sda, scl, rst, irq), _x(0), _y(0), _touched(false) {}

bool TouchManager::touched() {
    if (ts.available()) {
        _touched = true;
        _x = ts.data.x;
        _y = ts.data.y;
        return true;
    }
    return false;
}

void TouchManager::getPoint(int &x, int &y) {
    if (_touched) {
        x = _x;
        y = _y;
        _touched = false;   // reset until next read
    } else {
        x = -1;
        y = -1;
    }
}

void TouchManager::begin() {
    ts.begin(FALLING);
    Serial.println("Touchscreen initialized.");
}