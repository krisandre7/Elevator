#include "Monitor.h"
#include "BluetoothService.h"

#include <Arduino.h>
#include <Servo.h>

#include "CommandUs.h"
#include "DoorUs.h"

#define BIT_SIZE 7
#define MODULE_SIZE 90
#define PIN_SG90 32

Servo servo;

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

void Monitor::setupDoor() {
    servo.attach(PIN_SG90);

    doorBuilder = new DoorBuilder();
    doorBuilder->setEnable(true);

    doorBuilder->buildUpDownCounter(BIT_SIZE);
    doorBuilder->setupUpDownCounter(MODULE_SIZE);

    doorUs = new DoorUs;

    upDownCounter = doorBuilder->getUpDownCounter();
    doorUs->setUpDownCounter(upDownCounter);

    doorUs->setEnable(1);

    doorUs->doResetMicroservice();
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

void Monitor::doorLoop() {
    doorUs->doMicroservice();    

    // entradas
    doorUs->setDoorMode(commandUs->getDoorMode());
    doorUs->setDoorStart(commandUs->getDoorStart());
    doorUs->setReset(commandUs->getReset());

    // saída device
    servo.write(doorUs->getAngle());
}

void Monitor::commandLoop() {
    commandUs->doMicroservice();

    // Esses dois ifs sou eu forçando o command a terminar o teste
    // e a achar que a cabine subiu para o andar desejado
    if (commandUs->getState() == CommandState::S_TEST_REQUEST) {
        commandUs->setTestIsRunning(true);
    } else if (commandUs->getTestIsRunning()) {
        commandUs->setTestIsRunning(false);
    }

    if (commandUs->getState() == CommandState::S_WAIT_MOVING_CABIN) {
        commandUs->setCabinState(CabinState::S_STOPPED);
        commandUs->setCurrentFloor(bluetoothService->getBluetoothValue());
    }

    // entradas
    commandUs->setDoorAction(doorUs->getAction());
    commandUs->setBluetoothData(bluetoothService->getBluetoothValue());

    prints();
}

void Monitor::prints() {
    Serial.print(bluetoothService->getBluetoothValue());
    Serial.print(",");
    Serial.print((int) commandUs->getState());
    Serial.print(",");
    Serial.print(doorUs->getAngle());
    Serial.print(",");
    Serial.print((int) doorUs->getAction());
    Serial.print(",");
    Serial.print((int) commandUs->getDoorMode());
    Serial.print(",");
    Serial.println((int) commandUs->getDoorStart());
}