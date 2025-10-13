#include "motor.h"
#include <Arduino.h>

static uint8_t motor_pin = 4;

bool motor_init(uint8_t pin)
{
    motor_pin = pin;
    pinMode(motor_pin, OUTPUT);
    digitalWrite(motor_pin, LOW); // Ensure motor is off 
    printf("Motor initialized on pin %d\n", motor_pin);
    return true;
}

void motor_on(void)
{
    digitalWrite(motor_pin, HIGH);
    printf("Motor ON\n");
}

void motor_off(void)
{
    digitalWrite(motor_pin, LOW);
    printf("Motor OFF\n");
}