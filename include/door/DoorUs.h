#include <Arduino.h>
#include "Microservice.h"
#include "UpDownCounter.h"
#pragma once

enum class DoorState : int {
  S_RESET = 0,
  S_WAIT_4_START = 1,
  S_SET_UP = 2,
  S_COUNT_UP = 3,
  S_SET_DOWN = 4,
  S_COUNT_DOWN = 5,
  S_NOISE = 6
};

//Estados da Porta:
enum class DoorAction: int {
  ACT_CLOSED_DOOR = 0, // Estado 00 (Porta fechada)
  ACT_MOVING_DOOR,     // Estado 01 (Porta em movimento)
  ACT_OPENED_DOOR,     // Estado 02 (Porta aberta)
  ACT_NOISE            // Estado 03 (Ruído)
};

enum class DoorMode: int{
  MODE_CLOSE_DOOR = 0,
  MODE_OPEN_DOOR = 1
};

class DoorUs : public Microservice {

public:
  DoorUs() {
    /*! Configura as entradas do microsserviço */
    doorStart = 0;
    upDownCounter = NULL;
    doorMode = DoorMode::MODE_CLOSE_DOOR;
    enable_in = 0;
    resetIn = 0;

    /*! Ajusta a saída de sinalização */
    state = DoorState::S_RESET;
    classActive = 1;
    logicActive = 1;
    startActive = 1;
    setActive();
    reset_out = 0;
    set = 0;
    up_down = countType::up;
    count_enable = 0;
  }

  void setUpDownCounter(UpDownCounter* upDownCounter) {
    if (upDownCounter != NULL) {
      /*! Guarda a referência do objeto Contador */
      DoorUs::upDownCounter = upDownCounter;

      /*! Ajusta asaída de sinalização */
      classActive = 1;
      logicActive = upDownCounter->getActive();
      angle = upDownCounter->getQ();
    } else {
      /*! Ajusta asaída de sinalização */
      classActive = 0;
    }
    setActive();
  }

  void doResetMicroservice() {
    if ((getEnable() == 1) && (active == 1)) {
      /*! Operação de reset do Contador. */
      upDownCounter->doResetQ();

      state = DoorState::S_RESET;
    }
  }

  DoorAction doAction() {
    if (resetIn == 1) {
      // Modo limpa
      return DoorAction::ACT_CLOSED_DOOR;
    } else if (state == DoorState::S_COUNT_UP || state == DoorState::S_COUNT_DOWN) {
      // Abrindo porta
      return DoorAction::ACT_MOVING_DOOR;
    } else if (angle == 0) {
      // Porta fechada
      return DoorAction::ACT_CLOSED_DOOR;
    } else if (angle == 90) {
      // Porta aberta
      return DoorAction::ACT_OPENED_DOOR;
    } else {
      // Erro
      return DoorAction::ACT_NOISE;
    }
  }

  void doMicroservice() {
    if (Microservice::enable == 1 && active == 1) {

      switch (state) {
        case DoorState::S_RESET:
          upDownCounter->doResetQ();
          upDownCounter->setCountMode(countType::down);
          state = DoorState::S_WAIT_4_START;
          break;
        case DoorState::S_WAIT_4_START:
          if (doorStart == 1 && doorMode == DoorMode::MODE_CLOSE_DOOR && angle == 90) {
            state = DoorState::S_SET_DOWN;
          } else if (doorStart == 1 && doorMode == DoorMode::MODE_OPEN_DOOR && angle == 0) {
            state = DoorState::S_SET_UP;
          }
          break;
        case DoorState::S_SET_UP:
          if (DoorUs::doorStart) DoorUs::doorStart = 0;

          state = DoorState::S_COUNT_UP;
          upDownCounter->doResetQ();
          upDownCounter->setCountMode(countType::up);
          break;
        case DoorState::S_SET_DOWN:
          if (DoorUs::doorStart) DoorUs::doorStart = 0;

          state = DoorState::S_COUNT_DOWN;

          upDownCounter->doSetQ();
          upDownCounter->setCountMode(countType::down);
          break;
        case DoorState::S_COUNT_UP:
          if (angle == 89) {
            state = DoorState::S_WAIT_4_START;
          }
          upDownCounter->doCount();
          break;
        case DoorState::S_COUNT_DOWN:
          if (angle == 1) {
            state = DoorState::S_WAIT_4_START;
          }
          upDownCounter->doCount();
          break;
        default:
          state = DoorState::S_NOISE;
      }

      action = doAction();
      angle = upDownCounter->getQ();
    }
  }

  DoorAction getAction() {
    return action;
  }

  int getActive() {
    return active;
  }

  int getAngle() {
    return angle;
  }

  DoorAction getDoorAction() {
    return action;
  }

  void setDoorStart(bool start) {
    doorStart = start;
  }


  void setDoorMode(DoorMode mode) {
    doorMode = mode;
  }

  void setReset(int reset) {
    resetIn = reset;
  }

private:
  void setActive() {
    /*! Ajuste com a situação do microsserviço */
    Microservice::setActive();

    active &= classActive & logicActive & startActive;
  }

  /*! Entradas do microsserviço */
  UpDownCounter* upDownCounter;
  DoorMode doorMode;
  int enable_in;
  int resetIn;
  int angle;

  /*! Controle do microsserviço */
  DoorState state;
  bool doorStart;
  int classActive,
    logicActive,
    startActive;

  /*! Saída do microsserviço */
  // o enable vem do Microservice
  int reset_out;
  int set;
  countType up_down;
  int count_enable;
  DoorAction action;
};