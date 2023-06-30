#pragma once

// Estado interno do Comando
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