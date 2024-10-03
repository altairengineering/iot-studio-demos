#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

#include "button-led.h"
#include "mqtt.h"
#include "pir.h"
#include "dht.h"
#include "rgbd.h"
#include "fan.h"
#include "water.h"
#include "door.h"
#include "window.h"
#include "gas.h"
#include "rfid.h"
#include "credentials.h"

const char* mqtt_server = "mqtt.swx.altairone.com";

int status = WL_IDLE_STATUS;

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ");
  Serial.print(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    Serial.print(WiFi.status());
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  Serial.begin(9600);
  initWiFi();
  Serial.print("RRSI: ");
  Serial.println(WiFi.RSSI());

  SetupLedButton();
  SetupPIR();
  SetupGas();
  SetupDHT();
  SetupRGBD();
  SetupFan();
  SetupWater();
  SetupDoor();
  SetupWindow();  
  setupRFID(); 
  setup_mqtt(mqtt_server); 
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop(); 
  
  GasLogic();
  TurnLedButton();
  PIRLogic();
  DHTLogic();
  FanLogic();
  StartLEDPatterns();
  WaterLogic();
  RFIDLogic();

}