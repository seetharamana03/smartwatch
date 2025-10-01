#pragma once
#include <time.h>
#include <cmath>
#include <Arduino.h>

class MyTime {
public:
    static void init();
    static String getTimeString();
};