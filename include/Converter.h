/*! ****************************************************************************
 * @copyright   Miguel Grimm <miguelgrimm@gmail>
 *
 * @brief       Classe abstrata da lógica do conversor genérico de códigos.
 *
 * @file        Converter.h
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
 *                Arquitetura de Sistemas Digitais. Ufam, 2021.
 *              + NASCIMENTO, Miguel G. do. Anotações de aula da disciplina de
 *                Eletrônica Digital II. Ufam, 2021.
 * *****************************************************************************
 */


#ifndef C__USERS_MIGUELGRIMM_DESKTOP_CPPLINT_MASTER_CONVERTER_H_
#define C__USERS_MIGUELGRIMM_DESKTOP_CPPLINT_MASTER_CONVERTER_H_


#include <math.h>


enum class logicType : int
{
  positive = 0,
  negative = 1
};



/*! ----------------------------------------------------------------------------
 * @brief    A classe da lógica do conversor genérico de 'data' para 'result'.
 * -----------------------------------------------------------------------------
 */
class Converter
{


public:


  Converter (int dataBitSize = 4) 
  {
    /*! Configura as entradas da lógica */
    enable = 1;
    data   = 0;

    /*! Configura os controles da lógica */
    Converter::dataBitSize = dataBitSize;
    maximumData = pow (2, dataBitSize) - 1;

    /*! Configura as saídas da lógica */
    result = 0;

    sizeActive  = (dataBitSize <= sizeof(unsigned int)*8) ? 1 : 0;
    enableActive = 1;
    logicActive  = 1;
    dataActive   = 1;
    setActive ();
  }


  /*! --------------------------------------------------------------------------
   * @brief      Ajusta a entrada de habilita das operações lógicas.
   *
   * @param[in]  enable  -  Valor da entrada enable.
   * ---------------------------------------------------------------------------
   */
  void setEnable (int enable)
  {
    /*! Ajusta a entrada enable */
    if ((enable >= 0) && (enable <= 1))
    {
      /*! Guarda a entrada de habilita */
      Converter::enable = enable;
       
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
   * @brief      Ajusta a entrada de ativação da entrada decimal;
   *
   * @param[in]  logic  -  positive, o bit em 1 ativa a entrada decimal ou
   *                       negative, o bit em 0 ativa a entrada decimal.
   * ---------------------------------------------------------------------------
   */
  void setInLogic (logicType logic)
  {
    setLogic (logic);
  }


  /*! --------------------------------------------------------------------------
   * @brief      Ajusta a entrada de ativação da entrada decimal;
   *
   * @param[in]  logic  -  positive, o bit em 1 ativa a entrada decimal ou
   *                       negative, o bit em 0 ativa a entrada decimal.
   * ---------------------------------------------------------------------------
   */
  void setOutLogic (logicType logic)
  {
    setLogic (logic);
  }

  /*! Execuções da lógica */
  virtual void doEncoder ()   = 0;
  virtual void doDecoder ()   = 0;
  virtual void doConverter () = 0;

  /*! --------------------------------------------------------------------------
   * @brief       Adquire a sinalização de saída da lógica.
   *
   * @return      1, a lógica está habilitada e o valor da entrada é válido e
   *              0, em caso contrário.
   *  --------------------------------------------------------------------------
   */
  int getActive ()
  {
    /*! Retorna o valor da saída de sinalização da lógica */
    return active;
  }


protected:


  /*! --------------------------------------------------------------------------
   *  @brief       Ajusta o valor da saída de sinalização.
   *  --------------------------------------------------------------------------
   */
  void setActive ()
  {
    /*! Saída de sinalização da lógica */
    active = sizeActive & enableActive & logicActive & dataActive;
  }


  /*! --------------------------------------------------------------------------
   * @brief      Ajusta a entrada de ativação da entrada decimal;
   *
   * @param[in]  logic  -  positive, o bit em 1 ativa a entrada decimal ou
   *                       negative, o bit em 0 ativa a entrada decimal.
   * ---------------------------------------------------------------------------
   */
  inline void setLogic (logicType logic)
  {
    if ((logic >= logicType::positive) && (logic <= logicType::negative))
    {
      
      Converter::logic = logic;
    
      /*! Ajusta a saída de sinalização */
      logicActive = 1;
      setActive ();
    }
    else
    {
      /*! Ajusta a saída de sinalização */
      logicActive = 0;
      setActive ();
    }
  }


  /*! --------------------------------------------------------------------------
   * @brief       Conta o número de 1's do parâmetro de dados.
   *
   * @param[in]   dt, valor do dado de entrada.
   *
   * @return      Número de 1's existentes no parâmetro number.
   * ---------------------------------------------------------------------------
   */
  int countTheNumberOfOnes (unsigned int dt)
  {
    unsigned int   bit0    = 0;
    unsigned int   numOnes = 0;

    /*! Conta o número de 1's */
    for (int i=0; i < dataBitSize; i++) {

      /*! Elimina os 1's a esquerda do bit0 */
      bit0 = dt & 1;

      /*! Verifica se o bit0 possui o valor 1 */
      if (bit0 == 1)
      {
        /*! Incrementa o contador de 1's */
        numOnes++;
      }
      
      /*! Desloca para analisar o próximo bit para o bit0*/
      dt = dt >> 1;
    }
    
    return numOnes;
  }


  /*! --------------------------------------------------------------------------
   *  @brief       Identifica o índice do bit em 1.
   *
   *  @param[in]   dt, valor do dado de entrada.
   *
   *  @return      Valor do índice em 1 do dado de entrada.
   *  --------------------------------------------------------------------------
   */
  int findThePositionOfTheFirstBitToTheLeft (unsigned int dt)
  {
    unsigned int   bit0 = 0;
    int            position = 0;
    int            i = 0,
                   finish = 1;

    while ((i < dataBitSize) && (finish == 1)) {
  
      /*! Elimina os 1's a esquerda do bit0 */
      bit0 = dt & 1;
    
      /*! Verifica se o bit0 possui o valor 1 */
      if (bit0 == 1)
      {
        /*! Guarda o valor do índice do bit em 1 */
        position = i;
      
        /*! Encerra o laço */
        finish = 0;
      }
      else {
        /*! Desloca para analisar o próximo bit para o bit0 */
        dt = dt >> 1;
        i++;
      }  //  if num
    }  //  while.
  
    return position;

  }


protected:


  /*! Entradas da lógica */
  int             enable;
  unsigned int    data;
  logicType       logic;

  /*! Controles da lógica */
  int             dataBitSize;
  int             maximumData;

  int             sizeActive;
  int             enableActive;
  int             logicActive;
  int             dataActive;

  /*! Saídas da lógica */
  unsigned int    result;
  int             active;
  
};

#endif // C__USERS_MIGUELGRIMM_DESKTOP_CPPLINT_MASTER_CONVERTER_H_