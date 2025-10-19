#pragma once
#include <cmath>
#include <TFT_eSPI.h>
#include <time.h>

typedef enum
{
    SCREEN_HOME,
    SCREEN_WIFI,
    SCREEN_ACCELEROMETER,
    SCREEN_TIMER
} ScreenState;
extern ScreenState currentScreen;

class Display
{
public:
    Display();
    void initialize();
    void writeText(String text, int x, int y, int size = 2, uint16_t color = TFT_WHITE, uint16_t bgColor = TFT_BLACK);
    void handleHomeScreen(struct tm *timeinfo, int *battery, uint16_t *x, uint16_t *y);
    void handleWifiScreen(const char *ssid, bool isConnected, uint16_t *x, uint16_t *y);
    void handleAccelerometerScreen(uint32_t *steps, uint16_t *x, uint16_t *y);
    void handleStopwatchScreen(uint16_t *x, uint16_t *y, bool *stopWatchRunning, unsigned long *startMillis, unsigned long *elapsedMillis);
    void drawWiFiSymbol(int x, int y, uint16_t color = TFT_WHITE);
    void drawHomeSymbol(int x, int y, uint16_t color = TFT_WHITE, uint16_t bgColor = TFT_BLACK);
    void drawRefreshIcon(int x, int y, uint16_t color = TFT_WHITE);
    // void drawHeartIcon(int x, int y, uint16_t color = TFT_WHITE);
    void drawClockIcon(int x, int y, uint16_t color = TFT_WHITE);
    void drawFitnessIcon(int x, int y, uint16_t color = TFT_WHITE);
    void drawBatteryIcon(int x, int y, int battery, uint16_t color = TFT_WHITE);
    void fillScreen();
    void updateStopwatchTime(unsigned long elapsedMillis);
    // void updateStopwatchTime(unsigned long elapsedMillis);

private:
    TFT_eSPI tft;
};