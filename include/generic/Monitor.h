#include "AsciiToHexa.h"
#include "CommandBuilder.h"
#include "CommandUs.h"
#include "DataRegister.h"
#include "BluetoothService.h"
#include "Compare.h"
#include "DownCounter.h"
#include "CabinBuilder.h"
#include "CabinUs.h"

#include "UpDownCounter.h"
#include "DoorBuilder.h"
#include "DoorUs.h"

#include "DisplayBuilder.h"
#include "DisplayUs.h"

class Monitor {
   private:
    // Command
    BluetoothService *bluetoothService;
    CommandBuilder *commandBuilder;
    CommandUs *commandUs;
    DataRegister *commandDataRegister;
    AsciiToHexa *asciiToHexa;

    // Door
    DoorBuilder *doorBuilder;
    DoorUs *doorUs;
    UpDownCounter *upDownCounter;

    // Cabin
    CabinBuilder *cabinBuilder;
    CabinUs *cabinUs;
    DataRegister *cabinDataRegister;
    Compare *comparator;
    DownCounter *downCounter;

    // Display
    DisplayBuilder *displayBuilder;
    DisplayUs *displayUs;
    void prints();

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

    void setupDisplay();
    void setupCommand();
    void setupDoor();
    void setupCabin();
    void commandLoop();
    void doorLoop();
    void cabinLoop();
    void displayLoop();
};
