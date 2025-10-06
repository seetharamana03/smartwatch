#include "display.h"
#include "mywifi.h"
#include "mytime.h"
#include "power.h"
#include "icons.h"
#include "touch.h"

const unsigned long SCREEN_TIMEOUT = 20000; // 20 seconds
unsigned long lastInteraction = 0;
static unsigned long lastUpdate = 0;

Button fitnessButton = {20, 50, 20, 20, "F", TFT_BLUE};
MyDisplay display;
PowerManager pm;
TouchManager touch;
MyTime myTime;
MyWifi wifi("HotspotAnand", "chamali1");
int16_t x = 0;
int16_t y = 0;
int16_t z = 0;

const int BUTTON_PIN = 35; // GPIO0 for the button

void setup()
{
  //First Initialization
  Serial.begin(115200);
  delay(1000);
  Serial.println("Smartwatch starting...");
  
  //Display Initialization
  pm.initPower();
  display.begin();
  myTime.init();

  wifi.connect();
  pm.initButton();
  touch.begin();
  // MyTime::init();
}

void loop()
{
  String currentTime = myTime.getTimeString();
  String currentDate = myTime.getDateString();
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
  delay(1000);
}
