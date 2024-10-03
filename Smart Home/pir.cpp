#include <Arduino.h>
#include <ArduinoJson.h>
#include <time.h>

#include "pir.h"
#include "mqtt.h"
#include "credentials.h"

#define pyroelectric 14

bool previousPIRState = LOW;

void SetupPIR() {
  pinMode(pyroelectric, INPUT);
}

void PIRLogic() {
  
  bool pyroelectric_val = digitalRead(pyroelectric);

  if (pyroelectric_val != previousPIRState) {
    previousPIRState = pyroelectric_val;

    Serial.print("pyroelectric value = ");
    Serial.println(pyroelectric_val);
    
    StaticJsonDocument<200> jsonDoc;
    jsonDoc["PIR-status"] = pyroelectric_val;
    char jsonStr[200];
    serializeJson(jsonDoc, jsonStr);

    mqtt_publish(mqttTOPIC, jsonStr); 
  }

}