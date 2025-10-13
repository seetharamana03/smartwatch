#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdint.h>

    bool motor_init(uint8_t pin);
    void motor_on(void);
    void motor_off(void);

#ifdef __cplusplus
}
#endif

#endif





