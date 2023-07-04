#pragma once

#include <Arduino.h>
#include <Button.h>

enum class PinInButton : uint8_t{
  RESET = 2,
  CABIN_EN_MOVE = 0,
  CABIN_CLOCKWISE = 4
};

class ButtonService{
private:
  Button butReset, butCabinEnMv, butCabinClk; 

protected:
  ButtonService();

public:
  static ButtonService *singleton_;
  // Singletons should not be cloneable.
  ButtonService(ButtonService &other) = delete;

  // Singletons should not be assignable.
  void operator=(const ButtonService &) = delete;

  // Static ButtonService Instance
  static ButtonService *GetInstance();

  //0:released, 1:pressed
  bool readButton(PinInButton pin);
};