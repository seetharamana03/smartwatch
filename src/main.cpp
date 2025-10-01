// #include "display.h"
// #include "mywifi.h"
// #include "mytime.h"
// #include "power.h"

// const unsigned long SCREEN_TIMEOUT = 20000; // 20 seconds
// bool screenOn = false;
// unsigned long lastInteraction = 0;
// static unsigned long lastUpdate = 0;

// MyDisplay display;
// PowerManager power;
// // MyTime time;
// MyWifi wifi("HotspotAnand", "chamali1");
// int16_t x = 0;
// int16_t y = 0;
// int16_t z = 0;

// const int BUTTON_PIN = 35; // GPIO0 for the button

// void setup() {
//   // put your setup code here, to run once:
//   Serial.begin(115200);
//   delay(1000);

//   wifi.connect();
//   display.begin();
//   MyTime::init();
//   display.turnScreenOff();
//   power.initButton();
// }

// void loop() {

//   if(power.checkButtonPress()) {
//     lastInteraction = millis();
//     if (screenOn) {
//       display.turnScreenOff();
//       screenOn = false;
//     } else {
//       display.turnScreenOn();
//       screenOn = true;
//     }
//   }

//   if (screenOn && (millis() - lastInteraction > SCREEN_TIMEOUT)) {
//     display.turnScreenOff();
//     screenOn = false;
//   }

//   if(millis() - lastUpdate > 1000) {
//     lastUpdate = millis();
//     if(screenOn) {
//       String currentTime = MyTime::getTimeString();
//       display.drawText(currentTime, 100, 50, 4);
//     }
//   }
//   // String currentTime = MyTime::getTimeString();
//   // display.drawText(currentTime, 100, 50, 4);
//   // delay(1000);
// }

#include "display.h"
#include "mywifi.h"
#include "mytime.h"
#include "power.h"
#include "icons.h"

const unsigned long SCREEN_TIMEOUT = 20000; // 20 seconds
bool screenOn = false;
unsigned long lastInteraction = 0;
static unsigned long lastUpdate = 0;

Button fitnessButton = {20, 50, 20, 20, "F", TFT_BLUE};
MyDisplay display;
PowerManager power;
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
  display.begin();
  MyTime::init();
}

void loop()
{
  String currentTime = MyTime::getTimeString();
  display.drawHomeScreen(currentTime, fitnessButton);
  delay(1000);
}
