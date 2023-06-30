/*! ****************************************************************************
 * @copyright   Miguel Grimm <miguelgrimm@gmail>
 *
 * @brief       Classe abstrata da lógica genérica de fluxo de dados.
 *
 *              Este arquivo pertence ao Ecossistema de Aprendizagem Acelerada
 *              de sistemas embarcados com o uso da linguagem C++.
 *
 * @file        Flow.h
 * @version     1.0
 * @date        27 Julho 2021
 *
 * @section     HARDWARES & SOFTWARES.
 *              +compiler     GDB online - C++ 17
 *              +revisions    Versão (data): Descrição breve.
 *                            ++ 1.0 (27 Julho 2021): VersC#o inicial.
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
 *                Arquitetura de Sistemas Digitais. Ufam, 2021.
 *              + NASCIMENTO, Miguel G. do. Anotações de aula da disciplina de
 *                Eletrônica Digital II. Ufam, 2021.
 * *****************************************************************************
 */


#ifndef C__USERS_MIGUELGRIMM_DESKTOP_CPPLINT_MASTER_FLOW_H_
#define C__USERS_MIGUELGRIMM_DESKTOP_CPPLINT_MASTER_FLOW_H_


#include <math.h>


/*! ----------------------------------------------------------------------------
 *  @brief    A classe define a lógica genérico de operações aritméticas.
 * -----------------------------------------------------------------------------
 */
class Flow
{

public:

  Flow (int dataBitSize = 4)
  {
    /*! Configurações de lógica default */
    enable       = 1;
    dataA        = 0;
    dataB        = 0;
    carryIn      = 0;
    carryOut     = 0;

    /*! Configurações de controle default */
    Flow::dataBitSize = dataBitSize;
    maximumData = pow (2, dataBitSize) - 1;

    /*! Ajusta saída de sinalização */
    sizeActive    = (dataBitSize <= sizeof(unsigned int)*8) ? 1 : 0;
    enableActive  = 1;
    dataAActive   = 1;
    dataBActive   = 1;
    carryInActive = 1;
    setActive ();

    /*! Ajusta saída de dados */
    result   = 0;
    carryOut = 0;
  }


  /*! ----------------------------------------------------------------------------
   * @brief      Ajusta a entrada de habilitação das operações.
   *
   * @param[in]  enable   -  1, habilita todas as operações na classe e
   *                         0, desabilita em caso contrário.
   * -----------------------------------------------------------------------------
   */
  void setEnable (int enable)
  {

    /*! Ajusta a entrada enable */
    if ((enable >= 0) && (enable <= 1))
    {
      Flow::enable = enable;

      /*! Ajusta a saída de sinalização */
      enableActive = 1;
      setActive ();
    }
    else
    {
     /*! Ajusta a saída de sinalização */
      enableActive = 0;
      setActive ();
    }

  }


  /*! --------------------------------------------------------------------------
   *  @brief       Guarda o dado A, se estiver no limite do número de bits.
   *
   *  @param[in]   dataA  -  Valor do dado A.
   *  --------------------------------------------------------------------------
   */
  void setDataA (int dataA)
  {


    if (dataA <= maximumData)
    {
      /*! Guarda o dado de entrada */
      Flow::dataA = dataA;

      /*! Ajusta a saída de sinalização */
      dataAActive = 1;
      setActive ();
    }
    else
    {
      /* Lógica desabilitada ou valor inválido da entrada */
      dataAActive = 0;
      setActive ();
    }

  }


  /*! --------------------------------------------------------------------------
   *  @brief       Guarda o dado se estiver no limite do numero de bits.
   *
   *  @param[in]   dataB  -  Valor do dado B.
   *  --------------------------------------------------------------------------
   */
  void setDataB (int dataB)
  {

    if (dataB <= maximumData)
    {
      /*! Guarda o dado de entrada */
      Flow::dataB = dataB;

      /*! Ajusta a saída de sinalização */
      dataBActive = 1;
      setActive ();
    }
    else
    {
      /* Lógica desabilitada ou valor inválido da entrada */
      dataBActive = 0;
      setActive ();
    }

  }


  /*! ----------------------------------------------------------------------------
   *  @brief       Guarda o valor da entrada de vem-um de entrada.
   *
   *  @param[in]   carryIn  -  0..1, valor do bit de vem-um.
   *  ----------------------------------------------------------------------------
   */
  void setCarryIn (int carryIn)
  {

    if ((enable == 1) && (carryIn >= 0) && (carryIn <= 6))
    {
      /*! Guarda a entrada serial a */
      Flow::carryIn = carryIn;

      /*! Ajusta a saída de sinalização */
      carryInActive = 1;
      setActive ();
    }

    else
    {
      /* Lógica desabilitada ou valor inválido da entrada */
      carryInActive = 0;
      setActive ();
    }

  }


  /*! Execução da lógica */
  virtual void doFlow ()       = 0;
  virtual void doArithmetic () = 0;


  /*! --------------------------------------------------------------------------
   *  @brief      Adquire a saída de sinalização.
   *  --------------------------------------------------------------------------
   */
  int getActive ()
  {
    return active;
  }


  /*! --------------------------------------------------------------------------
   *  @brief      Adquire o valor do resultado da operação aritmética.
   *
   *  @return     0..dataBitSize-1, valor do resultado da operação aritmética.
   *  --------------------------------------------------------------------------
   */
  int getResult ()
  {
    return result;
  }


  /*! --------------------------------------------------------------------------
   *  @brief      Adquire o valor do bit de vai-um de saída.
   *
   *  @return     0..1, valor do bits de vai-um.
   *  --------------------------------------------------------------------------
   */
  int getCarryOut ()
  {

    /*! Acessa ao atributo protegido */
    return carryOut;

  }


protected:

  /*! --------------------------------------------------------------------------
   *  @brief      Extrai o n-ésimo bit do dado de entrada.
   *
   *              data   - Dado a ser extraído o bit.
   *
   *              number - Valor do n-ésimo bit.
   *
   *  @return     0..1,    valor do bit extraído do dado.
   *  --------------------------------------------------------------------------
   */
  int extractBit (unsigned int data, int number)
  {

    /*! O valor do bit a ser obtido. */
    int   resto;
    int   valor;

    /*! Retira os bits a direita do bit desejado. */
    if (number = 0)
    {
      resto = data;
    }
    else
    {
      // VHDL: resto = data / pow (2, number);
      resto = data >> number;
    }

    /*! Adquire o bit desejado. */
    valor = resto % 2;

    /*! Retorna o bit desejado. */
    return valor;

  }


  /*! --------------------------------------------------------------------------
   *  @brief      Extrai um grupo de últimos bits  do dado de entrada.
   *
   *              data   - Dado a ser extraído o grupo de bit.
   *
   *              number - Valor do número de bits do grupo.
   *
   *  @return     0..MAX,    valor do grupo de bits extraído do dado.
   *  --------------------------------------------------------------------------
   */
  int extractLastBits (unsigned int data, int number)
  {

    /*! Grupo de bits a ser obtido. */
    int   valor, resto;

    /*! Retira o grupo de últimos bits a direita. */
    resto = pow (2, number);
    valor = data % resto;

    /*! Retorno do grupo dos últimos bits. */
    return valor;

  }


  /*! --------------------------------------------------------------------------
   *  @brief      Ajusta a saída de sinalizaçãoo.
   *  --------------------------------------------------------------------------
   */
  void setActive ()
  {
    active = sizeActive & enableActive &
             dataAActive & dataBActive & carryInActive;
  }


  /*! Entradas de lógica */
  int            enable;
  unsigned int   dataA, dataB;
  int            carryIn, carryOut;

  /*! Controle da lógica */
  int            dataBitSize;
  int            maximumData;

  int            sizeActive;
  int            enableActive;
  int            dataAActive, dataBActive;
  int            carryInActive;

  /*! Saídas da lógica */
  unsigned int   result;
  int            active;

};


#endif // C__USERS_MIGUELGRIMM_DESKTOP_CPPLINT_MASTER_FLOW_H_
