#include "Monitor.h"

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

Monitor::Monitor() {}
Monitor::~Monitor() {}

class Callback : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
        std::string value = pCharacteristic->getValue();

        if (value.length() != 1 || value[0] < 0 || value[0] > 2) {
            Serial.println("Comando invalido >:]");
            return;
        }

        Serial.print("Comando recebido: ");
        Serial.println((int)value[0]);
        bluetoothCommand = (int)value[0];
    }
};

void Monitor::setupBluetooth() {
    BLEDevice::init("Elevador Squad 5");
    BLEServer *pServer = BLEDevice::createServer();
    BLEService *pService = pServer->createService(SERVICE_UUID);
    BLECharacteristic *pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_WRITE);

    pCharacteristic->setCallbacks(new Callback());

    pCharacteristic->setValue("3");
    pService->start();

    BLEAdvertising *pAdvertising = pServer->getAdvertising();
    pAdvertising->start();
}

void Monitor::setupCommand() {
    // Criação da fábrica de lógica
    commandBuilder = new CommandBuilder();

    // Habilitação da fábrica de lógica
    commandBuilder->setEnable(true);

    // Criação das lógicas na fábrica
    commandBuilder->buildDataRegister();
    commandBuilder->buildAsciiToHexa();

    // Configuração das lógicas na fábrica
    commandBuilder->setupDataRegister();
    commandBuilder->setupAsciiToHexa();

    // Criação de microsserviço
    commandUs = new CommandUs();

    // Obtenção das lógicas da fábrica
    dataRegister = commandBuilder->getDataRegister();
    asciiToHexa = commandBuilder->getAsciiToHexa();

    // Configuração dos microsserviços
    commandUs->setDataRegister(dataRegister);

    commandUs->setAsciiToHexa(asciiToHexa);

    // Habilitação e reset do microsserviço
    commandUs->setEnable(1);

    commandUs->doResetMicroservice();
}

void Monitor::commandLoop() {
    // commandUs->setDoorAction(doorAction);
    // commandUs->setCabinState(cabinState);
    // commandUs->setCurrentFloor(currentFloor);

    commandUs->setBluetoothData(bluetoothCommand);

    commandUs->doMicroservice();

    requestedfloor = commandUs->getRequestedFloor();
    startTest = commandUs->getStartTest();
    startDoor = commandUs->getStartDoor();
    startCabin = commandUs->getStartCabin();
    state = commandUs->getState();
    doorAction = commandUs->getDoorAction();

    Serial.print(requestedfloor);
    Serial.print(",");
    Serial.print(startDoor);
    Serial.print(",");
    Serial.print(startCabin);
    Serial.print(",");
    Serial.print(state);
    Serial.print(",");
    Serial.print(doorAction);
}