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

void Display::handleHomeScreen(struct tm *timeinfo, uint16_t *x, uint16_t *y)
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

    if (*x < 85 && *x > 20 && *y < 220 && *y > 150)
    {
        Serial.println("Wifi Button Pressed");
        currentScreen = SCREEN_WIFI;
    }
    else if (*x < 155 && *x > 85 && *y < 220 && *y > 150)
    {
        Serial.println("Timer Button Pressed");
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
