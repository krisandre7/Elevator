#pragma once
#include <Arduino.h>

// Estados da Cabine que saem para o comando
enum class CabinAction : int {
    S_TO_UP,    // Estado 00 (Subindo)
    S_TO_DOWN,  // Estado 01 (Descendo)
    S_STOPPED,  // Estado 02 (Parado)
    S_NOISE     // Estado 03 (Ruído)
};

String getString(CabinAction enumVal);