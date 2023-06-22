#include "Monitor.h"

int finishTime = 25;

Monitor monitor;

bool active;

void setup() {
    Serial.begin(115200);

    monitor.setupCommand();
}

int t = 0;

void loop() {
    monitor.commandLoop();
    delay(20);
}
