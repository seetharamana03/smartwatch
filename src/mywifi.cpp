#include "mywifi.h"

MyWifi::MyWifi(const char *ssid, const char *password)
    : _ssid(ssid), _password(password) {}

void MyWifi::connect()
{
    WiFi.begin(_ssid, _password);
    Serial.print("Connecting to WiFi");

    unsigned long startAttemptTime = millis();
    const unsigned long timeout = 10000;

    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < timeout)
    {
        delay(500);
        Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("\nWiFi connected");
        // Serial.print("IP address: ");
        // Serial.println(WiFi.localIP().toString());
    }
    else
    {
        Serial.println("\nWifi connection failed");
        WiFi.disconnect(true);
    }
}

bool MyWifi::isConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

String MyWifi::getIP()
{
    return WiFi.localIP().toString();
}