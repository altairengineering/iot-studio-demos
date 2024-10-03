#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP32Servo.h>

#include "water.h"
#include "mqtt.h"
#include "window.h"
#include "credentials.h"


#define waterPin 34

int previousWaterVal = 0;

void SetupWater(){
  pinMode(waterPin, INPUT);
}

void WaterLogic(){
  int water_val = analogRead(waterPin);
  Serial.print("Raindrop sensor value: ");
  Serial.println(water_val);

  if (water_val != previousWaterVal) {
    previousWaterVal = water_val;

    if(water_val > 0 ) {
      
      if (isWindowOpen){
        Serial.println("Rain detected, closing window...");
        WindowLogic("closed");
        isWindowOpen = false;

      }

      StaticJsonDocument<200> doc;
      doc["Raindrop-sensor"] = water_val;
      doc["Window-status"] = "closed";
      char payload[256];
      serializeJson(doc, payload);
      Serial.println("Publishing MQTT:");
      Serial.println(payload);
      
      mqtt_publish(mqttTOPIC, payload);
          
    } else if (water_val == 0) {

      StaticJsonDocument<200> doc;
      doc["Raindrop-sensor"] = water_val;
      char payload[256];
      serializeJson(doc, payload);

      Serial.println("Publishing MQTT (value is 0):");
      Serial.println(payload);
      mqtt_publish(mqttTOPIC, payload);
    }
  }
}