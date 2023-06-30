#pragma once

enum class DisplayState : int {
  S_RESET, //reseta o display
  S_TEST, // testa o display
  S_SHOW, // mostra o display
  S_NOISE // ruído
};