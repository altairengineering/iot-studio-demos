#include <Adafruit_NeoPixel.h>
#include "rgbd.h"

#define LED_PIN    26
#define LED_COUNT 4

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

String currentMode = "";
String newMode = "";
unsigned long lastUpdate = 0;
int step = 0;

void SetupRGBD() {
  Serial.println("Initializing RGBD...");
  strip.begin();           
  strip.show();           
  strip.setBrightness(50); 
  Serial.println("RGBD Initialized.");
}

void colorWipe(uint32_t color, int wait) {
  if (millis() - lastUpdate > wait) {
    lastUpdate = millis();
    if (step < strip.numPixels()) {
      strip.setPixelColor(step, color);
      strip.show();
      step++;
    } else {
      step = 0;
      currentMode = "";
    }
  }
}

void StopLEDPatterns() {
  strip.clear();
  strip.show();
  currentMode = "";
  step = 0;
 
}

void parse_led_status(const String& message) {
  Serial.print("Received mode change: ");
  Serial.println(message);
  newMode = message;
}

void modeParty() {
  uint32_t colors[] = {strip.Color(255, 0, 0), strip.Color(0, 255, 0), strip.Color(0, 0, 255)};
  if (millis() - lastUpdate > 50) {
    lastUpdate = millis();
    strip.setPixelColor(step % strip.numPixels(), colors[step / strip.numPixels() % 3]);
    strip.show();
    step++;
    if (step >= strip.numPixels() * 3) step = 0;
  }
}

void modeTheater() {
  uint32_t colors[] = {strip.Color(127, 127, 127), strip.Color(127, 0, 0), strip.Color(0, 0, 127)};
  theaterChase(colors[step / 10 % 3], 50);
  step++;
}

void modeRainbow() {
  rainbow(10);
}

void modeTheaterRainbow() {
  theaterChaseRainbow(50);
}

void theaterChase(uint32_t color, int wait) {
  if (millis() - lastUpdate > wait) {
    lastUpdate = millis();
    strip.clear();
    for (int i = step % 3; i < strip.numPixels(); i += 3) {
      strip.setPixelColor(i, color);
    }
    strip.show();
    step++;
  }
}

void rainbow(int wait) {
  if (millis() - lastUpdate > wait) {
    lastUpdate = millis();
    long firstPixelHue = step * 256;
    for (int i = 0; i < strip.numPixels(); i++) {
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show();
    step++;
  }
}

void theaterChaseRainbow(int wait) {
  if (millis() - lastUpdate > wait) {
    lastUpdate = millis();
    int firstPixelHue = step * 256;
    strip.clear();
    for (int i = step % 3; i < strip.numPixels(); i += 3) {
      int hue = firstPixelHue + (i * 65536L / strip.numPixels());
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(hue)));
    }
    strip.show();
    step++;
  }
}

void StartLEDPatterns() {
  if (newMode != currentMode) {
    currentMode = newMode;
    step = 0;
    Serial.print("Switching to mode: ");
    Serial.println(currentMode);
  }

  if (currentMode == "party") {
    modeParty();
  } else if (currentMode == "theater") {
    modeTheater();
  } else if (currentMode == "rainbow") {
    modeRainbow();
  } else if (currentMode == "theaterRainbow") {
    modeTheaterRainbow();
  } else {
    StopLEDPatterns();
  }
}
