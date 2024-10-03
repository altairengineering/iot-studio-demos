#ifndef RFID_H
#define RFID_H

#include <LiquidCrystal_I2C.h>

extern LiquidCrystal_I2C lcd;

void setupRFID();
void RFIDLogic();
void publishDoorStatus(const String& status);
void publishSecurityAlert(const String& message);

#endif