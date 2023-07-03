#include "DoorMode.h"
#include <Arduino.h>

String getString(DoorMode enumVal) {
    switch (enumVal) {
        case DoorMode::MODE_CLOSE_DOOR:
            return "MODE_CLOSE_DOOR";
        case DoorMode::MODE_OPEN_DOOR:
            return "MODE_OPEN_DOOR";
        default:
            return "UNKNOWN";
    }
}