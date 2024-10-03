#ifndef Door_H
#define Door_H
#include <ESP32Servo.h>

extern Servo door;
extern int servoPin;

void SetupDoor();
void OpenDoor();
void CloseDoor();
void DoorLogic(const String& command);

#endif