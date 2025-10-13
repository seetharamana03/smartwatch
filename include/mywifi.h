#pragma once
#include <cmath>
#include <WiFi.h>   // ESP32 core WiFi library

class MyWifi {
public:
    MyWifi(const char* ssid, const char* password);
    void connect();
    bool isConnected();
    String getIP();
private:
    const char* _ssid;
    const char* _password;
};