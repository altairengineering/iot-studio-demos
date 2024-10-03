#include <ESP32Servo.h>
#include <Arduino.h>
#include <ArduinoJson.h>

#include "door.h"

Servo door;
int servoPin = 13;
bool isDoorOpen = false;

void SetupDoor() {
    door.attach(servoPin);
}

void OpenDoor() {
    for (int pos = 0; pos <= 180; pos += 1) { 
        door.write(pos);
        delay(15);
    }
}

void CloseDoor() {
    for (int pos = 180; pos >= 0; pos -= 1) {
        door.write(pos);
        delay(15);
    }
}

void DoorLogic(const String& command) {
    if (command == "open" && !isDoorOpen) {
        OpenDoor();
        isDoorOpen = true;
    } else if (command == "closed" && isDoorOpen) {
        CloseDoor();
        isDoorOpen = false;
    }
}
