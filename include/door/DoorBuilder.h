/*! ****************************************************************************
 * @copyright   Miguel Grimm <miguelgrimm@gmail>
 *
 * @brief       Classe concreta da fábrica de objetos lógicos.
 *
 * @file        DoorBuilder.h
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
 *                Arquitetura de Sistemas Digitais. Ufam, 2021.
 *              + NASCIMENTO, Miguel G. do. Anotações de aula da disciplina de
 *                Eletrônica Digital II. Ufam, 2021.
 * *****************************************************************************
 */

#ifndef C__USERS_MIGUELGRIMM_DESKTOP_CPPLINT_MASTER_DoorLogicBuilder_H_
#define C__USERS_MIGUELGRIMM_DESKTOP_CPPLINT_MASTER_DoorLogicBuilder_H_


#include "Builder.h"
#include "UpDownCounter.h"


/*! ----------------------------------------------------------------------------
 *  @brief    A classe implementa a fábrica de objetos lógicos.
 *
 *  @use      Sequência de uso dos serviços da classe.
 *
 *            ++ DoorBuilder    fac;
 *
 *            ++ fac.setEnable (enable);
 *
 *            ++ fac.buildCounter (dataBitSize);
 *            ++ fac.buildCompare (dataBitSize);
 *
 *            ++ fac.setupCounter (moduleSize);
 *            ++ fac.setupCompare (data);
 *
 *            ++ cnt = fac.getCounter ();
 *            ++ cmp = fac.getCompare ();
 * -----------------------------------------------------------------------------
 */
class DoorBuilder : public BlueetoothBuilder
{

public:

  /*! --------------------------------------------------------------------------
   *  @brief      Configuração inicial do serviço da fábrica.
   *  --------------------------------------------------------------------------
   */
  DoorBuilder ()
  {

    /*! Configura as saídas da fábrica */
    upDownCounter = NULL;
  }


  /*! --------------------------------------------------------------------------
   *  @brief      Liberação dos objetos lógicos criados na fábrica.
   *  --------------------------------------------------------------------------
   */
  ~DoorBuilder ()
  {

    /*! Libera todos os objetos lógicos da fábrica */
    deleteUpDownCounter();
  }


  /*! --------------------------------------------------------------------------
   * @brief      Cria dinamicamente o Contador de Fibonacci.
   *
   * @param[in]  dataBitSize   Número dos bits do contador de Fibonacci.
   * ---------------------------------------------------------------------------
   */
  void buildUpDownCounter (int dataBitSize = 4)
  {

    if (enable == 1)
    {
      if (upDownCounter == NULL)
      {
        /*! Cria o objeto dinamicamente */
        upDownCounter = new UpDownCounter (dataBitSize);
      
        newActive = 1;
      }
      else
      {
        newActive = 0;
      }
      DoorBuilder::setActive ();
    }

  }

  /*! --------------------------------------------------------------------------
   *  @brief      Prepara o objeto da classe do Contador de Fibonacci.
   *  --------------------------------------------------------------------------
   */
  void setupUpDownCounter (unsigned int module)
  {

    if (enable == 1)
    {
      if (upDownCounter != 0)
      {
        /*! Configuração do contador */
        upDownCounter->setEnable (1);
        upDownCounter->setCountEnable (1);
        upDownCounter->setCountModule (module);
        upDownCounter->doResetQ ();
        upDownCounter->setCountMode (countType::up);
        
        setupActive = upDownCounter->getActive ();
        setActive ();
      }
    }

  }


  /*! --------------------------------------------------------------------------
   *  @brief      Libera o objeto do contador de Fibonacci criado na fábrica.
   *  --------------------------------------------------------------------------
   */
  void deleteUpDownCounter ()
  {

    /*! Libera o objeto na fábrica */
    if (upDownCounter != NULL)
    {
      delete upDownCounter;
      upDownCounter = NULL;
    }

  }


  /*! --------------------------------------------------------------------------
   *  @brief      Adquire a referência do objeto do Contador de Fibonacci.
   *
   *  @return     Referência do objeto do Contador de Fibonacci.
   *  --------------------------------------------------------------------------
   */
  UpDownCounter* getUpDownCounter ()
  {

    if (enable == 1)
    {
      return upDownCounter;
    } else {
        return NULL;
    }

  }

private:


  /*! Objetos lógicos da fábrica */
  UpDownCounter     * upDownCounter;
};


#endif // C__USERS_MIGUELGRIMM_DESKTOP_CPPLINT_MASTER_DoorLogicBuilder_H_