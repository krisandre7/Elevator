#include "CommandState.h"
#include <Arduino.h>

String getString(CommandState enumVal) {
    switch (enumVal) {
        case CommandState::S_RESET:
            return "S_RESET";
        case CommandState::S_WAIT_FOR_RESET:
            return "S_WAIT_FOR_RESET";
        case CommandState::S_TEST_REQUEST:
            return "S_TEST_REQUEST";
        case CommandState::S_WAIT_FOR_TEST:
            return "S_WAIT_FOR_TEST";
        case CommandState::S_WAIT_FLOOR_REQUEST:
            return "S_WAIT_FLOOR_REQUEST";
        case CommandState::S_SAVE_REQUESTED_FLOOR:
            return "S_SAVE_REQUESTED_FLOOR";
        case CommandState::S_MOVE_DOOR:
            return "S_MOVE_DOOR";
        case CommandState::S_CLOSE_DOOR:
            return "S_CLOSE_DOOR";
        case CommandState::S_OPEN_DOOR:
            return "S_OPEN_DOOR";
        case CommandState::S_WAIT_FINISH_DOOR:
            return "S_WAIT_FINISH_DOOR";
        case CommandState::S_MOVE_CABIN:
            return "S_MOVE_CABIN";
        case CommandState::S_WAIT_MOVING_CABIN:
            return "S_WAIT_MOVING_CABIN";
        case CommandState::S_NOISE:
            return "S_NOISE";
        default:
            return "UNKNOWN";
    }
}