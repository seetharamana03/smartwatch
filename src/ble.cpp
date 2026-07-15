#include "ble.h"

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Arduino.h>

namespace {
    constexpr char SERVICE_UUID[] = "4fafc201-1fb5-459e-8fcc-c5c9c331914b";
}

class ServerCallbacks : public BLEServerCallbacks {
public:
    explicit ServerCallbacks(BLEManager* mgr) : _mgr(mgr) {}
    void onConnect(BLEServer*) override    { _mgr->onConnect(); }
    void onDisconnect(BLEServer*) override { _mgr->onDisconnect(); }
private:
    BLEManager* _mgr;
};

void BLEManager::begin(const char* deviceName) {
    BLEDevice::init(deviceName);

    _server = BLEDevice::createServer();
    _server->setCallbacks(new ServerCallbacks(this));

    _service = _server->createService(SERVICE_UUID);
}

BLECharacteristic* BLEManager::addCharacteristic(const char* uuid, uint32_t props) {
    BLECharacteristic* ch = _service->createCharacteristic(uuid, props);

    // The CCCD (BLE2902) is only meaningful when the client can subscribe.
    if (props & (BLECharacteristic::PROPERTY_NOTIFY |
                 BLECharacteristic::PROPERTY_INDICATE)) {
        ch->addDescriptor(new BLE2902());
    }
    return ch;
}

void BLEManager::start() {
    _service->start();

    BLEAdvertising* advertising = BLEDevice::getAdvertising();
    advertising->addServiceUUID(SERVICE_UUID);
    advertising->setScanResponse(false);
    advertising->setMinPreferred(0x0);
    BLEDevice::startAdvertising();
}

void BLEManager::setValue(BLECharacteristic* ch, uint32_t value) {
    if (!ch) return;
    ch->setValue((uint8_t*)&value, sizeof(value));
    if (_connected) ch->notify();
}

void BLEManager::loop() {
    if (!_connected && _wasConnected) {
        delay(500);
        _server->startAdvertising();
        _wasConnected = false;
    }
    if (_connected && !_wasConnected) {
        _wasConnected = true;
    }
}