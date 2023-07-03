#include "CabinAction.h"
#include <Arduino.h>

String getString(CabinAction enumVal) {
    switch (enumVal) {
        case CabinAction::S_TO_UP:
            return "S_TO_UP";
        case CabinAction::S_TO_DOWN:
            return "S_TO_DOWN";
        case CabinAction::S_STOPPED:
            return "S_STOPPED";
        case CabinAction::S_NOISE:
            return "S_NOISE";
        default:
            return "UNKNOWN";
    }
}