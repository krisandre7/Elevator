#include "CabinState.h"
#include <Arduino.h>

String getString(CabinState enumVal) {
    switch (enumVal) {
        case CabinState::S_RESET:
            return "S_RESET";
        case CabinState::S_WAIT_FOR_RESET:
            return "S_WAIT_FOR_RESET";
        case CabinState::S_WAIT_FOR_START:
            return "S_WAIT_FOR_START";
        case CabinState::MOVE_CABIN:
            return "MOVE_CABIN";
        case CabinState::MOVE_TO_UP:
            return "MOVE_TO_UP";
        case CabinState::MOVE_TO_DOWN:
            return "MOVE_TO_DOWN";
        case CabinState::WAIT_FOR_FINISH:
            return "WAIT_FOR_FINISH";
        case CabinState::S_NOISE:
            return "S_NOISE";
        default:
            return "UNKNOWN";
    }
}