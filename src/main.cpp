#include <Arduino.h>
#include "Monitor.h"

int finishTime = 25;

Monitor *monitor;

bool active;

void setup() {
    Serial.begin(115200);
    esp_log_level_set("*", ESP_LOG_VERBOSE);
    monitor = Monitor::GetInstance();
    monitor->setupDoor();
    monitor->setupCommand();
    monitor->setupDisplay();
}

int t = 0;

void loop() {
    monitor->doorLoop();
    monitor->commandLoop();
    monitor->displayLoop();
}
