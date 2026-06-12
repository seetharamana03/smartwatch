//Final Driver with Extension

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
MyWifi wifi("MyHotspot", "12345678");
const char *ssid = "MyHotspot";
struct tm ntpTime;

ScreenState currentScreen = SCREEN_HOME;
uint32_t steps = 0;
uint16_t touchX = 0, touchY = 0;

bool stopWatchRunning = false;
unsigned long startMillis = 0;
unsigned long elapsedMillis = 0;
SemaphoreHandle_t displayMutex;

unsigned long lastTouchTime = 0;
const unsigned long touchCooldown = 300; //milliseconds

// Task handles (optional)
TaskHandle_t displayTaskHandle;
TaskHandle_t sensorTaskHandle;
// TaskHandle_t touchTaskHandle;
// TaskHandle_t wifiTaskHandle;
TaskHandle_t stopwatchTaskHandle;

// --- Tasks ---
void taskDisplay(void *pvParams);
void taskSensors(void *pvParams);
void taskStopWatch(void *pvParams);
// void taskTouch(void *pvParams);
// void taskWiFi(void *pvParams);

void setup()
{
  Serial.begin(115200);
  Wire.begin(21, 22);

  power.initPower();
  ft6236u_init();
  display.initialize();
  accel_init();
  mytime_init();
  wifi.connect();
  if (wifi.isConnected())
  {
    syncTimeWithNTP();
    getLocalTime(&ntpTime);
    pcf8563_set_time(&ntpTime);
  }

  displayMutex = xSemaphoreCreateMutex();

  // Create FreeRTOS tasks
  xTaskCreatePinnedToCore(taskDisplay, "Display", 4096, NULL, 1, &displayTaskHandle, 1);
  xTaskCreatePinnedToCore(taskSensors, "Sensors", 4096, NULL, 1, &sensorTaskHandle, 1);
  xTaskCreatePinnedToCore(taskStopWatch, "Stopwatch", 4096, NULL, 1, &stopwatchTaskHandle, 1);
  // xTaskCreatePinnedToCore(taskTouch, "Touch", 4096, NULL, 2, &touchTaskHandle, 1);
  // xTaskCreatePinnedToCore(taskWiFi,    "WiFi",    4096, NULL, 1, &wifiTaskHandle, 1);
}

void loop()
{
  // The main loop can be empty — FreeRTOS takes over
  vTaskDelete(NULL);
}

// --- Task Implementations ---

void taskDisplay(void *pvParams)
{
  struct tm rtcTime;
  static ScreenState lastScreen = SCREEN_HOME;
  static uint32_t lastSteps = 0;
  static int lastMinute = 0;
  bool needRedraw = true;
  bool needStopwatchRedraw = false;

  while (1)
  {
    pcf8563_get_time(&rtcTime);
    power.togglePower();
    checkTouch(&touchX, &touchY);
    int battery = power.getBatteryPercentage();
    int now = rtcTime.tm_min;

    if (lastMinute != now)
    {
      needRedraw = true;
      lastMinute = rtcTime.tm_min;
    }
    if (steps != lastSteps)
    {
      needRedraw = true;
      lastSteps = steps;
    }

    unsigned long nowMs = millis();
    bool validTouch = false;

    if ((touchX != 0 || touchY != 0) && (nowMs - lastTouchTime > touchCooldown))
    {
      validTouch = true;
      lastTouchTime = nowMs;
    }

    if(validTouch)
    {
      needRedraw = true;
    }

    if (needRedraw || currentScreen != lastScreen)
    {
      needRedraw = false;
      lastScreen = currentScreen;
      switch (currentScreen)
      {
      case SCREEN_HOME:
        if (xSemaphoreTake(displayMutex, portMAX_DELAY))
        {
          display.handleHomeScreen(&rtcTime, &battery, &touchX, &touchY);
          xSemaphoreGive(displayMutex);
        }
        break;
      case SCREEN_WIFI:
        if (xSemaphoreTake(displayMutex, portMAX_DELAY))
        {
          display.handleWifiScreen(ssid, wifi.isConnected(), &touchX, &touchY);
          xSemaphoreGive(displayMutex);
        }
        if (validTouch && touchX < 90 && touchX > 20 && touchY < 220 && touchY > 150)
        {
          if (xSemaphoreTake(displayMutex, portMAX_DELAY))
          {
            display.fillScreen();
            display.drawHomeSymbol(150, 180, TFT_WHITE, TFT_BLACK);
            display.drawRefreshIcon(70, 187, TFT_WHITE);
            xSemaphoreGive(displayMutex);
          }
          Serial.println("Connect Button Pressed");
          if (xSemaphoreTake(displayMutex, portMAX_DELAY))
          {
            display.writeText("Wifi Connecting...", 10, 30, 2, TFT_WHITE, TFT_BLACK);
            xSemaphoreGive(displayMutex);
          }
          wifi.connect();
          if (wifi.isConnected())
          {
            syncTimeWithNTP();
            getLocalTime(&ntpTime);
            pcf8563_set_time(&ntpTime);
          }
          needRedraw = true;
        }
        break;
      case SCREEN_ACCELEROMETER:
        if (xSemaphoreTake(displayMutex, portMAX_DELAY))
        {
          display.handleAccelerometerScreen(&steps, &touchX, &touchY);
          xSemaphoreGive(displayMutex);
        }
        break;
      case SCREEN_TIMER:
        if (xSemaphoreTake(displayMutex, portMAX_DELAY))
        {
          display.handleStopwatchScreen(&touchX, &touchY, &stopWatchRunning, &startMillis, &elapsedMillis);
          xSemaphoreGive(displayMutex);
        }
        break;
      }
    }
    touchX = 0;
    touchY = 0;
    vTaskDelay(pdMS_TO_TICKS(100)); // update every 100ms
  }
}

void taskSensors(void *pvParams)
{
  while (1)
  {
    steps = accel_get_steps();
    vTaskDelay(pdMS_TO_TICKS(1000)); // read every 1s
  }
}

void taskStopWatch(void *pvParams)
{
  while (1)
  {
    if (currentScreen == SCREEN_TIMER)
    {
      if (stopWatchRunning)
      {
        elapsedMillis = millis() - startMillis;
        if (xSemaphoreTake(displayMutex, portMAX_DELAY))
        {
          display.updateStopwatchTime(elapsedMillis);
          xSemaphoreGive(displayMutex);
        }
      }
      else
      {
        if (xSemaphoreTake(displayMutex, portMAX_DELAY))
        {
          display.updateStopwatchTime(elapsedMillis);
          xSemaphoreGive(displayMutex);
        }
      }
    }
    vTaskDelay(pdMS_TO_TICKS(200));
  }
}