#include "AsciiToHexa.h"
#include "CommandBuilder.h"
#include "CommandUs.h"
#include "DataRegister.h"

class Monitor {
   private:
    /* data */
    CommandBuilder *commandBuilder;
    CommandUs *commandUs;
    DataRegister *dataRegister;
    AsciiToHexa *asciiToHexa;

    // Definição da entradas bluetooth
    uint8_t bluetoothData = 0;
    unsigned int currentFloor = 0;
    DoorAction doorAction = DoorAction::ACT_CLOSED_DOOR;
    CabinState cabinState = CabinState::S_STOPPED;

    // Definição das saidas
    int state;
    int startTest;
    int startDoor;
    int startCabin;
    unsigned requestedfloor;

   protected:
    Monitor();
    static Monitor *singleton_;

   public:
    // Singletons should not be cloneable.
    Monitor(Monitor &other) = delete;

    // Singletons should not be assignable.
    void operator=(const Monitor &) = delete;

    /**
     * This is the static method that controls the access to the
     * Monitor instance. On the first run, it creates a
     * Monitor object and places it into the static field. On
     * subsequent runs, it returns the client existing object stored in the
     * static field.
     */
    static Monitor *GetInstance();
    void setupCommand();
    void commandLoop();
};
