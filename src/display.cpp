#include "display.h"
#define VIBRATION_PIN 4
#define LCD_BACKLIGHT 12

MyDisplay::MyDisplay() : tft(TFT_eSPI()), _isOn(false) {}

void MyDisplay::begin()
{
    pinMode(LCD_BACKLIGHT, OUTPUT);
    digitalWrite(LCD_BACKLIGHT, HIGH);
    _isOn = true;

    tft.init();
    tft.setRotation(2);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    pinMode(VIBRATION_PIN, OUTPUT);
    digitalWrite(VIBRATION_PIN, LOW);
}

void MyDisplay::turnOn()
{
    digitalWrite(LCD_BACKLIGHT, HIGH);
    _isOn = true;
    tft.writecommand(ST7789_DISPON);
}

void MyDisplay::turnOff()
{
    digitalWrite(LCD_BACKLIGHT, LOW);
    _isOn = false;
    tft.writecommand(ST7789_DISPOFF);
}

bool MyDisplay::isOn() const
{
    return _isOn;
}

void MyDisplay::turnScreenOn()
{
    digitalWrite(LCD_BACKLIGHT, HIGH);
}

void MyDisplay::turnScreenOff()
{
    digitalWrite(LCD_BACKLIGHT, LOW);
}

void MyDisplay::drawText(const String &text, int x, int y, int font, uint16_t color, uint16_t bg)
{
    tft.setTextColor(color, bg);
    tft.drawString(text, x, y, font);
}

void MyDisplay::drawButton(const Button &btn)
{
    tft.fillRect(btn.x, btn.y, btn.w, btn.h, btn.color);
    drawText(btn.label, btn.x + (btn.w / 4), btn.y + (btn.h / 4), 2, TFT_WHITE, btn.color);
}

bool MyDisplay::isPressed(const Button &btn, int touchX, int touchY)
{
    return (touchX >= btn.x && touchX <= (btn.x + btn.w) &&
            touchY >= btn.y && touchY <= (btn.y + btn.h));
}

void MyDisplay::drawHomeScreen(String currentTime, String currentDate, Button fitnessButton)
{
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    drawText(currentTime, 90, 20, 4);
    drawText(currentDate, 86, 45, 2);
    // drawButton(fitnessButton);

    // drawText("F", 20, 50, 4);
    // drawIcon(icon, 20, 50, 64, 64);
}