#include "accel.h"

// Global BMA423 object
static SensorBMA423 bma;

// Optional interrupt flag (not required unless you wire INT)
static volatile bool accel_irq = false;
#define ACCEL_INT_PIN 39  // adjust if needed (can be 38/39)
#define BMA423_I2C_ADDR_SECONDARY 0x19

void IRAM_ATTR accel_isr()
{
    accel_irq = true;
}

void accel_init()
{
    Serial.println("Initializing BMA423...");

    Wire.begin(21, 22);
    delay(100); // allow I2C bus to stabilize

    // Initialize the sensor
    if (!bma.begin(Wire, BMA423_I2C_ADDR_SECONDARY, 21, 22))
    {
        Serial.println("Failed to find BMA423 - check wiring!");
        while (1)
        {
            delay(1000);
        }
    }

    // Configure accelerometer
    bma.configAccelerometer();
    bma.enableAccelerometer();

    // Enable step counting features
    bma.enablePedometer(true);
    bma.resetPedometer();
    bma.enableFeature(SensorBMA423::FEATURE_STEP_CNTR, true);
    bma.enablePedometerIRQ();
    bma.configInterrupt();

    // Attach interrupt pin (optional)
    pinMode(ACCEL_INT_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(ACCEL_INT_PIN), accel_isr, RISING);

    Serial.println("BMA423 initialized successfully!");
}

uint32_t accel_get_steps()
{
    // You can poll this even without an interrupt
    return bma.getPedometerCounter();
}

void accel_read_raw(int16_t *x, int16_t *y, int16_t *z)
{
    bma.getAccelerometer(*x, *y, *z);
}
