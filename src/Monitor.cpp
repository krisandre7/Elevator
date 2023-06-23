#include "Monitor.h"
#include "BluetoothService.h"

#include <Arduino.h>

Monitor* Monitor::singleton_= nullptr;


Monitor::Monitor() {
    BluetoothService *bluetoothService = BluetoothService::GetInstance();
}

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
    bluetoothService->setupBluetooth();

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

    // Serial.print(requestedfloor);
    // Serial.print(",");
    // Serial.print(startDoor);
    // Serial.print(",");
    // Serial.print(startCabin);
    // Serial.print(",");
    // Serial.println(state);
    // Serial.print(",");
    // Serial.print(doorAction);
}