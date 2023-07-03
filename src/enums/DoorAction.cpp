#include "DoorAction.h"
#include <Arduino.h>

String getString(DoorAction enumVal) {
    switch (enumVal) {
        case DoorAction::ACT_CLOSED_DOOR:
            return "ACT_CLOSED_DOOR";
        case DoorAction::ACT_MOVING_DOOR:
            return "ACT_MOVING_DOOR";
        case DoorAction::ACT_OPENED_DOOR:
            return "ACT_OPENED_DOOR";
        case DoorAction::ACT_NOISE:
            return "ACT_NOISE";
        default:
            return "UNKNOWN";
    }
}