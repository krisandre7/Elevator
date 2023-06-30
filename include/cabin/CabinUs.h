#ifndef CABIN_MICROSSERVICE_H_
#define CABIN_MICROSSERVICE_H_

#include <string>
#include <stdbool.h>

#include "Compare.h"
#include "Microservice.h"
#include "DataRegister.h"
#include "DownCounter.h"

#include "CabinState.h"
#include "CabinAction.h"

class CabinUs : public Microservice {

private:

  void setActive () {
    // Ajuste com a situação do microsserviço
    Microservice::setActive();
    active &= classActive & logicActive & startActive;
  }

  // Entradas do microsserviço
  Compare  *cmp = nullptr;
  DataRegister *reg = nullptr;
  DownCounter  *dc = nullptr;
  uint8_t requestedFloor;
  unsigned int nSteps = 0;
  int startCabin;

  /* Controle do microsserviço */
  CabinState           state;
  int             start;
  int             classActive,
                  logicActive,
                  startActive;

  /* Saída do microsserviço */
  unsigned int currentFloor = 0;
  int isFinishedReset;
  CabinAction cabinAction;
  int startStepMotor;
  bool clkwise;

public:


  /** --------------------------------------------------------------------------
   *  @brief      coConfiguração inicial do serviço.
   *  --------------------------------------------------------------------------
   */
  CabinUs () {
    // Configuração das entradas do microsserviço
    unsigned int requestedFloor = 0;
    CabinAction cabinAction = CabinAction::S_STOPPED;
    startCabin = false;

    requestedFloor = 0;

    /*! Ajusta a saída de sinalização */
    state = CabinState::S_RESET;
    nSteps = 0;
    startStepMotor = false;


    active = 1;
    classActive = 1;
    logicActive = 1;
    startActive = 1;
    clkwise = false;
    setActive ();
  }
  /** --------------------------------------------------------------------------
   * @brief      Ajusta a referência do objeto da classe DataRegister
   *
   * @param[in]  reg  -  1, Endereço do objeto DataRegister
   * ---------------------------------------------------------------------------
   */
  void setDataRegister(DataRegister *reg = nullptr) {

    if ( reg != nullptr ) {

      // Guarda a referência do objeto DataRegister
      this->reg = reg;

      // Ajusta a saída de sinalização
      classActive = 1;
      logicActive = reg->getActive();
    } else {
      // Ajusta asaída de sinalização
      classActive = 0;
    }

    setActive ();
  }

  /** --------------------------------------------------------------------------
   * @brief      Ajusta a referência do objeto da classe contador
   *
   * @param[in]  dc, Endereço do objeto do contador descrescente
   * ---------------------------------------------------------------------------
   */
  void setDownCounter(DownCounter * dc = nullptr) {

    if ( dc != nullptr ) {
      // Guarda a referência do objeto Contador
      this->dc = dc;

      // Ajusta a saída de sinalização
      classActive = 1;
      logicActive = dc->getActive();
    } else {
      // Ajusta asaída de sinalização
      classActive = 0;
    }

    setActive ();
  }

  /** --------------------------------------------------------------------------
   * @brief      Ajusta a referência do objeto da classe contador
   *
   * @param[in]  dc, Endereço do objeto do contador descrescente
   * ---------------------------------------------------------------------------
   */
  void setComparator(Compare * cmp = nullptr) {

    if ( cmp != nullptr ) {
      // Guarda a referência do objeto Contador
      this->cmp = cmp;

      // Ajusta a saída de sinalização
      classActive = 1;
      logicActive = cmp->getActive();
    } else {
      // Ajusta a saída de sinalização
      classActive = 0;
    }

    setActive ();
  }

  /** --------------------------------------------------------------------------
   *  @brief      Executa o reset do microsserviço habilitado.
   *  --------------------------------------------------------------------------
   */
  void doResetMicroservice() {

    if ( (enable == 1) && (active == 1) ) {

      // Operação de reset dos Contadores.

      if ( reg != nullptr and dc != nullptr ) {
          reg->doResetQ();
          dc->doResetQ();
          requestedFloor = 0;
      }

      // Ajusta máquina de estados para o início */
      state = CabinState::S_RESET;
      cabinAction = CabinAction::S_STOPPED;
    }
  }

  void setCurrentFloor(unsigned int currentFloor) {
    this->currentFloor = currentFloor;
  }

  void setRequestedFloor(unsigned int requestedFloor){
	  if ( isFinishedReset ) this->requestedFloor = requestedFloor;
  }

  void setStartCabin(int startCabin) {
    this->startCabin = startCabin;
  }


  /** --------------------------------------------------------------------------
   *  @brief      Execução do microsserviço de sinalização.
   *  --------------------------------------------------------------------------
   */
  void doMicroservice () {
    if ( (enable == 1) && (active == 1) ) {

      switch ( state ) {
        //0
        case CabinState::S_RESET: {
            isFinishedReset = false;
          state = CabinState::S_WAIT_FOR_RESET;

          break;
        }
        //1
        case CabinState::S_WAIT_FOR_RESET: {
          requestedFloor = 0; //bluetooth manda p nois.
          state = CabinState::MOVE_CABIN;
          break;
        }
        //2
        case CabinState::S_WAIT_FOR_START: { //Esperar pulso == 1 (start ==1)
          isFinishedReset = true;
          if ( startCabin ) {
            state = CabinState::MOVE_CABIN;
            return;
          }

          return;
        }

        //3
        case CabinState::MOVE_CABIN: { // MOVE_CABIN

            cmp->setDataA(requestedFloor);
            cmp->setDataB(currentFloor);
            cmp->setGreastLessEqualIn(1);
            cmp->doArithmetic();

            dc->doSetQ();


          if ( cmp->getGreatestThenOut() ) {
            state = CabinState::MOVE_TO_UP;
            cabinAction = CabinAction::S_TO_UP;
            return;
          } else if( cmp->getEqualOut() ) {
              startStepMotor = false;
              state = CabinState::S_WAIT_FOR_START;
              cabinAction = CabinAction::S_STOPPED;
              return;
          } else if( cmp->getLessThenOut() ) {
            state = CabinState::MOVE_TO_DOWN;
            cabinAction = CabinAction::S_TO_DOWN;
            return;
          }
          else state = CabinState::S_NOISE;

          return;
        }

        //4
        case CabinState::MOVE_TO_UP: {
          clkwise = 0;
          cabinAction = CabinAction::S_TO_UP;
          state = CabinState::WAIT_FOR_FINISH;
          break;
        }
        //5
        case CabinState::MOVE_TO_DOWN: {
          clkwise = 1;
          state = CabinState::WAIT_FOR_FINISH;
          cabinAction = CabinAction::S_TO_DOWN;
          break;
        }
        //6
        case CabinState::WAIT_FOR_FINISH: {

            nSteps = dc->getQ();
            startStepMotor = true;


            if ( cabinAction == CabinAction::S_TO_UP and nSteps == 0 ) {
                currentFloor++;
                reg->setData(currentFloor);
                reg->setLoadEnable(1);
                reg->doRegister();
                cmp->setDataA(requestedFloor);
                cmp->setDataB(currentFloor);
                cmp->setGreastLessEqualIn(1);
                cmp->doArithmetic();

                if ( cmp->getEqualOut() ) {
                    dc->setCountEnable(false);
                    cabinAction = CabinAction::S_STOPPED;
                    state = CabinState::MOVE_CABIN;
                    return;
                } else {
                    dc->doSetQ();
                    return;
                }


            }

            else if ( cabinAction == CabinAction::S_TO_DOWN and nSteps == 0 ) {
                currentFloor--;
                reg->setData(currentFloor);
                reg->setLoadEnable(1);
                reg->doRegister();
                cmp->setDataA(requestedFloor);
                cmp->setDataB(currentFloor);
                cmp->setGreastLessEqualIn(1);
                cmp->doArithmetic();

                if ( cmp->getEqualOut() ) {
                    dc->setCountEnable(false);
                    cabinAction = CabinAction::S_STOPPED;
                    state = CabinState::MOVE_CABIN;
                    return;
                } else {
                    dc->doSetQ();
                    return;
                }


            }

            else {
                dc->setCountEnable(true);
                dc->doCount();
                return;
            }

        }

        //7
        case CabinState::S_NOISE: {
          state = CabinState::S_RESET;
          return;
        }

        default:
          // Detecção de ruído.
          state = CabinState::S_NOISE;
          break;
      }
    }
  }


  /*! --------------------------------------------------------------------------
   *  @brief      Adquire o valor da saída de sinalização.
   *
   *  @return     Valor da saída de sinalização.
   * ---------------------------------------------------------------------------
   */
  int getActive() {
    return active;
  }

  CabinState getState() {
    return this->state;
  }

  CabinAction getCabinAction() {
    return this->cabinAction;
  }

  unsigned int getCurrentFloor() {
    return this->currentFloor;
  }

  int getStartCabin() {
    return this->startCabin;
  }

  int getSteps() {
    return this->nSteps;
  }

  int getIsFinishedReset() {
    return this->isFinishedReset;
  }

  int getStartStepMotor() {
	  return this->startStepMotor;
  }

  bool getClkwise() {
      return clkwise;
  }
};

#endif // COMMAND_MICROSSERVICE_H_
