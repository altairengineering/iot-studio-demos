#ifndef MQTT_H
#define MQTT_H

#include <PubSubClient.h>

extern PubSubClient client;

void setup_mqtt(const char* mqtt_server);
void reconnect();
void mqtt_publish(const char* topic, const char* message);
void callback(char* subscribeTopic, byte* payload, unsigned int length) ;

#endif
