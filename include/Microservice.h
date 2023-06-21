/*! ****************************************************************************
 * @copyright   Miguel Grimm <miguelgrimm@gmail>
 *
 * @brief       Classe abstrata de microsserviços.
 *
 * @file        Microservice.h
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


#ifndef C__USERS_MIGUELGRIMM_DESKTOP_CPPLINT_MASTER_MICROSERVICE_H_
#define C__USERS_MIGUELGRIMM_DESKTOP_CPPLINT_MASTER_MICROSERVICE_H_


class Microservice
{


public:


  /*! Configuração do microsserviço */
  Microservice ()
  {

    /*! Configura as entradas */
    enable = 0;
   

    /*! Configura as saídas */
  

    /*! Ajusta a saída de sinalização */
    enableActive = 0;
    setActive ();

  }


  /*! --------------------------------------------------------------------------
   * @brief      Ajusta a entrada de habilita do microsserviço.
   *
   * @param[in]  enable   -  0..1, Valor de habilita/desabilita a operação
   *                               da lógica.
   * ---------------------------------------------------------------------------
   */
  void setEnable (int enable)
  {

    /*! Ajusta a entrada enable */
    if ((enable >= 0) && (enable <= 1))
    {
      Microservice::enable = enable;

      /*! Ajusta a saída de sinalização */
      enableActive = 1;
    }
    else
    {
      /*! Ajusta a saída de sinalização */
      enableActive = 0;
    }
    setActive ();

  }


  /*! --------------------------------------------------------------------------
   *  @brief      Executa o microsserviço caso este esteja habilitado.
   *  --------------------------------------------------------------------------
   */
  virtual void doMicroservice () = 0;


  /*! --------------------------------------------------------------------------
   *  @brief      Executa o reset caso o microsserviço esteja habilitado.
   *  --------------------------------------------------------------------------
   */
  virtual void doResetMicroservice () = 0;


  int getEnable()  {
    return enable;
  }

  


  /*! --------------------------------------------------------------------------
   *  @brief      Adquire o valor da saída de sinalização.
   *
   *  @return     Valor da saída de sinalização.
   * ---------------------------------------------------------------------------
   */
  int getActive ()
  {
    return active;
  }


protected:


  void setActive ()
  {
    active = enableActive;
  }

  /*! Entrada do microsserviço */
  int enable;

  /*! Controle de sinalização do microsserviço */
  int enableActive;

  /*! Saídas do microsserviço */
  
  
  int active;

};


#endif // C__USERS_MIGUELGRIMM_DESKTOP_CPPLINT_MASTER_MICROSERVICE_H_
