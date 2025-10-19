#include "display.h"
#include "mywifi.h"
#define VIBRATON_PIN 4
#define LCD_BACKLIGHT_PIN 12

Display::Display() : tft(TFT_eSPI()) {}

void Display::initialize()
{
    tft.init();
    tft.setRotation(2); // Set rotation to landscape mode
    pinMode(LCD_BACKLIGHT_PIN, OUTPUT);
    digitalWrite(LCD_BACKLIGHT_PIN, HIGH); // Turn on backlight
    tft.fillScreen(TFT_BLACK);             // Clear the screen with black color
    // writeText("Hello World", 10, 10, 2);
}

void Display::writeText(String text, int x, int y, int size, uint16_t color, uint16_t bgColor)
{
    tft.setTextColor(color, bgColor);
    tft.setTextSize(size);
    tft.setCursor(x, y);
    tft.println(text);
}

void Display::fillScreen()
{
    tft.fillScreen(TFT_BLACK);
}

void Display::handleHomeScreen(struct tm *timeinfo, int *battery, uint16_t *x, uint16_t *y)
{
    char currentTime[9];  // HH:MM:SS
    char currentDate[11]; // YYYY-MM-DD
    strftime(currentTime, sizeof(currentTime), "%H:%M", timeinfo);
    strftime(currentDate, sizeof(currentDate), "%Y-%m-%d", timeinfo);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    writeText(String(currentTime), 90, 20, 2);
    writeText(String(currentDate), 60, 45, 2);
    drawWiFiSymbol(50, 180, TFT_WHITE);
    // drawHeartIcon(100, 180, TFT_WHITE);
    drawClockIcon(120, 170, TFT_WHITE);
    drawFitnessIcon(190, 170, TFT_WHITE);
    drawBatteryIcon(120, 95, *battery, TFT_WHITE);

    if (*x < 85 && *x > 20 && *y < 220 && *y > 150)
    {
        Serial.println("Wifi Button Pressed");
        currentScreen = SCREEN_WIFI;
    }
    else if (*x < 155 && *x > 85 && *y < 220 && *y > 150)
    {
        Serial.println("Timer Button Pressed");
        currentScreen = SCREEN_TIMER;
    }
    else if (*x < 220 && *x > 155 && *y < 220 && *y > 150)
    {
        Serial.println("Accelerometer Button Pressed");
        currentScreen = SCREEN_ACCELEROMETER;
    }
    // else
    // {
    //     currentScreen = SCREEN_HOME;
    // }
}

void Display::handleWifiScreen(const char *ssid, bool isConnected, uint16_t *x, uint16_t *y)
{
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    if (isConnected)
    {
        writeText("WiFi Connected", 10, 10, 2);
        writeText("SSID:", 10, 40, 2);
        writeText(String(ssid), 10, 60, 2);
    }
    else
    {
        writeText("WiFi Not Connected", 10, 30, 2);
    }
    drawHomeSymbol(150, 180, TFT_WHITE, TFT_BLACK);
    drawRefreshIcon(70, 187, TFT_WHITE);

    if (*x < 220 && *x > 150 && *y < 220 && *y > 150)
    {
        Serial.println("Home Button Pressed");
        currentScreen = SCREEN_HOME;
    }
}

void Display::handleAccelerometerScreen(uint32_t *steps, uint16_t *x, uint16_t *y)
{
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    writeText("Today's Steps: ", 10, 10, 2);
    writeText(String(*steps), 10, 40, 2);
    drawHomeSymbol(150, 180, TFT_WHITE, TFT_BLACK);
    if (*x < 220 && *x > 150 && *y < 220 && *y > 150)
    {
        Serial.println("Home Button Pressed");
        currentScreen = SCREEN_HOME;
    }
}

// void Display::handleStopwatchScreen(uint16_t *x, uint16_t *y, bool *stopWatchRunning, unsigned long *startMillis, unsigned long *elapsedMillis)
// {

//     // Start/Stop Button
//     if (*x > 25 && *x < 115 && *y > 180 && *y < 220)
//     {
//         *stopWatchRunning = !(*stopWatchRunning);
//         if (*stopWatchRunning)
//         {
//             *startMillis = millis() - *elapsedMillis; // resume from paused
//             tft.fillRoundRect(25, 180, 90, 40, 6, TFT_ORANGE);
//             tft.drawString("STOP", 45, 190, 2);
//         }
//         else
//         {
//             tft.fillRoundRect(25, 180, 90, 40, 6, TFT_GREEN);
//             tft.drawString("START", 40, 190, 2);
//         }
//     }

//     // Reset Button
//     if (*x > 135 && *x < 225 && *y > 180 && *y < 220)
//     {
//         *stopWatchRunning = false;
//         *elapsedMillis = 0;
//         updateStopwatchTime(0);

//         tft.fillRoundRect(25, 180, 90, 40, 6, TFT_GREEN);
//         tft.drawString("START", 40, 190, 2);
//     }

//     tft.fillScreen(TFT_BLACK);

//     // Title
//     tft.setTextColor(TFT_CYAN, TFT_BLACK);
//     tft.drawString("STOPWATCH", 50, 20, 4);

//     // Time
//     tft.setTextColor(TFT_WHITE, TFT_BLACK);
//     tft.drawString("00:00.0", 60, 80, 6);

//     // Buttons
//     tft.fillRoundRect(25, 180, 90, 40, 6, TFT_GREEN);
//     tft.drawString("START", 40, 190, 2);

//     tft.fillRoundRect(135, 180, 90, 40, 6, TFT_RED);
//     tft.drawString("RESET", 155, 190, 2);
// }

// void Display::handleStopwatchScreen(uint16_t *x, uint16_t *y, bool *stopWatchRunning, unsigned long *startTime, unsigned long *elapsedTime)
// {
//     tft.fillScreen(TFT_BLACK);
//     tft.setTextColor(TFT_WHITE);

//     writeText("Start", 15, 110, 2, TFT_WHITE, TFT_BLACK);
//     tft.drawRect(10, 104, 70, 30, TFT_WHITE);

//     writeText("Stop", 95, 110, 2, TFT_WHITE, TFT_BLACK);
//     tft.drawRect(92, 104, 55, 30, TFT_WHITE);

//     writeText("Reset", 165, 110, 2, TFT_WHITE, TFT_BLACK);
//     tft.drawRect(160, 104, 70, 30, TFT_WHITE);

//     drawHomeSymbol(150, 180, TFT_WHITE, TFT_BLACK);

//     if (*x < 220 && *x > 150 && *y < 220 && *y > 150)
//     {
//         Serial.println("Home Button Pressed");
//         currentScreen = SCREEN_HOME;
//     }

//     //Start Button
//     if (*x < 80 && *x > 10 && *y < 134 && *y > 104)
//     {
//         if (!(*stopWatchRunning))
//         {
//             *stopWatchRunning = true;
//             *startTime = millis() - *elapsedTime;
//             Serial.println("Stopwatch Started");
//         }
//     }
//     //Stop Button
//     if (*x < 147 && *x > 92 && *y < 134 && *y > 104)
//     {
//         if (*stopWatchRunning)
//         {
//             *stopWatchRunning = false;
//             *elapsedTime = millis() - *startTime;
//             Serial.println("Stopwatch Stopped");
//         }
//     }
//     //Reset Button
//     if (*x < 230 && *x > 160 && *y < 134 && *y > 104)
//     {
//         *stopWatchRunning = false;
//         *elapsedTime = 0;
//         *startTime = millis();
//         Serial.println("Stopwatch Reset");
//         updateStopwatchTime(0);
//     }

//     if (*stopWatchRunning)
//     {
//         *elapsedTime = millis() - *startTime;
//         updateStopwatchTime(*elapsedTime);
//     }
//     else
//     {
//         updateStopwatchTime(*elapsedTime);
//     }
// }

void Display::handleStopwatchScreen(uint16_t *x, uint16_t *y,
                                    bool *stopWatchRunning,
                                    unsigned long *startTime,
                                    unsigned long *elapsedTime)
{
    static bool initialized = false;

    // Draw layout only once when entering stopwatch screen
    if (!initialized)
    {
        tft.fillScreen(TFT_BLACK);
        tft.setTextColor(TFT_WHITE);

        // --- Buttons ---
        writeText("Start", 15, 110, 2, TFT_WHITE, TFT_BLACK);
        tft.drawRect(10, 104, 70, 30, TFT_WHITE);

        writeText("Stop", 95, 110, 2, TFT_WHITE, TFT_BLACK);
        tft.drawRect(92, 104, 55, 30, TFT_WHITE);

        writeText("Reset", 165, 110, 2, TFT_WHITE, TFT_BLACK);
        tft.drawRect(160, 104, 70, 30, TFT_WHITE);

        // --- Home icon ---
        drawHomeSymbol(150, 180, TFT_WHITE, TFT_BLACK);

        // Initial display
        updateStopwatchTime(*elapsedTime);
        initialized = true;
    }

    // --- Home Button ---
    if (*x < 220 && *x > 150 && *y < 220 && *y > 150)
    {
        Serial.println("Home Button Pressed");
        currentScreen = SCREEN_HOME;
        initialized = false; // force redraw next time we return
        return;
    }

    // --- Start Button ---
    if (*x < 80 && *x > 10 && *y < 134 && *y > 104)
    {
        if (!(*stopWatchRunning))
        {
            *stopWatchRunning = true;
            *startTime = millis() - *elapsedTime; // resume if paused
            Serial.println("Stopwatch Started");
        }
    }

    // --- Stop Button ---
    if (*x < 147 && *x > 92 && *y < 134 && *y > 104)
    {
        if (*stopWatchRunning)
        {
            *stopWatchRunning = false;
            *elapsedTime = millis() - *startTime;
            Serial.println("Stopwatch Stopped");
        }
    }

    // --- Reset Button ---
    if (*x < 230 && *x > 160 && *y < 134 && *y > 104)
    {
        *stopWatchRunning = false;
        *elapsedTime = 0;
        *startTime = millis();
        Serial.println("Stopwatch Reset");
        updateStopwatchTime(0);
    }
}

void Display::updateStopwatchTime(unsigned long elapsedMillis)
{
    unsigned long totalSeconds = elapsedMillis / 1000;
    int hours = totalSeconds / 3600;
    int minutes = (totalSeconds % 3600) / 60;
    int seconds = totalSeconds % 60;

    char buffer[12];
    sprintf(buffer, "%02d:%02d:%02d", hours, minutes, seconds);
    // tft.fillRect(50, 80, 180, 40, TFT_BLACK);

    // tft.setTextColor(TFT_WHITE, TFT_BLACK);
    // tft.drawString(buffer, 50, 80, 6);
    writeText(String(buffer), 22, 30, 4);
}

void Display::drawWiFiSymbol(int x, int y, uint16_t color)
{
    int r1 = 8;  // inner radius
    int r2 = 16; // middle
    int r3 = 24; // outer

    // Small filled circle at center (the dot)
    tft.fillCircle(x, y, 2, color);

    // 3 semicircle arcs
    tft.drawArc(x, y, r1, r1 - 2, 135, 225, color, TFT_BLACK); // inner arc
    tft.drawArc(x, y, r2, r2 - 2, 135, 225, color, TFT_BLACK); // middle arc
    tft.drawArc(x, y, r3, r3 - 2, 135, 225, color, TFT_BLACK); // outer arc
}

void Display::drawHomeSymbol(int x, int y, uint16_t color, uint16_t bgColor)
{
    // Dimensions
    int width = 40;  // width of house
    int height = 35; // total height of house (roof + body)
    int roofHeight = 15;

    // --- Draw the roof ---
    int x0 = x;              // bottom left of roof
    int x1 = x + width / 2;  // top peak
    int x2 = x + width;      // bottom right of roof
    int y0 = y;              // base of roof
    int y1 = y - roofHeight; // top of roof

    // Roof triangle
    tft.fillTriangle(x0, y0, x1, y1, x2, y0, color);

    // --- Draw the house body ---
    int bodyHeight = height - roofHeight;
    int bodyY = y; // starts at base of roof
    tft.fillRect(x0 + 8, bodyY, width - 16, bodyHeight, color);

    // --- Optional door ---
    int doorW = 8;
    int doorH = 10;
    int doorX = x + (width / 2) - (doorW / 2);
    int doorY = y + bodyHeight - doorH;
    tft.fillRect(doorX, doorY, doorW, doorH, bgColor);

    // --- Outline ---
    tft.drawTriangle(x0, y0, x1, y1, x2, y0, TFT_WHITE);
    tft.drawRect(x0 + 8, bodyY, width - 16, bodyHeight, TFT_WHITE);
}

void Display::drawRefreshIcon(int x, int y, uint16_t color)
{
    int radius = 18;      // overall size
    int thickness = 3;    // arc thickness
    int start_angle = 45; // where the arc begins
    int end_angle = 315;  // where the arc ends

    // Draw main arc
    tft.drawArc(x, y, radius, radius - thickness, start_angle, end_angle, color, TFT_BLACK);

    // Arrowhead parameters
    float arrow_angle = (end_angle + 8) * 0.0174533; // rotate CCW by 10 degrees
    int arrow_len = 8;
    int offset_y = 4; // keeps arrow vertically aligned

    // Tip of the arc (arrow point)
    int tip_x = x + radius * cos(arrow_angle);
    int tip_y = y - radius * sin(arrow_angle) + offset_y;

    // Base points of arrowhead
    int base1_x = tip_x - arrow_len * cos(arrow_angle - 0.5);
    int base1_y = tip_y + arrow_len * sin(arrow_angle - 0.5);
    int base2_x = tip_x - arrow_len * cos(arrow_angle + 0.5);
    int base2_y = tip_y + arrow_len * sin(arrow_angle + 0.5);

    // Draw arrowhead
    tft.fillTriangle(tip_x, tip_y, base1_x, base1_y, base2_x, base2_y, color);
}

void Display::drawClockIcon(int x, int y, uint16_t color)
{
    int r = 16; // radius of the clock face

    // Draw outer circle
    tft.drawCircle(x, y, r, color);

    // Optional center dot
    tft.fillCircle(x, y, 2, color);

    // Hour hand (pointing at 10)
    int hour_len = r - 6;
    int hour_x = x - hour_len * cos(30 * 0.0174533); // 30° = 10 o'clock position
    int hour_y = y - hour_len * sin(30 * 0.0174533);
    tft.drawLine(x, y, hour_x, hour_y, color);

    // Minute hand (pointing at 2)
    int min_len = r - 3;
    int min_x = x + min_len * cos(60 * 0.0174533); // 60° = 2 o'clock
    int min_y = y - min_len * sin(60 * 0.0174533);
    tft.drawLine(x, y, min_x, min_y, color);
}

void Display::drawFitnessIcon(int x, int y, uint16_t color)
{
    int box_w = 40;   // box width
    int box_h = 24;   // box height
    int corner_r = 4; // corner radius for rounded look

    // Draw rounded rectangle (outer box)
    tft.drawRoundRect(x - box_w / 2, y - box_h / 2, box_w, box_h, corner_r, color);

    // Heartbeat waveform inside box
    int w = box_w - 8; // padding inside box
    int h = box_h / 2; // vertical amplitude
    int cx = x;        // center X
    int cy = y;        // center Y

    // Left flat line
    tft.drawLine(cx - w / 2, cy, cx - w / 4, cy, color);

    // Upward peak
    tft.drawLine(cx - w / 4, cy, cx - w / 8, cy - h / 2, color);

    // Downward dip
    tft.drawLine(cx - w / 8, cy - h / 2, cx, cy + h / 2, color);

    // Small recovery upward
    tft.drawLine(cx, cy + h / 2, cx + w / 8, cy - h / 3, color);

    // Back to baseline
    tft.drawLine(cx + w / 8, cy - h / 3, cx + w / 2, cy, color);
}

void Display::drawBatteryIcon(int x, int y, int battery, uint16_t color)
{
    int box_w = 50;     // main battery width
    int box_h = 24;     // battery height
    int corner_r = 4;   // rounded corner radius
    int terminal_w = 4; // width of battery terminal (tip)

    // --- Draw main rounded rectangle (battery body) ---
    tft.drawRoundRect(x - box_w / 2, y - box_h / 2, box_w, box_h, corner_r, color);

    // --- Draw small terminal on right side ---
    int term_x = x + box_w / 2; // start just outside the body
    int term_y = y - box_h / 4; // vertically centered on right
    tft.drawRect(term_x, term_y, terminal_w, box_h / 2, color);

    // --- Optional inner fill indicator (placeholder for % fill) ---
    // Example: a mid-level charge (50%)
    int fill_margin = 4;
    // int fill_width = (box_w - 2 * fill_margin) * ((float)battery / 100); // 50% fill level
    if (battery > 100)
    {
        battery = 100;
    }
    int fill_width = (box_w - 2 * fill_margin) * ((float)battery / 100); // 50% fill level
    int fill_height = box_h - 2 * fill_margin;
    int fill_x = x - box_w / 2 + fill_margin;
    int fill_y = y - fill_height / 2;
    tft.fillRect(fill_x, fill_y, fill_width, fill_height, TFT_GREEN);
    String percent = String(battery) + "%";

    // writeText(percent, x - 22, y - 7, 2, TFT_WHITE, TFT_BLACK);
}

// void Display::updateStopwatchTime(unsigned long elapsedMillis)
// {
//     int minutes = (elapsedMillis / 60000);
//     int seconds = (elapsedMillis / 1000) % 60;
//     int tenths = (elapsedMillis % 1000) / 100;

//     char buffer[16];
//     sprintf(buffer, "%02d:%02d.%d", minutes, seconds, tenths);

//     // Clear previous time region
//     tft.fillRect(60, 80, 160, 40, TFT_BLACK);
//     tft.setTextColor(TFT_WHITE, TFT_BLACK);
//     tft.drawString(buffer, 60, 80, 6);
// }
