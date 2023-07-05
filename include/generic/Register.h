#include <Arduino.h>

/*! ****************************************************************************
 * @copyright   Miguel Grimm <miguelgrimm@gmail>
 *
 * @brief       Clase abstrata da lógica do registrador genérico.
 *
 *              Este arquivo pertence ao Ecossistema de Aprendizagem Acelerada 
 *              de sistemas embarcados com o uso da linguagem C++.
 *
 * @file        Register.h
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


#ifndef C__USERS_MIGUELGRIMM_DESKTOP_CPPLINT_MASTER_REGISTER_H_
#define C__USERS_MIGUELGRIMM_DESKTOP_CPPLINT_MASTER_REGISTER_H_


#include <math.h>


/*! ----------------------------------------------------------------------------
 *  @brief    A classe cria uma abstração da lógica sequencial.
 * -----------------------------------------------------------------------------
 */
class Register
{
  
public:
  
  /*! --------------------------------------------------------------------------
   *  @brief      Configura o registrador com o valor zero.
   *  --------------------------------------------------------------------------
   */
  Register (int dataBitSize)
  {
    /*! Configura as entradas da lógica */
    enable      = 1;

    loadEnable  = 0;
    data        = 0;
    
    countEnable = 1;
    countModule = pow (2, dataBitSize);

    /*! Configura os controles da lógica */
    maximumData = pow (2, dataBitSize) - 1;
    maximumCount  = countModule - 1;

    /*! Configura as saídas da lógica */
    q = 0;

    sizeActive  = (dataBitSize <= sizeof(unsigned int)*8) ? 1 : 0;
    enableActive = 1;
    countActive  = 1;
    dataActive   = 1;
    setActive ();
  }


  /*! --------------------------------------------------------------------------
   * @brief      Ajusta a entrada de habilita da classe.
   *
   * @param[in]  enable   -  1, habilita todas as operações da lógica e
   *                         0, desabilita em caso contrário.
   * ---------------------------------------------------------------------------
   */
  void setEnable (int enable)
  {
    /*! Ajusta a entrada enable */
    if ((enable >= 0) && (enable <= 1))
    {
      Register::enable = enable;

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
   *  @brief      Ajuste de imediato da saída para o valor zero.
   *  --------------------------------------------------------------------------
   */
  void doResetQ ()
  {
    /*! Guarda a entrada reset */
    if (enable == 1)
    {
      q = 0;
    }
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
   *  @brief      Ajusta a entrada de habilita de carga.
   *
   *  @param[in]  loadEnable   -  1, habilita a carga quando executado e
   *                              0, desabilita em caso contrC!rio.
   * ---------------------------------------------------------------------------
   */
  void setLoadEnable (int loadEnable)
  {
    if ((loadEnable >= 0) && (loadEnable <= 1))
    {
      /*! Ajusta a entrada loadEnable */
      Register::loadEnable = loadEnable;
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
   *  @brief      Ajusta ao dado de carga do registrador.    
   *
   *  @param[in]  data  -  Dado a ser carregado no registrador quando executado.
   *  --------------------------------------------------------------------------
   */
  void setData (unsigned int data)
  {
    Serial.printf("maximumData: %d, data: %d\n", maximumData, data);
    if (data <= maximumData)
	{
	  /*! Guarda a entrada data */
	  Register::data = data;

	  /*! Ajusta a saída de sinalização */
	  dataActive = 1;
	}
    else
	{
      /*! Ajusta a saída de sinalização */
      dataActive = 0;
	}
    setActive ();
  }


  /*! --------------------------------------------------------------------------
   *  @brief      Ajusta a entrada de habilita contagem da lógica.
   *
   *  @param[in]  countEnable   -  1, habilita a contagem da lógica e
   *                               0, desabilita em caso contrário.
   * ---------------------------------------------------------------------------
   */
  void setCountEnable (int countEnable)
  {
    if ((countEnable >= 0) && (countEnable <= 1))
    {
      /*! Ajusta a entrada loadEnable */
      Register::countEnable = countEnable;

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
   *  @brief      Ajusta o módulo de contagem.
   *
   *  @param[in]  countModule   -  Valor do módulo de contagem.
   * ---------------------------------------------------------------------------
   */
  void setCountModule (unsigned int countModule)
  {
    if (countModule <= maximumData)
    {
      /*! Ajusta o módulo de contagem e Elimina os bits a esquerda */
      Register::countModule = countModule;
    
      /* Definição da contagem maxima */
      maximumCount = Register::countModule - 1;
      
      /*! Ajusta a saída de sinalização */
      countActive = 1;
    }
    else
    {
      /*! Ajusta a saída de sinalização */
      countActive = 0;
    }
    setActive ();
  } 


  /*! --------------------------------------------------------------------------
   *  @brief      Adquire o valor armazenado no registrador.
   *
   *  @return     Valor corrente armazenado.
   *  --------------------------------------------------------------------------
   */
  unsigned int getQ ()
  {
    /*! Valor armazenado */
    return q;
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


  /*! Execuções das lógicas sequenciais */
  virtual void doRegister () = 0;
  virtual void doCount ()  = 0;
  virtual void doFlipFlop () = 0;


  /*! --------------------------------------------------------------------------
   *  @brief      Incrementa o contador de 1.
   * ---------------------------------------------------------------------------
   */
  void incrementQ ()
  {

    if (q > maximumCount)
    {
      /*! Caso base: Contagem corrente no valor limite */
      q = 0;
    }
    else 
    {
      /*! Passo de indução: incremento normal do contador */
      q++;
    }
  }


  /*! --------------------------------------------------------------------------
   *  @brief      Decrementa o contador de 1.
   * ---------------------------------------------------------------------------
   */
  void decrementQ ()
  {
    if (q < 0)
    {
      /*! Caso base: Contagem corrente no valor limite */
      q = maximumCount;
    }
    else
    {
      /*! Passo de indução: decremento normal do contador */
      q--;
    }
  }


  /*! --------------------------------------------------------------------------
   *  @brief      Ajusta o valor da saída de sinalização.
   * ---------------------------------------------------------------------------
   */
  void setActive ()
  {
    active = sizeActive & enableActive & dataActive;
  }

  
  /*! --------------------------------------------------------------------------
   *  @brief      ATRIBUTOS PROTEGIDOS.
   * ---------------------------------------------------------------------------
   */

  /*! Entradas da lógica */
  int             enable;
  int             loadEnable;
  int             countEnable;
    
  unsigned int    data;
  unsigned int    countModule;
  

  /*! Controles da lógica */
  unsigned int    maximumData;
  unsigned int    maximumCount;
  
  int             enableActive;

  /*! Saídas da lógica */
  unsigned int    q;
  int             active;


private:


  /*! --------------------------------------------------------------------------
   *  @brief      ATRIBUTOS PRIVADOS.
   * ---------------------------------------------------------------------------
   */
  int             sizeActive;
  int             dataActive,
                  countActive;


};  //!< class Register.

#endif  // C__USERS_MIGUELGRIMM_DESKTOP_CPPLINT_MASTER_REGISTER_H_
