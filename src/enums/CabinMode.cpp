#include "CabinMode.h"
#include <Arduino.h>

String getString(CabinMode enumVal) {
    switch (enumVal) {
        case CabinMode::CLOCKWISE:
            return "CLOCKWISE";
        case CabinMode::COUNTERCLOCKWISE:
            return "COUNTERCLOCKWISE";
        default:
            return "UNKNOWN";
    }
}