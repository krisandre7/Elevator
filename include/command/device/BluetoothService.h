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

    /**
     * This is the static method that controls the access to the
     * bluetoothService instance. On the first run, it creates a
     * bluetoothService object and places it into the static field. On
     * subsequent runs, it returns the client existing object stored in the
     * static field.
     */
    static BluetoothService *GetInstance();

    void setupBluetooth();

    uint8_t getBluetoothValue();

    bool isOldValue();
};