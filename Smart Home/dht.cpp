#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <DHT11.h>
#include <ArduinoJson.h>

#include "mqtt.h"
#include "dht.h"
#include "rfid.h"
#include "credentials.h"

DHT11 dht11(17);

float lastHumidity = NAN;
float lastTemperature = NAN;

void SetupDHT() {
  lcd.init();      
  lcd.backlight();
  
}

void DHTLogic() {
 
  int temperature = 0;
  int humidity = 0;
  int result = dht11.readTemperatureHumidity(temperature, humidity);
  
  if (result == 0) {
    if (temperature != lastTemperature || humidity != lastHumidity) {
      lastTemperature = temperature;
      lastHumidity = humidity;

      StaticJsonDocument<200> jsonDoc;
      jsonDoc["Temperature"] = temperature;
      jsonDoc["Humidity"] = humidity;
      char jsonStr[200];
      serializeJson(jsonDoc, jsonStr);

      mqtt_publish(mqttTOPIC, jsonStr);
  
      Serial.print("Temperature: ");
      Serial.print(temperature);
      Serial.print(" °C\tHumidity: ");
      Serial.print(humidity);
      Serial.println(" %");

      lcd.setCursor(0, 0);
      lcd.print("Temp: ");
      lcd.print(temperature);
      lcd.print(" C");
      lcd.setCursor(0, 1);
      lcd.print("Hum: ");
      lcd.print(humidity);
      lcd.print(" %");
  } else {
      
  }
}
        
  }