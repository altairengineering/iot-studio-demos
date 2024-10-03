#ifndef CREDENTIALS_H
#define CREDENTIALS_H

#include <Arduino.h>

extern char* mqttTOPIC;
extern char* securityAlertTOPIC; 
extern char* SUBTopicProperties; 
extern char* SUBTopicActions; 
extern String authorizedCardUID; 
extern char* PUBTopicFan; 
extern char* PUBTopicRGB; 
extern char* PUBTopicDoorAction; 
extern char* PUBTopicWindowAction; 
extern char* ssid; 
extern char* password; 

#endif // CREDENTIALS_H