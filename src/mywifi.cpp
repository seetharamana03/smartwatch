#include "mywifi.h"

MyWifi::MyWifi(const char* ssid, const char* password)
    : _ssid(ssid), _password(password) {}

void MyWifi::connect() {
    WiFi.begin(_ssid, _password);
    Serial.print("Connecting to WiFi");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP().toString());
}

bool MyWifi::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

String MyWifi::getIP() {
    return WiFi.localIP().toString();
}