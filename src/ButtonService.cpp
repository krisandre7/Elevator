#include <Arduino.h>
// #include <Button.h>
#include "ButtonService.h"

ButtonService* ButtonService::singleton_ = nullptr;

ButtonService::ButtonService():
  butReset((uint8_t) PinInButton::RESET),
  butCabinEnMv((uint8_t) PinInButton::CABIN_EN_MOVE),
  butCabinClk((uint8_t) PinInButton::CABIN_CLOCKWISE)
{
  butReset.begin();
  butCabinEnMv.begin();
  butCabinClk.begin();
}

ButtonService* ButtonService::GetInstance(){
  if(singleton_ == nullptr) singleton_ = new ButtonService();
  return singleton_;
}

bool ButtonService::readButton(PinInButton pin){
  switch(pin){
    case PinInButton::RESET:
      return butReset.pressed();
    case PinInButton::CABIN_EN_MOVE:
      return butCabinEnMv.pressed();
    case PinInButton::CABIN_CLOCKWISE:
      return butCabinClk.pressed();
    default:
      return false;
  }
}