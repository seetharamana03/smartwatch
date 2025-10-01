#pragma once
#include <AXP20X.h>

class PowerManager {
    public:
        void initButton();
        bool checkButtonPress();
    private:
        static void IRAM_ATTR axpISR();
        static volatile bool axpPressed;
        AXP20X_Class power;
};