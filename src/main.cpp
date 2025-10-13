#include <Arduino.h>
#include "display.h"
#include "power.h"
#include "mywifi.h"
#include "motor.h"
#include "mytime.h"
#include "touch.h"
#include "accel.h"

Display display;
Power power;
const char *ssid = "HotspotAnand";
const char *password = "chamali1";
MyWifi wifi(ssid, password);
bool isConnected = false;
ScreenState currentScreen = SCREEN_HOME;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin(21, 22);
  power.initPower();
  delay(100);

  accel_init();
  ft6236u_init();
  display.initialize();
  mytime_init();
  wifi.connect();

  if (wifi.isConnected())
  {
    syncTimeWithNTP();
  }
  else
  {
    Serial.println("Skipping NTP sync, WiFi not connected");
  }

  if (wifi.isConnected())
  {
    struct tm ntpTime;
    getLocalTime(&ntpTime);
    pcf8563_set_time(&ntpTime);
  }
}

void loop()
{
  // // put your main code here, to run repeatedly:
  struct tm rtcTime;
  int16_t dirx, diry, dirz;
  uint16_t x, y;
  uint32_t steps;
  pcf8563_get_time(&rtcTime);
  power.togglePower();
  checkTouch(&x, &y);
  steps = accel_get_steps();

  switch (currentScreen)
  {
  case SCREEN_HOME:
    display.handleHomeScreen(&rtcTime, &x, &y);
    break;
  case SCREEN_WIFI:
    display.handleWifiScreen(ssid, wifi.isConnected(), &x, &y);
    if (x < 90 && x > 20 && y < 220 && y > 150)
    {
      Serial.println("Connect Button Pressed");
      wifi.connect();
    }
    break;
  case SCREEN_ACCELEROMETER:
    display.handleAccelerometerScreen(&steps, &x, &y);
  }

  if (x != 0 && y != 0)
  {
    Serial.printf("Touch at (%d, %d)\n", x, y);
  }

  delay(100);
}