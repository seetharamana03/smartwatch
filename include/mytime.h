#pragma once
#include <time.h>
#include <cmath>
#include <Arduino.h>

class MyTime {
public:
    MyTime();
    void init();
    String getTimeString();
    String getDateString();
private:
    struct tm timeinfo;
};