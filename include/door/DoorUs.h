#include <Arduino.h>
#include "Microservice.h"
#include "UpDownCounter.h"

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
    start = 0;
    upDownCounter = NULL;
    doorMode = 0;
    enable_in = 0;
    reset_in = 0;

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

  void setStart(int start) {
    if ((start >= 0) && (start <= 1)) {
      /*! Guarda a entrada de controle. */
      DoorUs::start = start;

      /*! Ajusta a saída de sinalização. */
      startActive = 1;
    } else {
      /*! Ajusta a saída de sinalização. */
      startActive = 0;
    }
    setActive();
  }

  int get_out_reset(int reset, DoorState target, DoorState current) {
    if (reset == 1) {
      return 0;
    } else {
      if (current == target) {
        return 1;
      } else {
        return 0;
      }
    }
  }

  int get_out_count(int reset, DoorState current) {
    if (reset == 1) {
      return 0;
    } else {
      if (current == DoorState::S_COUNT_DOWN || current == DoorState::S_COUNT_UP) {
        return 1;
      } else {
        return 0;
      }
    }
  }

  int S_get_outet(int reset, DoorState current) {
    if (reset == 1) {
      return 0;
    } else {
      if (current == DoorState::S_SET_UP || current == DoorState::S_SET_DOWN) {
        return 1;
      } else {
        return 0;
      }
    }
  }

  countType get_out_up_down(int reset, DoorState current) {
    if (reset) {
      return countType::down;
    } else {
      if (current == DoorState::S_SET_UP || current == DoorState::S_COUNT_UP) {
        return countType::up;
      } else {
        return countType::down;
      }
    }
  }

  DoorAction ACT_get_oution(int reset, DoorState current, int angle) {
    if (reset == 1) {
      // Modo limpa
      return DoorAction::ACT_CLOSED_DOOR;
    } else if (current == DoorState::S_COUNT_UP || current == DoorState::S_COUNT_DOWN) {
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
          state = DoorState::S_WAIT_4_START;
          break;
        case DoorState::S_WAIT_4_START:
          if (start == 1 && doorMode == 0 && angle == 90) {
            state = DoorState::S_SET_DOWN;
          } else if (start == 1 && doorMode == 1 && angle == 0) {
            state = DoorState::S_SET_UP;
          }
          break;
        case DoorState::S_SET_UP:
          if (DoorUs::start) DoorUs::start = 0;

          state = DoorState::S_COUNT_UP;
          upDownCounter->doResetQ();
          upDownCounter->setCountMode(countType::up);
          break;
        case DoorState::S_SET_DOWN:
          if (DoorUs::start) DoorUs::start = 0;

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

      reset_in = get_out_reset(reset_in, DoorState::S_RESET, state);

      action = ACT_get_oution(reset_in, state, angle);

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

  void setDoorMode(int value) {
    doorMode = value;
  }

  String getString() {
    String stringState= String((int) state);
    return stringState;
  }

  void openDoor() {
    setStart(1);
    setDoorMode(1);
  }

  void closeDoor() {
    setStart(1);
    setDoorMode(0);
  }

private:
  void setActive() {
    /*! Ajuste com a situação do microsserviço */
    Microservice::setActive();

    active &= classActive & logicActive & startActive;
  }

  /*! Entradas do microsserviço */
  UpDownCounter* upDownCounter;
  bool doorMode;
  int enable_in;
  int reset_in;
  int angle;

  /*! Controle do microsserviço */
  DoorState state;
  int start;
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