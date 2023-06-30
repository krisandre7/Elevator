#ifndef COMMAND_LOGICBUILDER_H_
#define COMMAND_LOGICBUILDER_H_


#include "Builder.h"
#include "AsciiToHexa.h"
#include "DataRegister.h"

class CommandBuilder:public Builder {

public:

    /* --------------------------------------------------------------------------
    *  @brief      Configuração inicial do serviço da fábrica.
    *  --------------------------------------------------------------------------
    */
    CommandBuilder() {
        // Configura as referências dos objetos da fábrica 
        reg = nullptr;
        a2h = nullptr;
    }


    /* --------------------------------------------------------------------------
    *  @brief      Liberação dos objetos lógicos da fábrica.
    *  --------------------------------------------------------------------------
    */
    ~CommandBuilder() {
        // Libera todos os objetos lógicos da fC!brica
        deleteDataRegister();
        deleteHexaToAscii();
    }

    /* --------------------------------------------------------------------------
    * @brief      Cria dinamicamente o decodificador Ascii para Hexadecimal.
    *  --------------------------------------------------------------------------
    */
    void buildAsciiToHexa() {
      if ( enable == true ) {
        if ( a2h == nullptr) {
            // Cria o objeto dinamicamente
            a2h = new AsciiToHexa();
            newActive = true;
        } else newActive = false;
        CommandBuilder::setActive();
      }
    }

  /*! --------------------------------------------------------------------------
   * @brief      Cria dinamicamente o registrador de 2 bits
   *  --------------------------------------------------------------------------
   */
  void buildDataRegister () {
    if ( enable == true ) {
	    if ( reg == nullptr ) {
	      // Cria o objeto dinamicamente
	      reg = new DataRegister(2);
	      newActive = 1;
	    } else newActive = 0;
	    CommandBuilder::setActive ();
    }
  }
  
  void setupDataRegister () {
     /*! Configuração do decodificador */
	    reg->setEnable(1);

	    setupActive = reg->getActive();
	    setActive();
  }

  /*! --------------------------------------------------------------------------
   *  @brief      Prepara o objeto da classe do Decodificador de Hexa para Ascii
   *  --------------------------------------------------------------------------
   */
  void setupAsciiToHexa() {
    if ( enable == 1 and a2h != nullptr ) {
	    /*! Configuração do decodificador */
	    a2h->setEnable(1);

	    setupActive = a2h->getActive();
	    setActive();
    }
  }


    /*! --------------------------------------------------------------------------
    *  @brief      Libera o objeto da classe DataRegister
    *  --------------------------------------------------------------------------
    */
    void deleteDataRegister() {
      /*! Libera o objeto na fábrica */
      if ( reg != nullptr ) {
        delete reg;
        reg = nullptr;
      }
    }
    
    /*! --------------------------------------------------------------------------
    *  @brief      Libera o objeto do Codificador de Hexa para Ascii
    *  --------------------------------------------------------------------------
    */
    void deleteHexaToAscii() {
      /*! Libera o objeto na fábrica */
      if ( a2h != nullptr ) {
        delete a2h;
        a2h = nullptr;
      }
    }


    /*! --------------------------------------------------------------------------
    *  @brief      Adquire a referência do objeto da clase DataRegister
    *
    *  @return     Referência do objeto da classe DataRegister
    *  --------------------------------------------------------------------------
    */
    DataRegister *getDataRegister() {
      if ( enable == 1 )
        return reg;
      return nullptr;
    }
  
    /*! --------------------------------------------------------------------------
    *  @brief      Adquire a referência do objeto do Codificador de Hexa para Ascii
    *
    *  @return     Referência do objeto do Codificador de Hexa para Ascii
    *  --------------------------------------------------------------------------
    */
    AsciiToHexa * getAsciiToHexa() {
      if ( enable == 1 )
        return a2h;
      return nullptr;
    }


private:
    /*! Objetos lógicos da fábrica */
    DataRegister *reg = nullptr;
    AsciiToHexa *a2h = nullptr;
};


#endif // COMMAND_LOGICBUILDER_H_
