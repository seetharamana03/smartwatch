#include "power.h"
#include <Wire.h>
#include <AXP20X.h>

// static volatile bool axpPressed = false;
// AXP20X_Class power;

volatile bool PowerManager::axpPressed = false;

void IRAM_ATTR PowerManager::axpISR()
{
    axpPressed = true;
}

void PowerManager::initButton()
{
    Wire.begin(21, 22);
    if (power.begin(Wire, AXP202_SLAVE_ADDRESS) == AXP_FAIL)
    {
        Serial.println("AXP202 init failed");
        while (1);
    }

    power.enableIRQ(AXP202_PEK_SHORTPRESS_IRQ, true);
    power.clearIRQ();

    pinMode(35, INPUT_PULLUP);
    attachInterrupt((digitalPinToInterrupt(35)), axpISR, FALLING);
}

bool PowerManager::checkButtonPress()
{
    if (axpPressed)
    {
        axpPressed = false;
        power.readIRQ();
        power.clearIRQ();
        return true;
    }
    return false;
}