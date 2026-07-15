#pragma once

#include <cstdint>

class BLEServer;
class BLEService;
class BLECharacteristic;

class BLEManager {
public:
    // Phase 1: init stack + create the (empty) service.
    void begin(const char* deviceName);

    // Phase 2: register a characteristic. Call after begin(), before start().
    // Returns a handle you keep so you can push values to it later.
    BLECharacteristic* addCharacteristic(const char* uuid, uint32_t props);

    // Phase 3: freeze the service + start advertising.
    void start();

    void setValue(BLECharacteristic* ch, uint32_t value);

    void loop();

    void onConnect()    { _connected = true; }
    void onDisconnect() { _connected = false; }

private:
    BLEServer*  _server  = nullptr;
    BLEService* _service = nullptr;
    bool _connected    = false;
    bool _wasConnected = false;
};