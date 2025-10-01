#include "display.h"
#include "mywifi.h"
#include "mytime.h"
#include "power.h"
#include "icons.h"
#include "touch.h"

const unsigned long SCREEN_TIMEOUT = 20000; // 20 seconds
// bool screenOn = false;
unsigned long lastInteraction = 0;
static unsigned long lastUpdate = 0;

Button fitnessButton = {20, 50, 20, 20, "F", TFT_BLUE};
MyDisplay display;
PowerManager pm;
TouchManager touch;
// MyTime time;
MyWifi wifi("HotspotAnand", "chamali1");
int16_t x = 0;
int16_t y = 0;
int16_t z = 0;

const int BUTTON_PIN = 35; // GPIO0 for the button

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);
  wifi.connect();
  pm.initButton();
  display.begin();
  touch.begin();
  MyTime::init();
}

void loop()
{
  String currentTime = MyTime::getTimeString();
  String currentDate = MyTime::getDateString();
  display.drawHomeScreen(currentTime, currentDate, fitnessButton);

  if (pm.checkButtonPress())
  {
    if (display.isOn())
    {
      display.turnOff();
    }
    else
    {
      display.turnOn();
    }
  }

  if (touch.touched())
  {
    int x, y;
    touch.getPoint(x,y);
    Serial.printf("Touch at X=%d, Y=%d\n", x, y);
  }
  delay(10);
}
