#ifndef COMMAND_MICROSSERVICE_H_
#define COMMAND_MICROSSERVICE_H_

#include <assert.h>

#include <string>

#include "AsciiToHexa.h"
#include "DataRegister.h"
#include "DoorUs.h"
#include "Microservice.h"

// Estados:
enum class CommandState : int {
    S_RESET,                 // Estado 00 (Reset)
    S_WAIT_FOR_RESET,        // Estado 01 (Espera reset das historias terminar)
    S_TEST_REQUEST,          // Estado 02 (Requisição de teste)
    S_WAIT_FOR_TEST,         // Estado 03 (Espera fim de teste)
    S_WAIT_FLOOR_REQUEST,    // Estado 04 (Espera requisição de andar)
    S_SAVE_REQUESTED_FLOOR,  // Estado 05 (Salva requisição de andar)
    S_MOVE_DOOR,             // Estado 06 (Fecha se estiver aberta e vice-versa)
    S_CLOSE_DOOR,            // Estado 07 (Solicita fechamento da porta)
    S_OPEN_DOOR,             // Estado 08 (Solicita abertura da porta)
    S_WAIT_FINISH_DOOR,      // Estado 09 (Espera fim de movimento da porta)
    S_MOVE_CABIN,            // Estado 10 (Solicita movimento da cabine)
    S_WAIT_MOVING_CABIN,     // Estado 11 (A) (Espera movimento da cabine)
    S_NOISE                  // Estado 12 (B) (Ruído)
};

// Estados da Cabine:
enum class CabinState : int {
    S_TO_UP,    // Estado 00 (Subindo)
    S_TO_DOWN,  // Estado 01 (Descendo)
    S_STOPPED,  // Estado 02 (Parado)
    S_NOISE     // Estado 03 (Ruído)
};

class CommandUs : public Microservice {
   private:
    void setActive() {
        // Ajuste com a situação do microsserviço
        Microservice::setActive();
        active &= classActive & logicActive & startActive;
    }

    // Entradas do microsserviço
    DataRegister *reg = nullptr;  // Registrador de andar solicitado
    AsciiToHexa *a2h =
        nullptr;        // Conversor de dado ascii (bluetooth) para hexadecimal
    uint8_t bluetoothData;  // Dados recebidos via bluetooth (ascii)
    bool resetIsRunning;  // Flag de reset em execução (1 = em execução, 0 = não
                          // em execução)
    bool testIsRunning;  // Flag de teste em execução (1 = em execução, 0 = não
                         // em execução)
    uint8_t currentFloor;   // Andar atual [0,2]
    DoorAction doorAction;  // Estado da porta (0 = fechada, 1 = em movimento, 2
                            // = aberta)
    CabinState
        cabinState;  // Estado da cabine (0 = subindo, 1 = descendo, 2 = parado)

    /* Controle do microsserviço */
    CommandState state;  // Estado atual do microsserviço
    int classActive,     // Flag de criação de classes
        logicActive,     // Flag de active de componentes camada logica
        startActive;     // startActive

    /* Saída do microsserviço */
    unsigned int requestedFloor;  // Andar solicitado
    bool startReset;              // Flag de início de reset
    bool startTest;               // Flag de início de teste
    bool doorStart;               // Flag de início de movimento da porta
    bool startCabin;              // Flag de início de movimento da cabine
    DoorMode doorMode;            // 0 = fechar porta, 1 = abrir porta
    bool isOldValue;

   public:
    /** --------------------------------------------------------------------------
     *  @brief      Configuração inicial do serviço.
     *  --------------------------------------------------------------------------
     */
    CommandUs() {
        // Configuração das entradas do microsserviço
        bluetoothData = '\0';
        doorMode = DoorMode::MODE_CLOSE_DOOR;
        testIsRunning = false;
        currentFloor = 48;
        doorAction = DoorAction::ACT_NOISE;
        cabinState = CabinState::S_NOISE;

        /*! Ajusta a saída de sinalização */
        state = CommandState::S_RESET;
        requestedFloor = 3;
        startReset = false;
        startTest = false;
        doorStart = false;
        startCabin = false;

        active = 1;
        classActive = 1;
        logicActive = 1;
        startActive = 1;
        setActive();
    }

    void setDataRegister(DataRegister *reg = nullptr) {
        if (reg != nullptr) {
            // Guarda a referência do objeto DataRegister
            this->reg = reg;

            // Ajusta a saída de sinalização
            classActive = 1;
            logicActive = reg->getActive();
        } else {
            // Ajusta asaída de sinalização
            classActive = 0;
        }

        setActive();
    }

    void setAsciiToHexa(AsciiToHexa *a2h = nullptr) {
        if (a2h != nullptr) {
            // Guarda a referência do objeto Codificador
            this->a2h = a2h;

            // Ajusta a saída de sinalização
            classActive = 1;
            logicActive = a2h->getActive();
        } else {
            // Ajusta asaída de sinalização
            classActive = 0;
        }

        setActive();
    }

    // /**
    // --------------------------------------------------------------------------
    //  *  @brief      Executa o reset do microsserviço habilitado.
    //  *
    //  --------------------------------------------------------------------------
    //  */
    void doResetMicroservice() {
        if ((enable == 1) && (active == 1)) {
            // Operação de reset dos Contadores.
            reg->doResetQ();

            // Ajusta máquina de estados para requisitar teste */
            state = CommandState::S_TEST_REQUEST;
        }
    }

    /** --------------------------------------------------------------------------
     *  @brief      Execução do microsserviço de sinalização.
     *  --------------------------------------------------------------------------
     */
    void doMicroservice() {
        if ((enable == 1) && (active == 1)) {
            switch (state) {
                case CommandState::S_RESET: {  // reset all
                    reg->doResetQ();

                    startReset = true;

                    if (resetIsRunning) {
                        startReset = false;
                        state = CommandState::S_WAIT_FOR_RESET;
                    }

                    return;
                }

                case CommandState::S_WAIT_FOR_RESET: {  // wait reset all
                    if (!resetIsRunning) state = CommandState::S_TEST_REQUEST;

                    return;
                }

                case CommandState::S_TEST_REQUEST: {
                    startTest = true;

                    if (testIsRunning) {
                        startTest = false;
                        state = CommandState::S_WAIT_FOR_TEST;
                    }

                    return;
                }

                case CommandState::S_WAIT_FOR_TEST: {
                    if (!testIsRunning)
                        state = CommandState::S_WAIT_FLOOR_REQUEST;

                    return;
                }

                case CommandState::S_WAIT_FLOOR_REQUEST: {
                    // Serial.print("Ó o dado: ");
                    // Serial.println(bluetoothData);
                    if (!isOldValue && bluetoothData >= '0' && bluetoothData <= '2')
                        state = CommandState::S_SAVE_REQUESTED_FLOOR;
                    return;
                }

                case CommandState::S_SAVE_REQUESTED_FLOOR: {
                    if (a2h == nullptr) {
                        state = CommandState::S_NOISE;
                        return;
                    }

                    a2h->setAscii(bluetoothData);
                    a2h->doEncoder();

                    if (a2h->getActive()) {
                        unsigned data = a2h->getHexa();
                        reg->setData(data);
                        reg->setLoadEnable(true);
                        reg->doRegister();

                        if (!reg->getActive())
                            state = CommandState::S_NOISE;
                        else {
                            requestedFloor = reg->getQ();
                            
                            if (currentFloor != requestedFloor)
                                state = CommandState::S_MOVE_DOOR;
                            else
                                state = CommandState::S_WAIT_FLOOR_REQUEST;
                            //     state = CommandState::S_MOVE_CABIN;
                            // else
                                // state = CommandState::S_MOVE_DOOR;
                        }
                    } else
                        state = CommandState::S_NOISE;

                    return;
                }

                case CommandState::S_MOVE_DOOR: {
                    if (currentFloor == requestedFloor)
                        state = CommandState::S_OPEN_DOOR;
                    else
                        state = CommandState::S_CLOSE_DOOR;

                    return;
                }

                case CommandState::S_CLOSE_DOOR: {
                    if (doorAction == DoorAction::ACT_CLOSED_DOOR) {
                        state = CommandState::S_WAIT_FINISH_DOOR;
                        return;
                    }

                    doorStart = true;
                    doorMode = DoorMode::MODE_CLOSE_DOOR;

                    if (doorAction == DoorAction::ACT_MOVING_DOOR)
                        state = CommandState::S_WAIT_FINISH_DOOR;

                    return;
                }

                case CommandState::S_OPEN_DOOR: {
                    if (doorAction == DoorAction::ACT_OPENED_DOOR) {
                        state = CommandState::S_WAIT_FINISH_DOOR;
                        return;
                    }

                    doorStart = true;
                    doorMode = DoorMode::MODE_OPEN_DOOR;

                    if (doorAction == DoorAction::ACT_MOVING_DOOR)
                        state = CommandState::S_WAIT_FINISH_DOOR;

                    return;
                }

                case CommandState::S_WAIT_FINISH_DOOR: {
                    doorStart = false;
                    if (doorAction == DoorAction::ACT_OPENED_DOOR) {
                        state = CommandState::S_WAIT_FLOOR_REQUEST;
                    } else if (doorAction == DoorAction::ACT_CLOSED_DOOR) {
                        state = CommandState::S_MOVE_CABIN;
                    }
                    return;
                }

                case CommandState::S_MOVE_CABIN: {
                    startCabin = true;

                    if (cabinState != CabinState::S_STOPPED)
                        state = CommandState::S_WAIT_MOVING_CABIN;

                    return;
                }

                case CommandState::S_WAIT_MOVING_CABIN: {
                    if (cabinState == CabinState::S_STOPPED) {
                        state = CommandState::S_OPEN_DOOR;
                        startCabin = false;
                    }
                    return;
                }

                case CommandState::S_NOISE: {
                    state = CommandState::S_RESET;
                    return;
                }

                default:
                    // Detecção de ruído.
                    state = CommandState::S_NOISE;
                    break;
            }
        }
    }

    int getActive() { return active; }

        void setBluetoothData(int bluetoothData) {
        this->bluetoothData = bluetoothData;
    }

    void setTestIsRunning(unsigned int testIsRunning) {
        if (testIsRunning == 0 or testIsRunning == 1)
            this->testIsRunning = testIsRunning;
        else
            this->testIsRunning = false;
    }

    bool getTestIsRunning() { return testIsRunning; }

    void setCurrentFloor(int currentFloor) {
        this->currentFloor = currentFloor;
    }

    void setDoorAction(DoorAction doorAction) { this->doorAction = doorAction; }

    void setCabinState(CabinState cabinState) { this->cabinState = cabinState; }

    CommandState getState() { return this->state; }

    int getDoorStart() { return this->doorStart; }

    void setIsOldValue(bool oldValue) { this->isOldValue = oldValue; }

    DoorMode getDoorMode() { return this->doorMode; }

    bool getReset() { return this->startReset; }

    int getCurrentFloor() { return this->currentFloor; }

    int getRequestedFloor() { return this->requestedFloor; }
};

#endif  // COMMAND_MICROSSERVICE_H_
