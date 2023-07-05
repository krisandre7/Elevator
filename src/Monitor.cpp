#include "Monitor.h"
#include <Arduino.h>

#include "CommandUs.h"
#include "BluetoothService.h"
#include "ButtonService.h"

#include "DoorUs.h"
#include <Servo.h>

#include "CabinAction.h"
#include "CommandState.h"
#include "Stepper.h"
#include "CabinMode.h"
#include "CabinState.h"

#include "DisplayUs.h"
#include "DisplayBuilder.h"
#include "DisplayState.h"

#define BIT_SIZE 7
#define MODULE_SIZE 90
#define PIN_SG90 32
#define DELAY 1000

Servo servo;

unsigned long last =  millis();

#define STEPS_PER_REVOLUTION 32 //NÚMERO DE PASSOS POR VOLTA
#define CLOCKWISE_SPEED 1100
#define COUNTERCLOCKWISE_SPEED 800
#define STEPPER_STEPS 16
#define STEPS_CLOCKWISE 320
#define STEPS_COUNTERCLOCKWISE 245 // NÚMERO MÁXIMO DE PASSOS PARA UM ANDAR
Stepper stepper(STEPS_PER_REVOLUTION, 16, 18, 17, 19); //INICIALIZA O MOTOR

Monitor* Monitor::singleton_= nullptr;

static bool toggleEnable = false, toggleClockwise = false;

#define BTN_TOUCH_DELAY 200

void toggleRiseCabinEn(){
  static long lastEvent = 0;
  long cur = millis();
  if(cur - lastEvent >= BTN_TOUCH_DELAY) stepper.cabinEn =  (toggleEnable^=1);
  
  lastEvent = cur;
}

void toggleRiseClkwise(){
  static long lastEvent = 0;
  long cur = millis();
  if(cur - lastEvent >= BTN_TOUCH_DELAY) stepper.cabinClk = (toggleClockwise^=1);
  lastEvent = cur;
}

Monitor::Monitor():
  bluetoothService(BluetoothService::GetInstance()),
  buttons(ButtonService::GetInstance()){
    attachInterrupt((uint8_t) PinInButton::CABIN_EN_MOVE, toggleRiseCabinEn, FALLING);
    attachInterrupt((uint8_t) PinInButton::CABIN_CLOCKWISE, toggleRiseClkwise, FALLING);
  }

Monitor *Monitor::GetInstance(){
  if(singleton_==nullptr) singleton_ = new Monitor();
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

    cabinBuilder->setupComparator();
    cabinBuilder->setupDataRegister();
    cabinBuilder->setupDownCounter(STEPS_CLOCKWISE);
    
    cabinUs = new CabinUs();

    cabinDataRegister = cabinBuilder->getDataRegister();
    comparator = cabinBuilder->getComparator();
    downCounter = cabinBuilder->getDownCounter();

    cabinUs->setComparator(comparator);
    cabinUs->setDataRegister(cabinDataRegister);
    cabinUs->setDownCounter(downCounter);

    cabinUs->setEnable(true);
    cabinUs->doResetMicroservice();

    stepper.setSpeed(CLOCKWISE_SPEED);
}

void Monitor::setupDisplay(){
    displayBuilder = new DisplayBuilder();

    displayBuilder->setEnable(1);
    displayBuilder->buildDisplay(PIN_DISPLAY_CLK, PIN_DISPLAY_DIO);
    displayBuilder->setupDisplay();

    displayUs = new DisplayUs(displayBuilder->getDisplay());
    
    displayUs->setEnable(1);

    displayUs->doResetMicroservice();
}

void Monitor::doorLoop() {
    static int lastTime=0;
    int curTime = millis();
    doorUs->doMicroservice();    

    // entradas
    doorUs->setDoorMode(commandUs->getDoorMode());
    doorUs->setDoorStart(commandUs->getDoorStart());
    doorUs->setReset(commandUs->getReset());

    // saída device
    // if(curTime-lastTime<1000) return;
    servo.write(doorUs->getAngle());
    // lastTime=curTime;
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

    // entradas da porta (a pedido do Fernando)
    commandUs->setDoorAction(doorUs->getAction());
    commandUs->setIsOldValue(bluetoothService->isOldValue());
    commandUs->setBluetoothData(bluetoothService->getBluetoothValue());

    // entradas da cabine
    commandUs->setCurrentFloor(cabinUs->getCurrentFloor());
    commandUs->setCabinAction(cabinUs->getCabinAction());
}

void Monitor::cabinLoop() {
    cabinUs->doMicroservice();

    cabinUs->setRequestedFloor(commandUs->getRequestedFloor());
    cabinUs->setStartCabin(commandUs->getStartCabin());
    
    if(buttons->readButton(PinInButton::CABIN_EN_MOVE)) toggleEnable ^= 1;
    if(buttons->readButton(PinInButton::CABIN_CLOCKWISE)) toggleClockwise ^= 1;

    if (cabinUs->getCabinState() == CabinState::MOVE_TO_UP || (toggleEnable && toggleClockwise)) {
        stepper.setSpeed(CLOCKWISE_SPEED);
        cabinUs->setCountModule(STEPS_CLOCKWISE);
    }
    else if (cabinUs->getCabinState() == CabinState::MOVE_TO_DOWN || (toggleEnable && !toggleClockwise)) {
        stepper.setSpeed(COUNTERCLOCKWISE_SPEED);
        cabinUs->setCountModule(STEPS_COUNTERCLOCKWISE);
    }

    if(toggleEnable)
      stepper.step(toggleClockwise ? INT_MAX : INT_MIN, true);
    else {
        if (cabinUs->getCabinState() == CabinState::WAIT_FOR_FINISH) {
            stepper.step(cabinUs->isClockwise() ? STEPPER_STEPS: -STEPPER_STEPS);
        }
    }
}

void Monitor::displayLoop(){
    displayUs->setAndar(commandUs->getCurrentFloor());
    displayUs->setUpDownStop(commandUs->getCabinAction());

    displayUs->doMicroservice();
}
void Monitor::prints() {

    if(millis()-last < DELAY) return;

    Serial.println("-------------");
    Serial.println("COMANDO");
    Serial.println("-------------");
    Serial.print("Dado: ");
    Serial.print(bluetoothService->getBluetoothValue());
    Serial.print(",\n");
    Serial.print("Estado Comando: ");
    Serial.print(getString(commandUs->getState()));
    Serial.print(",\n");
    Serial.print("Andar atual: ");
    Serial.println(commandUs->getCurrentFloor());

    Serial.println("-------------");
    Serial.println("PORTA");
    Serial.println("-------------");
    Serial.print("Modo Porta: ");
    Serial.print(getString(commandUs->getDoorMode()));
    Serial.print(",\n");
    Serial.print("Start Porta: ");
    Serial.print(commandUs->getDoorStart() ? "true" : "false");
    Serial.print(",\n");
    Serial.print("Angulo: ");
    Serial.print(doorUs->getAngle());
    Serial.print(",\n");
    Serial.print("Ação Porta: ");
    Serial.println(getString(doorUs->getAction()));
    Serial.println("-------------");
    Serial.println("CABINE");
    Serial.println("-------------");
    Serial.print("Estado Cabine: ");
    Serial.print(getString(cabinUs->getState()));
    Serial.print(",\n");
    Serial.print("Ação Cabine: ");
    Serial.print(getString(cabinUs->getCabinAction()));
    Serial.print(",\n");
    Serial.print("Start Cabin: ");
    Serial.print(cabinUs->getStartCabin() ? "true" : "false");
    Serial.print(",\n");
    Serial.print("Andar Atual: ");
    Serial.print(commandUs->getCurrentFloor());
    Serial.print(",\n");
    Serial.print("Andar Req.: ");
    Serial.print(commandUs->getRequestedFloor());
    Serial.print(",\n");
    Serial.print("Clockwise: ");
    Serial.println(getString(cabinUs->getClkwise()));

    last = millis();
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