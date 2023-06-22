#include "BluetoothService.h"

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define DEVICE_NAME "Elevador Squad 5"

int blValue = 3;

BluetoothService* BluetoothService::singleton_= nullptr;;

/**
 * Static methods should be defined outside the class.
 */
BluetoothService *BluetoothService::GetInstance()
{
    /**
     * This is a safer way to create an instance. instance = new BluetoothService is
     * dangeruous in case two instance threads wants to access at the same time
     */
    if(singleton_==nullptr){
        singleton_ = new BluetoothService();
    }
    return singleton_;
}

class Callback : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
        std::string value = pCharacteristic->getValue();

        if (value.length() != 1 || value[0] < 0 || value[0] > 2) {
            Serial.println("Comando invalido >:]");
            return;
        }

        Serial.print("Comando recebido: ");
        Serial.println((int)value[0]);
        blValue = (int)value[0];
    }
};

BluetoothService::BluetoothService() {}

void BluetoothService::setupBluetooth() {
    BLEDevice::init(DEVICE_NAME);
    pServer = BLEDevice::createServer();
    pService = pServer->createService(SERVICE_UUID);
    pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_WRITE);

    pCharacteristic->setCallbacks(new Callback());

    pCharacteristic->setValue("3");
    pService->start();

    pAdvertising = pServer->getAdvertising();
    pAdvertising->start();
}

int BluetoothService::getValue() {
    return blValue;
}