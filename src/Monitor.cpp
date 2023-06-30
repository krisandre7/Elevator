#include "Monitor.h"
#include "BluetoothService.h"

#include <Arduino.h>
#include <Servo.h>

#include "CommandUs.h"
#include "DoorUs.h"

#include "CabinAction.h"
#include "CommandState.h"
#include <Stepper.h>

#include "DisplayUs.h"
#include "DisplayBuilder.h"

#define BIT_SIZE 7
#define MODULE_SIZE 90
#define PIN_SG90 32
#define PIN_DISPLAY_CLK 14
#define PIN_DISPLAY_DIO 12

Servo servo;

#define STEPS_PER_REVOLUTION 65 //NÚMERO DE PASSOS POR VOLTA
#define STEPPER_SPEED 400
Stepper stepper(STEPS_PER_REVOLUTION, 16, 18, 17, 19); //INICIALIZA O MOTOR

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
    commandDataRegister = commandBuilder->getDataRegister();
    asciiToHexa = commandBuilder->getAsciiToHexa();

    // Configuração dos microsserviços
    commandUs->setDataRegister(commandDataRegister);

    commandUs->setAsciiToHexa(asciiToHexa);

    // Habilitação e reset do microsserviço
    commandUs->setEnable(1);

    commandUs->doResetMicroservice();
}

void Monitor::setupCabin() {
    cabinBuilder = new CabinBuilder();
    cabinBuilder->setEnable(true);
    cabinBuilder->buildComparator();
    cabinBuilder->buildDataRegister();
    cabinBuilder->buildDownCounter();
    
    cabinUs = new CabinUs();

    cabinDataRegister = cabinBuilder->getDataRegister();
    comparator = cabinBuilder->getComparator();
    downCounter = cabinBuilder->getDownCounter();

    cabinUs->setComparator(comparator);
    cabinUs->setDataRegister(cabinDataRegister);
    cabinUs->setDownCounter(downCounter);

    cabinUs->setEnable(true);
    cabinUs->doResetMicroservice();

    stepper.setSpeed(STEPPER_SPEED);
}

void Monitor::setupDisplay(){
    displayBuilder = new DisplayBuilder();

    displayBuilder->setEnable(1);
    displayBuilder->buildDisplay(PIN_DISPLAY_CLK, PIN_DISPLAY_DIO);
    displayBuilder->setupDisplay();

    displayUs = new DisplayUs();
    
    displayUs->setEnable(1);

    displayUs->doResetMicroservice();
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

    // if (commandUs->getState() == CommandState::S_MOVE_CABIN) {
    //     commandUs->setCabinAction(bluetoothService->getBluetoothValue() > commandUs->getCurrentFloor()
    //         ? CabinAction::S_TO_UP : CabinAction::S_TO_DOWN);
    // }

    // if (commandUs->getState() == CommandState::S_WAIT_MOVING_CABIN) {
    //     commandUs->setCabinAction(CabinAction::S_STOPPED);
    //     commandUs->setCurrentFloor(bluetoothService->getBluetoothValue() - '0');
    // }

    // entradas da porta (a pedido do Fernando)
    commandUs->setDoorAction(doorUs->getAction());
    commandUs->setIsOldValue(bluetoothService->isOldValue());
    commandUs->setBluetoothData(bluetoothService->getBluetoothValue());

    // entradas da cabine
    commandUs->setCurrentFloor(cabinUs->getCurrentFloor());
    commandUs->setCabinAction(cabinUs->getCabinAction());

    prints();
}


void Monitor::cabinLoop() {
    cabinUs->doMicroservice();

    cabinUs->setRequestedFloor(commandUs->getRequestedFloor());
    cabinUs->setStartCabin(commandUs->getStartCabin());

    stepper.step(cabinUs->getClkwise() ? cabinUs->getSteps() : -cabinUs->getSteps());
}

void Monitor::displayLoop(){

    if(displayUs->getState()==DisplayState::S_TEST){
        displayBuilder->setAllDigits();
        displayBuilder->printDataDisplay();
    }
    if(displayUs->getState()==DisplayState::S_SHOW){
        displayBuilder->setData(commandUs->getCabinAction(),commandUs->getCurrentFloor());
        displayBuilder->printDataDisplay();
    }
    displayUs->doMicroservice();
}
void Monitor::prints() {
    Serial.print(bluetoothService->getBluetoothValue());
    Serial.print(",");
    Serial.print((int) commandUs->getState());
    Serial.print(",");
    Serial.print((int) cabinUs->getState());
    Serial.print(",");
    Serial.print((int) cabinUs->getCabinAction());
    Serial.print(",");
    Serial.print(commandUs->getRequestedFloor());
    Serial.print(",");
    Serial.print(cabinUs->getStartCabin());
    Serial.print(",");
    Serial.println(cabinUs->getClkwise());
    // Serial.print(doorUs->getAngle());
    // Serial.print(",");
    // Serial.print(commandUs->getCurrentFloor());
    // Serial.print(",");
    // Serial.println(commandUs->getRequestedFloor());
    // Serial.print((int) doorUs->getAction());
    // Serial.print(",");
    // Serial.print((int) commandUs->getDoorMode());
    // Serial.print(",");
    // Serial.println((int) commandUs->getDoorStart());
}