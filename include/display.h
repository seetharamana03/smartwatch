#pragma message("Compiling with TFT setup: " __FILE__)
#include <cmath>
#include <TFT_eSPI.h>

struct Button {
    int x, y, w, h;
    String label;
    uint16_t color;
};

class MyDisplay {
public:
    MyDisplay();
    void begin();
    void turnOn();
    void turnOff();
    bool isOn() const;
    void drawHomeScreen(String currentTime, String currentDate, Button fitnessButton);
    void drawFitnessScreen();

    void turnScreenOn();
    void turnScreenOff();
private:
    TFT_eSPI tft;
    bool _isOn;
    void drawText(const String& text, int x, int y, int font = 2, uint16_t color = TFT_WHITE, uint16_t bg = TFT_BLACK);
    void drawButton(const Button &btn);
    bool isPressed(const Button &btn, int touchX, int touchY);
};