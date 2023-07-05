#include "Arduino.h"

#ifndef C__USERS_MIGUELGRIMM_DESKTOP_CPPLINT_MASTER_LOGICBUILDER_H_
#define C__USERS_MIGUELGRIMM_DESKTOP_CPPLINT_MASTER_LOGICBUILDER_H_

#include <TM1637Display.h>
#include "Builder.h"
#include "DisplayDevice.h"

class DisplayBuilder : public Builder
{

public:

  /*! --------------------------------------------------------------------------
   *  @brief      Configuração inicial do serviço da fábrica.
   *  --------------------------------------------------------------------------
   */
  DisplayBuilder ()
  {
    /*! Configura as refrências dos objetos da fábrica */
    display = NULL;
  }


  /*! --------------------------------------------------------------------------
   *  @brief      Liberação dos objetos lógicos criados na fábrica.
   *  --------------------------------------------------------------------------
   */
  ~DisplayBuilder (){
    /*! Libera todos os objetos lógicos da fábrica */
    deleteDisplay();
  }

  void buildDisplay (int pinClock,int pinDio){
    if (enable == 1){
      if (display == NULL){
        /*! Cria o objeto dinamicamente */
        display = new DisplayDevice();
        newActive = 1;
      }
      else newActive = 0;

      DisplayBuilder::setActive ();
    }
  }

  void setupDisplay (int brightness=0x0f){
    if (enable == 1){
      if (display != NULL){
        /*! Configuração do display */
        display->setup(brightness);
        setupActive = 1;
        setActive ();
      }
    }
  }

  void deleteDisplay (){
    if (display != NULL){
      delete display;
      display = NULL;
    }
  }

  DisplayDevice* getDisplay(){
    if(enable == 1) return display;
    return NULL;
  }
  
private:
  /*! Objetos lógicos da fábrica */
  DisplayDevice* display;
};


#endif // C__USERS_MIGUELGRIMM_DESKTOP_CPPLINT_MASTER_LOGICBUILDER_H_
