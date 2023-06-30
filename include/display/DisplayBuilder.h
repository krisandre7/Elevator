#include "Arduino.h"

#ifndef C__USERS_MIGUELGRIMM_DESKTOP_CPPLINT_MASTER_LOGICBUILDER_H_
#define C__USERS_MIGUELGRIMM_DESKTOP_CPPLINT_MASTER_LOGICBUILDER_H_

#include <TM1637Display.h>
#include "Builder.h"
#define QTSEGMENTOS 4

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
    aux[0]=aux[1]=aux[2]=aux[3]=0x00;
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
        display = new TM1637Display(pinClock,pinDio);
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
        display->setBrightness(brightness);
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

  TM1637Display* getDisplay(){
    if(enable == 1){
      return display;
    }
    return NULL;
  }

  uint8_t* getData(){
    return data;
  }

  void setData(CabinAction updownstop,int andar){
    data[0]=data[1]=data[3]=0x00; //0
    if(updownstop==CabinAction::S_TO_DOWN) data[1] = SEG_E | SEG_D | SEG_C;
    else if(updownstop==CabinAction::S_TO_UP) data[1] = SEG_F | SEG_A | SEG_B;
    data[2] = display->encodeDigit(andar+1); //0
  }

  void setAllDigits(int value=0xff){
    for(int i=0;i<QTSEGMENTOS;i++) data[i]=value;
  }

  void printDataDisplay(){
    if(isDataChanged()){
      display->clear();
      display->setSegments(data);
    } 
  }

  int isDataChanged(){
    int inequal=0;
    for(int i=0;i<QTSEGMENTOS;i++){
      if(data[i]!=aux[i]) inequal=1;
      aux[i]=data[i];
    }
    return inequal;
  }
  
private:
  /*! Objetos lógicos da fábrica */
  uint8_t data[4],aux[4];
  TM1637Display* display;
};


#endif // C__USERS_MIGUELGRIMM_DESKTOP_CPPLINT_MASTER_LOGICBUILDER_H_
