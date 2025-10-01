#include "display.h"
#define VIBRATION_PIN 4
#define LCD_BACKLIGHT 12

MyDisplay::MyDisplay() : tft(TFT_eSPI()) {}

void MyDisplay::drawText(const String& text, int x, int y, int font, uint16_t color, uint16_t bg){
    tft.setTextColor(color, bg);
    tft.drawString(text, x, y, font);
}

// void MyDisplay::drawIcon(const uint16_t *icon, int x, int y, int w, int h){
//     tft.pushImage(x, y, w, h, icon);
// }

void MyDisplay::drawButton(const Button &btn) {
    tft.fillRect(btn.x, btn.y, btn.w, btn.h, btn.color);
    drawText(btn.label, btn.x + (btn.w / 4), btn.y + (btn.h / 4), 2, TFT_WHITE, btn.color);
}

bool MyDisplay::isPressed(const Button &btn, int touchX, int touchY) {
    return (touchX >= btn.x && touchX <= (btn.x + btn.w) &&
            touchY >= btn.y && touchY <= (btn.y + btn.h));
}

void MyDisplay::begin() {
    pinMode(LCD_BACKLIGHT, OUTPUT);
    digitalWrite(LCD_BACKLIGHT, HIGH);

    tft.init();
    tft.setRotation(2);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    
    pinMode(VIBRATION_PIN, OUTPUT);
    digitalWrite(VIBRATION_PIN, LOW);
}

void MyDisplay::drawHomeScreen(String currentTime, Button fitnessButton){
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    drawText(currentTime, 130, 50, 4);
    drawButton(fitnessButton);
    
    // drawText("F", 20, 50, 4);
    // drawIcon(icon, 20, 50, 64, 64);
}

void MyDisplay::turnScreenOn(){
    tft.writecommand(0x11); // Sleep OUT command
    delay(120);             // Wait for the screen to wake up
    tft.writecommand(0x29); // Display ON command
    tft.fillScreen(TFT_BLACK);
    digitalWrite(LCD_BACKLIGHT, HIGH);
    Serial.println("Screen turned on");
}

void MyDisplay::turnScreenOff(){
    tft.writecommand(0x28); // Display OFF command
    tft.writecommand(0x10); // Sleep IN command
    Serial.println("Screen turned off");
}
