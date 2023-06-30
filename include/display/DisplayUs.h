#ifndef C__USERS_MIGUELGRIMM_DESKTOP_CPPLINT_MASTER_SEMAFOROUS_H_
#define C__USERS_MIGUELGRIMM_DESKTOP_CPPLINT_MASTER_SEMAFOROUS_H_


#include "Microservice.h"
#define QTSEGMENTOS 4

enum class stateType : int
{
  wait_for_test = 0,
  testing = 1,
  set_display = 2,
  noise = 3
};

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
    andar      = 0; //[0,2]
    updownstop = 0; // 0 - parado, 1 - descendo, 2 - subindo
    start      = 0;
    
    /*! Ajusta a saída de sinalização */
    state = stateType::wait_for_test;
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
      state = stateType::wait_for_test;
      doMicroservice();
      
    }
  }


  /*! --------------------------------------------------------------------------
   * @brief      Realiza a partida no serviço.
   *
   * @param[in]  fibo  -  1, Endereço do objeto do contador crescente.
   * ---------------------------------------------------------------------------
   */
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
        case stateType::wait_for_test:
            state = stateType::testing;
            break;
        case stateType::testing:
            if(mode==1 || reset==1) state = stateType::set_display;
            break;
        case stateType::set_display:
            mode=1;
            break;
        case stateType::noise:
            state = stateType::wait_for_test;
            break;
        default:
            state = stateType::wait_for_test;
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
      return (stateType::set_display)==state;
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
  
  int getUpdownstop(){
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
  
  void setUpDownStop(int updownstop){
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
  int             updownstop;
    
  /*! Controle do microsserviço */
  stateType       state = stateType::wait_for_test;
  int             start;
  int             classActive,
                  logicActive,
                  startActive;

  /*! Saída do microsserviço */
  int data[QTSEGMENTOS]={0xff,0xff,0xff,0xff};
};

#endif // C__USERS_MIGUELGRIMM_DESKTOP_CPPLINT_MASTER_SEMAFOROUS_H_

