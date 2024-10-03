#include <ESP32Servo.h>
#include <Arduino.h>
#include <ArduinoJson.h>

#include "window.h"

#define WindowServoPin 5

Servo window;

bool isWindowOpen = false;

void SetupWindow() {
    window.attach(WindowServoPin);
  
}

void OpenWindow() {
    Serial.print("OpenWindow\n");
    window.write(190); 
    delay(1000);
    isWindowOpen = true;

}

void CloseWindow() {
     Serial.print("CloseWindow\n");
     window.write(100);
     delay(1000);
     isWindowOpen = false;
}

void WindowLogic(const String& status) {
    if (status == "open" && !isWindowOpen) {
        OpenWindow();
        isWindowOpen = true;
        Serial.print("WindowLogic called with status: ");
        Serial.println(status);
       
    } else if (status == "closed" && isWindowOpen) {
        CloseWindow();
        isWindowOpen = false;
        Serial.print("WindowLogic called with status: ");
        Serial.println(status);
    }
}
