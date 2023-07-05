#pragma once

#include <Arduino.h>
#include <TM1637Display.h>
#include "CabinAction.h"

#define QTSEGMENTOS 4
#define PIN_DISPLAY_CLK 25
#define PIN_DISPLAY_DIO 26

class DisplayDevice{
public:
  DisplayDevice():display(new TM1637Display(PIN_DISPLAY_CLK,PIN_DISPLAY_DIO)){
    for(int i=0;i<QTSEGMENTOS;i++) aux[i]=0x00;
  }

  void setup(int brightness){
    display->setBrightness(brightness);
  }

  uint8_t* getData(){
    return data;
  }

  void setData(CabinAction updownstop,int andar){
    data[0]=data[1]=data[3]=0x00; //0
    if(updownstop==CabinAction::S_TO_DOWN) data[1] = SEG_E | SEG_D | SEG_C;
    else if(updownstop==CabinAction::S_TO_UP) data[1] = SEG_F | SEG_A | SEG_B;
    data[2] = display->encodeDigit(andar); //0
  }

  void setAllDigits(int value=0xff){
    for(int i=0;i<QTSEGMENTOS;i++) data[i]=value;
  }

  void printDataDisplay(){
    if(isDataChanged()){
      display->clear();
      display->setSegments(data);
    } 
  }

  int isDataChanged(){
    int inequal=0;
    for(int i=0;i<QTSEGMENTOS;i++){
      if(data[i]!=aux[i]) inequal=1;
      aux[i]=data[i];
    }
    return inequal;
  }

private:
  TM1637Display* display;
  uint8_t data[QTSEGMENTOS],aux[QTSEGMENTOS];
};