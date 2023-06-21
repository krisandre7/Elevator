//This example code is in the Public Domain (or CC0 licensed, at your option.)
//By Evandro Copercini - 2018
//
//This example creates a bridge between Serial and Classical Bluetooth (SPP)
//and also demonstrate that SerialBT have the same functionalities of a normal Serial

#include "BluetoothSerial.h"

#include "DataRegister.h"
#include "AsciiToHexa.h"
#include "CommandBuilder.h"
#include "CommandUs.h"


int finishTime = 25;

// Criação estática dos objetos de microserviço e fábrica
CommandBuilder *fac = nullptr;
CommandUs *cmdUs = nullptr;

DataRegister *reg = nullptr;
AsciiToHexa *a2h = nullptr;

// Definição da entrada
uint8_t ascii = ' ';
int testIsRunnning = false;
uint8_t bluetoothData = 0;
unsigned int currentFloor = 0;
DoorAction doorAction = DoorAction::ACT_CLOSED_DOOR;
CabinState cabinState = CabinState::S_STOPPED;

// Definição das saidas
int state;
int startTest;
int startDoor;
int startCabin;
unsigned requestedfloor;

bool active;

//#define USE_PIN // Uncomment this to use PIN during pairing. The pin is specified on the line below
const char *pin = "1234";  // Change this to more secure PIN.

String device_name = "ESP32-BT-Slave";

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin(device_name);  //Bluetooth device name
  Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());
  //Serial.printf("The device with name \"%s\" and MAC address %s is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str(), SerialBT.getMacString()); // Use this after the MAC method is implemented

#ifdef USE_PIN
  SerialBT.setPin(pin);
  Serial.println("Using PIN");
#endif

  // Criação da fábrica de lógica
  fac = new CommandBuilder();

  // Habilitação da fábrica de lógica
  fac->setEnable(true);

  // Criação das lógicas na fábrica
  fac->buildDataRegister();
  active = fac->getActive();

  fac->buildAsciiToHexa();
  active = fac->getActive();

  // Configuração das lógicas na fábrica
  fac->setupDataRegister();
  active = fac->getActive();

  fac->setupAsciiToHexa();
  active = fac->getActive();

  // Criação de microsserviço
  cmdUs = new CommandUs();

  // Obtenção das lógicas da fábrica
  reg = fac->getDataRegister();
  a2h = fac->getAsciiToHexa();

  // Configuração dos microsserviços
  cmdUs->setDataRegister(reg);
  active = cmdUs->getActive();

  cmdUs->setAsciiToHexa(a2h);
  active = cmdUs->getActive();

  // Habilitação e reset do microsserviço
  cmdUs->setEnable(1);
  active = cmdUs->getActive();
  printf("act7: %d\n\n\r", active);

  cmdUs->doResetMicroservice();
}

int t = 0;


void loop() {
  if (Serial.available()) { //enviar dados bluetooth
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) { //receber dados bluetooth
    bluetoothData = SerialBT.read();
    Serial.write(bluetoothData);
    cmdUs->setBluetoothData(bluetoothData);
  }

  

  cmdUs->setDoorAction(doorAction);
  cmdUs->setCabinState(cabinState);
  cmdUs->setCurrentFloor(currentFloor);
  
  cmdUs->doMicroservice();
  
  requestedfloor = cmdUs->getRequestedFloor();
  startTest = cmdUs->getStartTest();
  startDoor = cmdUs->getStartDoor();
  startCabin = cmdUs->getStartCabin();
  state = cmdUs->getState();

  delay(20);
}
