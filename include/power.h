#pragma once
#include <AXP20X.h>

class Power
{
    public: 
        void initPower();
        void togglePower();
        int getBatteryPercentage();
    private:
        AXP20X_Class axp;
        static void IRAM_ATTR axpISR();
        static volatile bool axpPressed;
        bool powerOn;
};