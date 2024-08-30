#ifndef SMARTSTRIP_H
#define SMARTSTRIP_H

#include "./neopixelcontroller/neopixelcontroller.h"
#include "./webserver/webserver.h"
#include "Arduino.h"

enum LedStripType {
    RGB,
    RGBW
};

struct LedStripConfig {
    int pin;
    int ledCount;
    LedStripType type;
};

class SmartStrip {
public:
    SmartStrip(String mdnsName, int _speed, int _numStrips, LedStripConfig* stripConfig);
    void init();
    void tick();
private:
    int numStrips;
    int speed; // This is how long to delay between each tick in milliseconds. Smaller value = faster
    WebServer webServer;
    NeopixelController** neopixelControllers;
    void powerOn(String url, WiFiClient& client);
    void powerOff(String url, WiFiClient& client);
    void colorHsv(String url, WiFiClient& client);
    void setBrightness(String url, WiFiClient& client);
    void setMode(String url, WiFiClient& client);
    void setFadeSpeed(String url, WiFiClient& client);
};

#endif