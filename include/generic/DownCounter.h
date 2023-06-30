/*! ****************************************************************************
 * @copyright   Miguel Grimm <miguelgrimm@gmail>
 *
 * @brief       Classe concreta da lógica contador decrescente.
 *
 *              Este arquivo pertence ao Ecossistema de Aprendizagem Acelerada
 *              de sistemas embarcados com o uso da linguagem C++.
 *
 * @file        DownCounter.hpp
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
 *                            +teacher	  Miguel Grimm <miguelgrimm@gmail.com>
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


#ifndef C__USERS_MIGUELGRIMM_DESKTOP_CPPLINT_MASTER_DOWNCOUNTER_H_
#define C__USERS_MIGUELGRIMM_DESKTOP_CPPLINT_MASTER_DOWNCOUNTER_H_


#include "Register.h"


/*! ----------------------------------------------------------------------------
 *  @brief    A classe implementa o contador decrescente. Os valores usados
 *            nos parâmetros indicam os valores ajustados no construtor.
 *
 *  @use      Sequência de uso dos serviços da classe.
 *
 *            ++ DownCounter  cnt (4);
 *
 *            ++ OPERAÇÃO DE CONFIGURAÇÃO.
 *            ++ cnt.setEnable (1);
 *            ++ cnt.setCountEnable (1);
 *            ++ cnt.setCountModule (16);
 *            ++ active = cnt.getActive ();
 *
 *            ++ OPERAÇÃO RESET/SET.
 *            ++ cnt.doResetQ ();
 *            ++ cnt.doSetQ ();
 *
 *            ++ OPERAÇÃO DE CONTAGEM.
 *            ++ cnt.doCount ();
 *
 *            ++ OPERAÇÃO DE ACESSO A SAÍDA.
 *            ++ q = cnt.getQ ();
 * -----------------------------------------------------------------------------
 */
class DownCounter : public Register
{

public:

  /*! ----------------------------------------------------------------------------
   *  @brief      Inicializa o contador com a contagem máxima.
   *
   *  @param[in]  module      -  Valor do módulo de contagem.
   * -----------------------------------------------------------------------------
   */
  DownCounter (int countBitSize) : Register (countBitSize)
  {
    /*! Corpo vazio. */
  }


  /*! --------------------------------------------------------------------------
   *  @brief      Decrementa o contador de 1.
   *----------------------------------------------------------------------------
   */
  void doCount ()
  {
    if ((enable == 1) && (active = 1))
    {
      if (countEnable == 1)
      {
        /*! Modo contagem */
        decrementQ ();
      }
    }
  }


private:


  void doRegister ()
  {
    /*! Corpo vazio. */
  }
  void doFlipFlop ()
  {
    /*! Corpo vazio. */
  }


};  //!< class DownCounter.


#endif  // C__USERS_MIGUELGRIMM_DESKTOP_CPPLINT_MASTER_DOWNCOUNTER_H_
