/*! ****************************************************************************
 * @copyright   Miguel Grimm <miguelgrimm@gmail>
 *
 * @brief       Classe concreta da lógica do registrador de dados.
 *
 *              Este arquivo pertence ao Ecossistema de Aprendizagem Acelerada 
 *              de sistemas embarcados com o uso da linguagem C++.
 *
 * @file        DataRegister.h
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
 *                            +teacher    Miguel Grimm <miguelgrimm@gmail.com>
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

#ifndef C__USERS_MIGUELGRIMM_DESKTOP_CPPLINT_MASTER_DATAREGISTER_H_
#define C__USERS_MIGUELGRIMM_DESKTOP_CPPLINT_MASTER_DATAREGISTER_H_


#include "Register.h"


/*! ----------------------------------------------------------------------------
 *  @brief    A classe concreta o registrador de dados.
 *
 *  @use      Sequência de uso dos serviços da classe.
 *
 *            ++ DataRegister  reg (4);
 *
 *            ++ OPERAÇÃO DE CONFIGURAÇÃO.
 *            ++ reg.setEnable (1);
 *            ++ reg.setLoadEnable (0)
 *
 *            ++ OPERAÇÃO DE RESET/SET.
 *            ++ reg.doResetQ ();
 *            ++ reg.doSetQ ();
 *
 *            ++ OPERAÇÃO DE CARGA DE DADOS:
 *            ++ reg.setLoadEnable (1);
 *            ++ reg.setData (0);
 *            ++ active = reg.getActive ();
 *            ++ reg.doRegister ();
 *            ++ reg.setLoadEnable (0);
 *            ++ q = reg.getQ ();
 * -----------------------------------------------------------------------------
 */
class DataRegister : public Register
{
  
public:
  
  /*! Configuração da lógica */
  DataRegister (int dataBitSize = 4) : Register (dataBitSize)
  {
    /*! Corpo vazio */
  }


  /*! ----------------------------------------------------------------------------
   *  @brief      Carrega o registrador com um dado.
   *  ----------------------------------------------------------------------------
   */
  void doRegister ()
  {

    if ((enable == 1) && (active == 1))
    {
      if (loadEnable == 1)
	  {
	    /*! Modo carga */
        q = data;
	  }
    }
  }
  

private:


  /*! ----------------------------------------------------------------------------
   *  @brief      Funções-membro vazias.
   * -----------------------------------------------------------------------------
   */
  void doCount ()
  {
    /*! Corpo vazio. */
  }
  void doFlipFlop ()
  {
    /*! Corpo vazio. */
  }


};  //!< class DataRegister.


#endif  // C__USERS_MIGUELGRIMM_DESKTOP_CPPLINT_MASTER_DATAREGISTER_H_
