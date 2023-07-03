#pragma once
#include <Arduino.h>

// Ordem para a porta abrir ou fechar
enum class DoorMode: int{
  MODE_CLOSE_DOOR,
  MODE_OPEN_DOOR
};

String getString(DoorMode enumVal);