#include "Monitor.h"

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

Monitor::Monitor() {}

Monitor* Monitor::singleton_= nullptr;;

/**
 * Static methods should be defined outside the class.
 */
Monitor *Monitor::GetInstance()
{
    /**
     * This is a safer way to create an instance. instance = new Monitor is
     * dangeruous in case two instance threads wants to access at the same time
     */
    if(singleton_==nullptr){
        singleton_ = new Monitor();
    }
    return singleton_;
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

    commandUs->setBluetoothData(0);

    commandUs->doMicroservice();

    requestedfloor = commandUs->getRequestedFloor();
    startTest = commandUs->getStartTest();
    startDoor = commandUs->getStartDoor();
    startCabin = commandUs->getStartCabin();
    state = commandUs->getState();
    // doorAction = commandUs->getDoorAction();

    Serial.print(requestedfloor);
    Serial.print(",");
    Serial.print(startDoor);
    Serial.print(",");
    Serial.print(startCabin);
    Serial.print(",");
    Serial.println(state);
    // Serial.print(",");
    // Serial.print(doorAction);
}