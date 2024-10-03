#include "mqtt.h"
#include "fan.h"
#include "door.h"
#include "window.h"
#include "rgbd.h"
#include "dht.h"
#include "gas.h"
#include "pir.h"
#include "rfid.h"
#include "credentials.h"

#include <WiFi.h>
#include <ArduinoJson.h>

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived to ");
    Serial.print(topic);
    Serial.print(". Payload: ");
    String message;
    for (unsigned int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    
    Serial.println(message);

    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, message);

    if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.f_str());
        return;
    }

    if (String(topic) == PUBTopicFan) {
        bool fanStatus = doc["Fan-status"];
        setFanState(fanStatus);

    }else if (String(topic) == PUBTopicRGB) {
        const char* mode = doc["RGBD-mode"];
        Serial.print("RGBD-mode: ");
        Serial.println(mode);
        parse_led_status(String(mode));

    }else if (String(topic) == PUBTopicDoorAction) { 
        const char* doorCommand = doc["OpenCloseDoor"]["input"]["command"];
        if (doorCommand) {
            Serial.print("Door command received: ");
            Serial.println(doorCommand);
            DoorLogic(String(doorCommand));
        } 
    }else if (String(topic) == PUBTopicWindowAction) { 
        const char* WindowCommand = doc["OpenCloseWindow"]["input"]["status"];;
        if (WindowCommand) {
            Serial.print("Window command received: ");
            Serial.println(WindowCommand);
            WindowLogic(String(WindowCommand));
        } 
      }
          
}


void setup_mqtt(const char* mqtt_server) {
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
    client.setBufferSize(512);
    reconnect();
}

void reconnect() {
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        if (client.connect("ESP32Client","K4184q@homey","1234")) {
            Serial.println("connected");
            client.subscribe(SUBTopicProperties);
            client.subscribe(SUBTopicActions);
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
                   
        }
    }
}

void mqtt_publish(const char* topic, const char* msg) {
    if (!client.connected()) {
        reconnect();
    }
    client.publish(topic, msg);
    client.loop();
}

