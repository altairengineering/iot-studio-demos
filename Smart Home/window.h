#ifndef Window_H
#define Window_H
#include <ESP32Servo.h>

extern Servo window;
extern int servoPin;
extern bool isWindowOpen;

void SetupWindow();
void OpenWindow();
void CloseWindow();
void WindowLogic(const String& status);

#endif