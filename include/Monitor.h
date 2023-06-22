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

   public:
    Monitor(/* args */);
    ~Monitor();
    void setupCommand();
    void commandLoop();
};
