#include <ESP32Servo.h>
#define PIN_SG90 32  // Output pin used
// #include "UpDownCounter.h"
// #include "DoorLogicBuilder.h"
// #include "DoorUs.h"

#define BIT_SIZE 7
#define MODULE_SIZE 90
#define DELAY 2000

// DoorBuilder* fac = NULL;
// DoorUs* doorUs = NULL;
// UpDownCounter* upDownCounter = NULL;
double clk, last;
int doorPulse, modePort, angle;

Servo sg90;

void setup() {
  Serial.begin(9600);
  sg90.setPeriodHertz(50);           // PWM frequency for SG90
  sg90.attach(PIN_SG90, 500, 2400);  // Minimum and maximum pulse width (in µs) to go from 0° to 180

  // fac = new DoorBuilder;
  // fac->setEnable(1);

  // fac->buildUpDownCounter(BIT_SIZE);
  // fac->setupUpDownCounter(MODULE_SIZE);

  // doorUs = new DoorUs;
  // doorPulse = 0;
  // modePort = 0;
  // angle = 0;

  // upDownCounter = fac->getUpDownCounter();

  // doorUs->setUpDownCounter(upDownCounter);

  // doorUs->doResetMicroservice();
}

void loop() {
  // if (Serial.available()) {
  //   // Lê dois ints do serial no formato "int,int"
  //   // onde o primeiro é o doorPulse e o segundo é o modePort
  //   String str = Serial.readStringUntil('\n');
  //   int index = str.indexOf(',');
  //   doorPulse = str.substring(0, index).toInt();
  //   modePort = str.substring(index + 1).toInt();

  //   Serial.print(doorPulse);
  //   Serial.print(", ");
  //   Serial.println(modePort);
  // }

  // if (doorPulse) {
  //   modePort ? doorUs->openDoor() : doorUs->closeDoor();
  // }

  // angle = doorUs->getAngle();

  // sg90.write(angle);
  // delay(30);

  // doorUs->doMicroservice();

  // doorPulse = 0;

  // if(millis()-last < DELAY) return;


  sg90.write(90);
  // last=millis();
  // rotation from 0 to 180°
  // for (int pos = 0; pos <= 180; pos += 1) {
  //   sg90.write(pos);
  //   delay(30);
  // }
  // // Rotation from 180° to 0
  // for (int pos = 180; pos >= 0; pos -= 1) {
  //   sg90.write(pos);
  //   delay(30);
  // }
}