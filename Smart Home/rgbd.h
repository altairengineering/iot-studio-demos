#ifndef RGBD_H
#define RGBD_H

void SetupRGBD();
void StartLEDPatterns();
void StopLEDPatterns();
void parse_led_status(const String& message);
void colorWipe(uint32_t color, int wait);
void theaterChaseRainbow(int wait);
void rainbow(int wait);
void theaterChase(uint32_t color, int wait);
void modeTheaterRainbow();
void modeTheater();
void modeParty();
void modeRainbow();

#endif