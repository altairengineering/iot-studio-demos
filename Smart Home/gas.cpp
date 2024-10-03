#include <Arduino.h>
#include <ArduinoJson.h>

#define gasPin 23
#define buzPin 25

bool previousGasStatus = false;

#include "gas.h"
#include "mqtt.h"
#include "credentials.h"

void SetupGas() {
  pinMode(buzPin, OUTPUT);
  pinMode(gasPin, INPUT);
}

void GasLogic() {
  bool gasVal = digitalRead(gasPin);  // Reads the value detected by the gas sensor
  bool currentGasStatus = (gasVal == 0);

  Serial.println(gasVal);

  // If hazardous gas is detected, sound the buzzer
  if (gasVal == 0) {
    // Use tone() to generate a continuous sound
    tone(buzPin, 1000);  // 1000 Hz frequency tone
  } else {
    // Stop the tone when no gas is detected
    noTone(buzPin);
  }

  // Check for status change and publish to MQTT if needed
  if (currentGasStatus != previousGasStatus) {
    StaticJsonDocument<200> doc;
    doc["Gas-status"] = currentGasStatus;
    char gas_payload[256];
    serializeJson(doc, gas_payload);
    Serial.println("Publishing gas_payload:");
    Serial.println(gas_payload);
    mqtt_publish(mqttTOPIC, gas_payload);
    previousGasStatus = currentGasStatus;
  }
}
