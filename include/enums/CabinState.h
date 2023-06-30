#pragma once

enum class CabinState : int {
  S_RESET,                  // Estado 00
  S_WAIT_FOR_RESET,         // Estado 01
  S_WAIT_FOR_START,         // Estado 02
  MOVE_CABIN,               // Estado 03
  MOVE_TO_UP,               // Estado 04
  MOVE_TO_DOWN,             // Estado 05
  WAIT_FOR_FINISH,          // Estado 06
  S_NOISE                   // Estado 07
};