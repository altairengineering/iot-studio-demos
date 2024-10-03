#include <Arduino.h>
#include <ArduinoJson.h>

#include "fan.h"
#include "mqtt.h"
#include "credentials.h"

#define fanPin1 19
#define fanPin2 18
#define btn2 27

int btn_count = 0; 
int speed_val = 130; 

bool FanState = false; 
bool Btn2State = false; 

void SetupFan() {
  pinMode(btn2, INPUT);
  pinMode(fanPin1, OUTPUT);
  pinMode(fanPin2, OUTPUT);
}

void setFanState(bool state) {
  FanState = state;
  digitalWrite(fanPin1, FanState ? HIGH : LOW);
  analogWrite(fanPin2, FanState ? speed_val : 0); 
  Serial.println(FanState ? "Fan ON" : "Fan OFF");
}

void FanLogic() {
  static bool lastBtn2State = false;
  boolean btn2_val = digitalRead(btn2);

  if (btn2_val != lastBtn2State && btn2_val == LOW) { 
    FanState = !FanState;
    setFanState(FanState);
        
    StaticJsonDocument<200> jsonDoc;
    jsonDoc.clear();
    jsonDoc["Fan-status"] = FanState; // Store fan state in JSON
    char jsonStr[200];
    serializeJson(jsonDoc, jsonStr);

    mqtt_publish(mqttTOPIC, jsonStr);
  }

  lastBtn2State = btn2_val;
}
