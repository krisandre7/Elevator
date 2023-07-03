#pragma once
#include <Arduino.h>

enum class DoorState : int {
  S_RESET,
  S_WAIT_4_START,
  S_SET_UP,
  S_COUNT_UP,
  S_SET_DOWN,
  S_COUNT_DOWN,
  S_NOISE
};

String getString(DoorState enumVal);