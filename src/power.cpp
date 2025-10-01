#include "power.h"
#include <Wire.h>

volatile bool PowerManager::axpInt = false;

void IRAM_ATTR PowerManager::axpISR() {
    axpInt = true;
}

void PowerManager::initButton() {
    Wire.begin(21, 22);
    if (power.begin(Wire, AXP202_SLAVE_ADDRESS) ==  AXP_FAIL){
        Serial.println("AXP202 init failed");
        while(1);
    }
    power.enableIRQ(AXP202_PEK_SHORTPRESS_IRQ, true);
    power.clearIRQ();

    pinMode(35, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(35), axpISR, FALLING);
}

bool PowerManager::checkButtonPress(){
    if (axpInt) {
        axpInt = false;
        power.readIRQ();
        if(power.isPEKShortPressIRQ()) {
            power.clearIRQ();
            return true;
        }
        power.clearIRQ();
    }
    return false;
}