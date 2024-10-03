#include <Arduino.h>
#include <ArduinoJson.h>

#include "button-led.h"
#include "mqtt.h"
#include "rgbd.h"
#include "credentials.h"

#define btn1 16
#define led_y 12

bool ledState = false; 
bool btnState = false; 

void SetupLedButton() {
  pinMode(btn1, INPUT);
  pinMode(led_y, OUTPUT);
}

void TurnLedButton() {
  static bool lastBtnState = false; // Track the last button state
  bool btn1_val = digitalRead(btn1); // Read current button state

  if (btn1_val != lastBtnState && btn1_val == LOW) {
    ledState = !ledState; // Toggle the LED state
    digitalWrite(led_y, ledState ? HIGH : LOW); 

    StaticJsonDocument<200> jsonDoc;
    jsonDoc.clear();
    jsonDoc["Led-status"] = ledState; // Store LED state in JSON
    char jsonStr[200];
    serializeJson(jsonDoc, jsonStr);

    mqtt_publish(mqttTOPIC, jsonStr); // Publish MQTT message
  }

  lastBtnState = btn1_val; // Update last button state
}
