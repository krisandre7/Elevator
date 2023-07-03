#include "DisplayState.h"
#include <Arduino.h>

String getString(DisplayState enumVal) {
    switch (enumVal) {
        case DisplayState::S_RESET:
            return "S_RESET";
        case DisplayState::S_TEST:
            return "S_TEST";
        case DisplayState::S_SHOW:
            return "S_SHOW";
        case DisplayState::S_NOISE:
            return "S_NOISE";
        default:
            return "UNKNOWN";
    }
}