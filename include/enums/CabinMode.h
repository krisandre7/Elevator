#pragma once
#include <Arduino.h>

enum class CabinMode: int {
    CLOCKWISE,
    COUNTERCLOCKWISE,
};

String getString(CabinMode enumVal);