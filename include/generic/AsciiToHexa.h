/*! ****************************************************************************
 * @copyright   Miguel Grimm <miguelgrimm@gmail>
 *
 * @brief       Classe concreta da lógica do codificador do código
 *              ASCII para o código hexadecimal.
 *
 * @file        AsciiToHexa.h
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


#ifndef C__USERS_MIGUELGRIMM_DESKTOP_CPPLINT_MASTER_ONEHOT2BINARY_H_
#define C__USERS_MIGUELGRIMM_DESKTOP_CPPLINT_MASTER_ONEHOT2BINARY_H_


#include "Converter.h"


/*! ----------------------------------------------------------------------------
 *  @brief    A classe implementa a lógica do codificador do código
 *            ASCII para código Hexadecimal.
 *
 *  @use      Sequência de uso dos serviços da classe.
 *
 *            ++ AsciiToHexa    oh2b ();
 *
 *            ++ oh2b.setEnable (enable);
 *            ++ oh2b.setAscii (data);
 *
 *            ++ oh2b.doEncoder ();
 *
 *            ++ binary = oh2b.getHexadecimal ();
 * -----------------------------------------------------------------------------
 */
class AsciiToHexa : public Converter
{


public:


  /*! --------------------------------------------------------------------------
   *  @brief      Inicializa a lógica.
   *  --------------------------------------------------------------------------
   */
  AsciiToHexa () : Converter (7)
  {
    /*! Corpo vazio. */
  }


  /*! --------------------------------------------------------------------------
   *  @brief       Guarda o valor da entrada da lógica.
   *  
   *  @param[in]   data  -  Valor do código ASCII a ser codificado.
   *  --------------------------------------------------------------------------
   */
  void setAscii (unsigned int data)
  {

    /*! Verifica se o número de entrada é válido */
    if (((data >= '0') && (data <= '9')) || 
        ((data >= 'a') && (data <= 'f')) ||
        ((data >= 'A') && (data <= 'F')) || 
        (data == ' '))
    {
      /*! Guarda o valor de entrada */
      Converter::data = data;
      dataActive = 1;
      setActive ();
    }
    else
    {
      /*! Apaga as saídas da lógica */
      dataActive = 0;
      setActive ();
    }

  }


  /*! --------------------------------------------------------------------------
   *  @brief       Realiza a lógica de codificação de ASCII para hexadecimal.
   *  --------------------------------------------------------------------------
   */
  void doEncoder () {

    if ((enable == 1) && (active == 1))
    {
      /*! Codifica de ASCII para hexadecimal */
      if ((data >= '0') && (data <= '9'))
      {
        /*! Número de '0' até '9' */
        result = data - '0';
      }
      else if ((data >= 'a') && (data <= 'f'))
      {
        /*! Letras minúsculas de 'a' até 'f' */
        result = (data - 'a') + 10;
      }
      else if ((data >= 'A') && (data <= 'F'))
      {
        /*! letras maiúsculas de 'A' até 'F' */
        result = (data - 'A') + 10;
      }
      else
      {
        /*! Espaço em branco */
        result = 0;
      }
    }
    else
    {
      result = 0;
      active = 0;
    }

  }


  /*! --------------------------------------------------------------------------
   *  @brief       Adquire o resultado de saída da lógica.
   *  
   *  @return      código Ascii, O valor resultado da lógica ou
   *               0, caso a lógica esteja desabilitada.
   *  --------------------------------------------------------------------------
   */
  unsigned int getHexa ()
  {
    /*! Retorna o valor da saída de resultado da lógica */
    return result;
  }


private:


  /*! Implementação dos métodos virtuais */
  void doDecoder ()
  {
    /*! Corpo vazio. */
  }
  void doConverter ()
  {
    /*! Corpo vazio. */
  }

};


#endif // C__USERS_MIGUELGRIMM_DESKTOP_CPPLINT_MASTER_ONEHOT2BINARY_H_
