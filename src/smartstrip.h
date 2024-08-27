#ifndef SMARTSTRIP_H
#define SMARTSTRIP_H

#include "./neopixelcontroller/neopixelcontroller.h"
#include "./webserver/webserver.h"
#include "Arduino.h"

class SmartStrip {
public:
    SmartStrip(String mdnsName, int neopixelPin, int ledCount);
    void init();
    void tick();
private:
    WebServer webServer;
    NeopixelController neopixelController;
    void powerOn(String url, WiFiClient& client);
    void powerOff(String url, WiFiClient& client);
    void colorHsv(String url, WiFiClient& client);
    void setBrightness(String url, WiFiClient& client);
    void setMode(String url, WiFiClient& client);
    void setFadeSpeed(String url, WiFiClient& client);
};

#endif