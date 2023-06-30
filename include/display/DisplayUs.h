#ifndef C__USERS_MIGUELGRIMM_DESKTOP_CPPLINT_MASTER_SEMAFOROUS_H_
#define C__USERS_MIGUELGRIMM_DESKTOP_CPPLINT_MASTER_SEMAFOROUS_H_


#include "Microservice.h"
#include "DisplayState.h"
#define QTSEGMENTOS 4

class DisplayUs : public Microservice
{

    
public:
  /*! --------------------------------------------------------------------------
   *  @brief      Configuração inicial do serviço.
   *  --------------------------------------------------------------------------
   */
  DisplayUs ()
  {
    /*! Configura as entradas do microsserviço */
    mode       = 0;
    reset      = 0;
    start      = 0;
    
    /*! Ajusta a saída de sinalização */
    state = DisplayState::S_RESET;
    classActive = 1;
    logicActive = 1;
    startActive = 1;
    setActive ();
  }

  /*! --------------------------------------------------------------------------
   *  @brief      Executa o reset di microsserviço habilitado.
   *  --------------------------------------------------------------------------
   */
  void doResetMicroservice ()
  {
    if ((enable == 1) && (active == 1))
    {
        
      mode = 0;
      andar = 0;
      updownstop = CabinAction::S_STOPPED;
      state = DisplayState::S_RESET;
    }
  }

  void setStart (int start)
  {
    if ((start >= 0) && (start <= 1))
    {
      /*! Guarda a entrada de controle. */
      DisplayUs::start = start;

      /*! Ajusta a saída de sinalização. */
      startActive = 1;
    }
    else
    {
      /*! Ajusta a saída de sinalização. */
      startActive = 0;
    }
    setActive ();
  }


  /*! --------------------------------------------------------------------------
   *  @brief      Execução do microsserviço de sinalização.
   *  --------------------------------------------------------------------------
   */
  void doMicroservice ()
  {
    if ((enable == 1) && (active == 1))
    {
      switch (state)
      {
        case DisplayState::S_RESET:
            state = DisplayState::S_TEST;
            break;
        case DisplayState::S_TEST:
            if(mode==1) state = DisplayState::S_SHOW;
            break;
        case DisplayState::S_SHOW:
            break;
        case DisplayState::S_NOISE:
            state = DisplayState::S_TEST;
            break;
        default:
            state = DisplayState::S_TEST;
            break;
      }
    }
  }


  /*! --------------------------------------------------------------------------
   *  @brief      Adquire o valor da saída de sinalização.
   *
   *  @return     Valor da saída de sinalização.
   * ---------------------------------------------------------------------------
   */
  int getTesteOk(){
      return (state = DisplayState::S_SHOW)==state;
  }
  
  int getActive ()
  {
      return active;
  }
    
  int getState () 
  {
      return (int) state;
  }
  
  int getAndar(){
      return andar;
  }
  
  CabinAction getUpdownstop(){
      return updownstop;
  }
  
  int getDigit(int indice){
      return data[indice];
  }
  
  void setReset(int reset){
      DisplayUs::reset = reset;
  }
  
  void setMode(int mode){
      DisplayUs::mode = mode;
  }
  
  void setAndar(int andar){
      DisplayUs::andar = andar;
  }
  
  void setUpDownStop(CabinAction updownstop){
      DisplayUs::updownstop = updownstop;
  }
  
private:


  void setActive ()
  {
    /*! Ajuste com a situação do microsserviço */
    Microservice::setActive ();
    
    active &= classActive & logicActive & startActive;
  }

  /*! Entradas do microsserviço */
  int             mode;
  int             reset;
  int             andar;
  CabinAction     updownstop;
    
  /*! Controle do microsserviço */
  DisplayState    state;
  int             start;
  int             classActive,
                  logicActive,
                  startActive;

  /*! Saída do microsserviço */
  int data[QTSEGMENTOS]={0xff,0xff,0xff,0xff};
};

#endif // C__USERS_MIGUELGRIMM_DESKTOP_CPPLINT_MASTER_SEMAFOROUS_H_

