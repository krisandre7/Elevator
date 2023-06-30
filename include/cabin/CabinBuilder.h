#ifndef COMMAND_LOGICBUILDER_H_
#define COMMAND_LOGICBUILDER_H_


#include "Builder.h"
#include "Compare.h"
#include "DataRegister.h"
#include "DownCounter.h"

class CabinBuilder:public Builder
{

public:

    /* --------------------------------------------------------------------------
    *  @brief      ConfiguraC'C#o inicial do serviC'o da fC!brica.
    *  --------------------------------------------------------------------------
    */
	CabinBuilder() {

        // Configura as refrC*ncias dos objetos da fábrica
        reg = nullptr;
        dc  = nullptr;
        cmp = nullptr;
    }


    /* --------------------------------------------------------------------------
    *  @brief      Liberação dos objetos lógicos da fábrica.
    *  --------------------------------------------------------------------------
    */
    ~CabinBuilder() {
        // Libera todos os objetos lC3gicos da fC!brica
        deleteDataRegister();
        deleteDownCounter();
        deleteCompare();
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
	    CabinBuilder::setActive ();
    }
  }

  void setupDataRegister () {
     /*! Configuração do decodificador */
	    reg->setEnable(1);

	    setupActive = reg->getActive();
	    setActive();
  }
  /*! --------------------------------------------------------------------------
   * @brief      Cria dinamicamente o Comparador de 4 bits
   *  --------------------------------------------------------------------------
   */

  void buildComparator () {
    if ( enable == true ) {
	    if ( cmp == nullptr ) {
	      // Cria o objeto dinamicamente
	      cmp = new Compare(4);
	      newActive = 1;
	    } else newActive = 0;
	    CabinBuilder::setActive ();
    }
  }

  void setupComparator () {
     /*! Configuração do comparador */
	    cmp->setEnable(1);

	    setupActive = reg->getActive();
	    setActive();
  }
  /*! ----

/*! --------------------------------------------------------------------------
   * @brief      Cria dinamicamente o Contador Decrementador de 2 bits.
   *  --------------------------------------------------------------------------
   */
  void buildDownCounter () {
    if ( enable == true ) {
	    if ( dc == nullptr ) {
	      // Cria o objeto dinamicamente
	      dc = new DownCounter (4);
	      newActive = 1;
	    } else newActive = 0;
	    CabinBuilder::setActive ();
    }
  }

   /*! --------------------------------------------------------------------------
   *  @brief      Prepara o objeto da classe do Contador Decrementador de 2 bits
   *  --------------------------------------------------------------------------
   */
  void setupDownCounter(unsigned int module) {
    if ( enable == 1 and dc != nullptr ) {
	    /*! Configuração do contador */
	    dc->setEnable(1);
	    dc->setCountEnable(0);
	    dc->setCountModule(module);
	    dc->doResetQ();

	    setupActive = dc->getActive();
	    setActive();
    }
  }

  void setupCompare(unsigned int module) {
    if ( enable == 1 and dc != nullptr ) {
	    /*! Configuração do contador */
	    dc->setEnable(1);
	    dc->setCountEnable(0);
	    dc->setCountModule(module);
	    dc->doResetQ();

	    setupActive = dc->getActive();
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
    *  @brief      Libera o objeto do Contador Decrementador de 2 bits
    *  --------------------------------------------------------------------------
    */
    void deleteDownCounter() {
        /*! Libera o objeto na fábrica */
        if ( dc != nullptr ) {
	        delete dc;
	        dc = nullptr;
        }
    }

 /*! --------------------------------------------------------------------------
    *  @brief      Libera o objeto do Comparador de 2 bits
    *  --------------------------------------------------------------------------
    */
    void deleteCompare() {
        /*! Libera o objeto na fábrica */
        if ( cmp != nullptr ) {
	        delete cmp;
	        cmp = nullptr;
        }
    }

    /*!
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
    *  @brief      Adquire a referência do objeto do Contador Decrementador de 2 bits
    *
    *  @return     Referência do objeto do Contador Decrementador de 2 bits
    *  --------------------------------------------------------------------------
    */
    DownCounter *getDownCounter() {
        if ( enable == 1 )
            return dc;
        return nullptr;
    }


    /*! --------------------------------------------------------------------------
    *  @brief      Adquire a referência do objeto do Comparador 4 bits
    *
    *  @return     Referência do objeto do Contador Decrementador de 2 bits
    *  --------------------------------------------------------------------------
     */

     Compare *getComparator() {
         if (enable == 1)
            return cmp;
        return nullptr;
     }


private:

    /*! Objetos lógicos da fábrica */
    DownCounter * dc = nullptr;
    DataRegister * reg = nullptr;
    Compare * cmp = nullptr;
};


#endif // COMMAND_LOGICBUILDER_H_
