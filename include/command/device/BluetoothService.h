#pragma once
#include <Arduino.h>
#include <NimBLEDevice.h>

class BluetoothService {
   private:
    BLEServer *pServer;
    BLEService *pService;
    BLECharacteristic *pCharacteristic;
    BLEAdvertising *pAdvertising;

   protected:
    BluetoothService();

    static BluetoothService *singleton_;

   public:
    // Singletons should not be cloneable.
    BluetoothService(BluetoothService &other) = delete;

    // Singletons should not be assignable.
    void operator=(const BluetoothService &) = delete;

    // Static singleton instance
    static BluetoothService *GetInstance();

    void setupBluetooth();

    uint8_t getBluetoothValue();

    bool isOldValue();
};