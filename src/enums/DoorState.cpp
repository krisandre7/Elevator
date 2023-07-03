#include "DoorState.h"
#include <Arduino.h>

String getString(DoorState enumVal) {
    switch (enumVal) {
        case DoorState::S_RESET:
            return "S_RESET";
        case DoorState::S_WAIT_4_START:
            return "S_WAIT_4_START";
        case DoorState::S_SET_UP:
            return "S_SET_UP";
        case DoorState::S_COUNT_UP:
            return "S_COUNT_UP";
        case DoorState::S_SET_DOWN:
            return "S_SET_DOWN";
        case DoorState::S_COUNT_DOWN:
            return "S_COUNT_DOWN";
        case DoorState::S_NOISE:
            return "S_NOISE";
        default:
            return "UNKNOWN";
    }
}