#pragma once
#include <Arduino.h>

// Estados da Porta que saem para o comando
enum class DoorAction: int {
  ACT_CLOSED_DOOR, // Estado 00 (Porta fechada)
  ACT_MOVING_DOOR,     // Estado 01 (Porta em movimento)
  ACT_OPENED_DOOR,     // Estado 02 (Porta aberta)
  ACT_NOISE            // Estado 03 (Ruído)
};

String getString(DoorAction enumVal);