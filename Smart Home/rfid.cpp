#include <Wire.h>
#include "MFRC522_I2C.h"
#include <ESP32Servo.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>
#include "rfid.h"
#include "door.h"
#include "mqtt.h"
#include "credentials.h"


#define I2C_ADDRESS 0x28  // I2C address for the MFRC522
MFRC522 mfrc522(I2C_ADDRESS);   // Create MFRC522 instance

LiquidCrystal_I2C lcd(0x27, 16, 2);

void publishDoorStatus(const String& status) {
  StaticJsonDocument<200> jsonDoc;
  jsonDoc["Door-status"] = status; // Set the door status in JSON
  
  char jsonStr[200];
  serializeJson(jsonDoc, jsonStr); // Serialize JSON to string

  if (client.publish(mqttTOPIC, jsonStr)) {
    Serial.println("Door status published successfully.");
  } else {
    Serial.println("Failed to publish door status.");
  }
}

void publishSecurityAlert(const String& message) {
  StaticJsonDocument<200> jsonDoc;
  JsonObject securityAlert = jsonDoc.createNestedObject("SecurityAlert");
  securityAlert["data"] = message; // Set the security alert message in JSON
  
  char jsonStr[200];
  serializeJson(jsonDoc, jsonStr); // Serialize JSON to string

  if (client.publish(securityAlertTOPIC, jsonStr)) {
    Serial.println("Security alert published successfully.");
  } else {
    Serial.println("Failed to publish security alert.");
  }
}

void setupRFID() {
  Wire.begin();
  mfrc522.PCD_Init();
  door.attach(servoPin);

  Serial.println(F("Scan your card to open the door..."));
}

void RFIDLogic() {
  // Check for new RFID card
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    delay(50);
    return;
  }

  // Get the UID of the card
  String cardUID = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    cardUID += String(mfrc522.uid.uidByte[i], HEX);
  }
  
  Serial.print(F("Card UID: "));
  Serial.println(cardUID);

  // Check if the detected card UID matches the authorized card UID
  if (cardUID.equals(authorizedCardUID)) {
    Serial.println(F("Authorized card detected. Opening door..."));

     // Update the LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Card Authorized");
    delay (3000);
    lcd.clear();

    publishDoorStatus("opening"); // Notify that door is opening
    OpenDoor();
    delay(5000);  // Keep the door open for 5 seconds
    
    publishDoorStatus("open"); // Notify that door is fully open
    CloseDoor();
    
    publishDoorStatus("closing"); // Notify that door is closing
    delay(1000); // Small delay to ensure the door has time to fully close
    publishDoorStatus("closed"); // Notify that door is fully closed
   

  } else {
    Serial.println(F("Unauthorized card detected."));
    // Update the LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Card Unauthorized");
    publishSecurityAlert("Unauthorized Card Detected");
    delay (3000);
    lcd.clear();
    
  }

  
  mfrc522.PICC_HaltA();
}


