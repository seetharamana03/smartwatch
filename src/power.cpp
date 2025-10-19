#include "power.h"
#include <AXP20X.h>

volatile bool Power::axpPressed = false;

void IRAM_ATTR Power::axpISR()
{
    axpPressed = true;
}

void Power::initPower()
{
    Wire.begin(21, 22);
    if (axp.begin(Wire, AXP202_SLAVE_ADDRESS, false) == AXP_PASS)
    {
        Serial.println("AXP202 Init Success!");
    }
    else
    {
        Serial.println("AXP202 Init Fail!");
    }

    axp.setPowerOutPut(AXP202_LDO2, AXP202_ON);  // Provide 3.3V to LCD_VDD
    delay(100);                                  // Small delay to allow LCD to power up properly
    axp.setPowerOutPut(AXP202_DCDC3, AXP202_ON); // Provide 3.3V to MCU
    delay(100);                                  // Small delay to allow MCU to power up properly
    powerOn = true;

    axp.enableIRQ(AXP202_PEK_SHORTPRESS_IRQ, true);
    axp.clearIRQ();
    delay(10);

    pinMode(35, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(35), axpISR, FALLING);
}

void Power::togglePower()
{
    if (axpPressed)
    {
        axpPressed = false;
        axp.readIRQ();

        if (powerOn)
        {
            axp.setPowerOutPut(AXP202_LDO2, AXP202_OFF); // Turn off LCD
            delay(100);                                  // Small delay to allow LCD to power down properly
            powerOn = false;
            Serial.println("Power Off");
        }
        else
        {
            axp.setPowerOutPut(AXP202_LDO2, AXP202_ON); // Turn on LCD
            delay(100);                                 // Small delay to allow LCD to power up properly
            powerOn = true;
            Serial.println("Power On");
        }
        axp.clearIRQ();
        delay(10);
    }
}

int Power::getBatteryPercentage()
{
    return axp.getBattPercentage();
}