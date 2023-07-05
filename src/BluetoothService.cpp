#include "BluetoothService.h"

#include <Arduino.h>
#include <NimBLEDevice.h>

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define DEVICE_NAME "Elevador Squad 5"

uint8_t blValue = '\0';
bool oldValue = true;

#define FLOOR_MIN 1
#define FLOOR_MAX 3

BluetoothService *BluetoothService::singleton_ = nullptr;

BluetoothService::BluetoothService()
    : pServer(nullptr),
      pService(nullptr),
      pCharacteristic(nullptr),
      pAdvertising(nullptr) {}

BluetoothService *BluetoothService::GetInstance() {
    if (singleton_ == nullptr) singleton_ = new BluetoothService();
    return singleton_;
}

class Callback : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
        std::string value = pCharacteristic->getValue();
        uint8_t number = '0' + value[0];

        if (value.length() != 1 || number < ('0'+FLOOR_MIN) || number > ('0'+FLOOR_MAX)) {
            Serial.println("Comando invalido >:]");
            return;
        }

        Serial.print("Comando recebido: ");
        Serial.println(number);
        blValue = number;

        oldValue = false;
    }
};

void BluetoothService::setupBluetooth() {
    BLEDevice::init("MyESP32");
    BLEServer *pServer = BLEDevice::createServer();

    BLEService *pService = pServer->createService(SERVICE_UUID);

    BLECharacteristic *pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID, NIMBLE_PROPERTY::WRITE);

    pCharacteristic->setCallbacks(new Callback());

    pCharacteristic->setValue("Hello World");
    pService->start();

    BLEAdvertising *pAdvertising = pServer->getAdvertising();
    pAdvertising->start();

    Serial.println("Characteristic defined! Now you can read it in your phone!");
}

uint8_t BluetoothService::getBluetoothValue() { 
    oldValue = true; 
    return blValue; 
}

bool BluetoothService::isOldValue() { return oldValue; }