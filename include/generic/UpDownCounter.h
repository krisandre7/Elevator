/*! ****************************************************************************
 * @copyright   Miguel Grimm <miguelgrimm@gmail>
 *
 * @brief       Classe concreta da lógica do contador crescente/decrescente.
 *
 *              Este arquivo pertence ao Ecossistema de Aprendizagem Acelerada 
 *              de sistemas embarcados com o uso da linguagem C++.
 *
 * @file        UpDownCounter.h
 * @version     1.0
 * @date        27 Julho 2021
 *
 * @section     HARDWARES & SOFTWARES.
 *              +compiler     GDB online - C++ 17
 *              +revisions    Versão (data): Descrição breve.
 *                            ++ 1.0 (27 Julho 2021): Versão inicial.
 *
 * @section     AUTHORS & DEVELOPERS.
 *              +institution  UFAM - Universidade Federal do Amazonas.
 *              +courses      Engenharia da Computação / Engenharia Elétrica.
 *              +teacher      Miguel Grimm <miguelgrimm@gmail.com>
 *
 *                            Compilação e Depuração:
 *               +teacher     Miguel Grimm <miguelgrimm@gmail.com>
 *
 * @section     LICENSE
 *
 *              GNU General Public License (GNU GPL).
 *
 *              Este programa é um software livre; Você pode redistribuí-lo
 *              e/ou modificá-lo de acordo com os termos do "GNU General Public
 *              License" como publicado pela Free Software Foundation; Seja a
 *              versão 3 da licença, ou qualquer outra versão posterior.
 *
 *              Este programa é distribuído na esperança de que seja útil,
 *              mas SEM QUALQUER GARANTIA; Sem a garantia implícita de
 *              COMERCIALIZAÇÃO OU USO PARA UM DETERMINADO PROPÓSITO.
 *              Veja o site da "GNU General Public License" para mais detalhes.
 *
 * @htmlonly    http://www.gnu.org/copyleft/gpl.html
 *
 * @section     REFERENCES.
 *              + NASCIMENTO, Miguel G. do. Anotações de aula da disciplina de
 *                Arquitetura de Sistemas Digitais. Ufam, 2021
 *              + NASCIMENTO, Miguel G. do. Anotações de aula da disciplina de
 *                Eletrônica Digital II. Ufam, 2021
 * *****************************************************************************
 */


#ifndef C__USERS_MIGUELGRIMM_DESKTOP_CPPLINT_MASTER_UPDOWNCOUNTER_H_
#define C__USERS_MIGUELGRIMM_DESKTOP_CPPLINT_MASTER_UPDOWNCOUNTER_H_


#include "Register.h"


enum class countType : int
{
  down = 0,
  up   = 1
};


/*! ----------------------------------------------------------------------------
 *  @brief    A classe implementa o contador crescente/decrescente. Os valores
 *            usados nos parâmetros indicam os valores ajustados no construtor.
 *
 *  @use      Sequência de uso dos serviços da classe.
 *
 *            ++ UpDownCounter  cnt (4);
 *
 *            ++ OPERAÇÃO DE CONFIGURAÇÃO.
 *            ++ cnt.setEnable (1);
 *            ++ cnt.setCountEnable (1);
 *            ++ cnt.setCountModule (10);
 *            ++ cnt.setLoadEnable (0);
 *            ++ active = cnt.getActive ();
 *
 *            ++ OPERAÇÃO DE RESET/SET.
 *            ++ cnt.doResetQ ();
 *            ++ cnt.doSetQ ();
 *
 *            ++ OPERAÇÃO DE CARGA DE DADOS.
 *            ++ cnt.setLoadEnable (1);
 *            ++ cnt.setData (5);
 *            ++ cnt.doCount ();
 *            ++ cnt.setLoadEnable (0);
 *
 *            ++ OPERAÇÃO DE CONTAGEM CRESCENTE.
 *            ++ cnt.setCountMode (countType::up);
 *            ++ cnt.doCount ();
 *            ++ q = cnt.getQ ();
 *
 *            ++ OPERAÇÃO DE CONTAGEM DECRESCENTE.
 *            ++ cnt.setCountMode (countType::down);
 *            ++ cnt.doCount ();
 *            ++ q = cnt.getQ ();
 * -----------------------------------------------------------------------------
 */
class UpDownCounter : public Register
{


public:

  
  /*! --------------------------------------------------------------------------
   *  @brief      Inicializa o contador com o valor zero.
   *
   *  @param[in]  countBitSize   -  Número de bits do contador.
   * ---------------------------------------------------------------------------
   */
  UpDownCounter (int countBitSize = 4) : Register (countBitSize)
  {
    /*! Configura as entradas da lógica */
    countMode   = countType::up;
    data        = 5;

    /*! Configura as saídas da lógica*/
    modeActive   = 1;
    UpDownCounter::setActive ();
  }


  /*! --------------------------------------------------------------------------
   *  @brief      Ajusta de imediato  o valor da saída para o valor máximo.
   * ---------------------------------------------------------------------------
   */
  void doSetQ ()
  {
    if (enable == 1)
    {
      /*! Modo preset */
      q = maximumCount;   
    }
  }


  /*! --------------------------------------------------------------------------
   *  @brief      Ajusta o módulo de contagem.
   *
   *  @param[in]  countMode   -  Valor do tipo de contagem.
   * ---------------------------------------------------------------------------
   */
  void setCountMode (countType countMode)
  {
    if ((countMode >= countType::down) && (countMode <= countType::up))
    {
      /*! Ajusta o módulo de contagem e Elimina os bits a esquerda */
      UpDownCounter::countMode = countMode;

      /*! Ajusta a saída de sinalização */
      modeActive = 1;
      UpDownCounter::setActive ();
    }
    else
    {
      /*! Ajusta a saída de sinalização */
      modeActive = 0;
      UpDownCounter::setActive ();
    }
  } 


  /*! ----------------------------------------------------------------------------
   *  @brief      Incrementa/Decrementa/Carrega/Reset/Preset o contador de 1.
   *------------------------------------------------------------------------------
   */
  void doCount ()
  {
    if ((enable == 1) && (active == 1))
    {
      if (loadEnable == 1)
      {
        /*! Modo carga */
        q = data;
      }
      else if (countEnable == 1)
      {
        /*! Modo contagem */
        if (countMode == countType::up)
        {
          /*! Contagem crescente */
          incrementQ ();
        }
        else
        {
          /*! Contagem decrescente */
          decrementQ ();
        }
      }
    }
  }


private:


  /*! --------------------------------------------------------------------------
   *  @brief      Ajusta a saída de sinalização.
   * ---------------------------------------------------------------------------
   */
  void setActive ()
  {
    /*! Ajusta a saída de sinalização */
    Register::setActive ();

    active &= modeActive;
  }

  void doRegister ()
  {
    /*! Corpo vazio. */
  }
  void doFlipFlop ()
  {
    /*! Corpo vazio. */
  }

  /*! Entradas da lógica */
  countType       countMode;

  /* Controles da lógica */
  int             modeActive,
                  presetActive;

};  //!< class UpDownCounter.


#endif  // C__USERS_MIGUELGRIMM_DESKTOP_CPPLINT_MASTER_UPDOWNCOUNTER_H_
