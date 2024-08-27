#include "smartstrip.h"
#include "webserver/webserver.cpp"
#include "neopixelcontroller/neopixelcontroller.cpp"

SmartStrip::SmartStrip(String mdnsName, int neopixelPin, int ledCount) : 
webServer(mdnsName), 
neopixelController(neopixelPin, ledCount) { }

void SmartStrip::init() {
    webServer.init();
    neopixelController.init();

    // Register API Routes using lambda functions to capture 'this'
    webServer.registerRoute(new RouteHandler(HTTPMethod::POST, "/power/on", [this](String url, WiFiClient& client) {
        this->powerOn(url, client);
    }));
    webServer.registerRoute(new RouteHandler(HTTPMethod::POST, "/power/off", [this](String url, WiFiClient& client) {
        this->powerOff(url, client);
    }));
    webServer.registerRoute(new RouteHandler(HTTPMethod::POST, "/color/hsv", [this](String url, WiFiClient& client) {
        this->colorHsv(url, client);
    }));
    webServer.registerRoute(new RouteHandler(HTTPMethod::POST, "/brightness", [this](String url, WiFiClient& client) {
        this->setBrightness(url, client);
    }));
    webServer.registerRoute(new RouteHandler(HTTPMethod::POST, "/mode", [this](String url, WiFiClient& client) {
        this->setMode(url, client);
    }));
    webServer.registerRoute(new RouteHandler(HTTPMethod::POST, "/fade-speed", [this](String url, WiFiClient& client) {
        this->setFadeSpeed(url, client);
    }));
}


void SmartStrip::tick() {
    webServer.tick();
    neopixelController.tick();
}


void SmartStrip::powerOff(String url, WiFiClient& client) {
    Serial.println("Powering off shelf.");
    neopixelController.turn_off();
}

void SmartStrip::powerOn(String url, WiFiClient& client) {
    Serial.println("Powering on shelf.");
    neopixelController.turn_on();
}

void SmartStrip::colorHsv(String url, WiFiClient& client) {
    Serial.println("Setting shelf color.");
    Serial.println(url);

    uint16_t h;
    uint8_t s;
    uint8_t v;

    h = url.substring(url.indexOf("h=") + 2, url.indexOf("&s=")).toInt();
    s = url.substring(url.indexOf("s=") + 2, url.indexOf("&v=")).toInt();
    v = url.substring(url.indexOf("v=") + 2).toInt();

    neopixelController.set_color(h, s, v);
}

void SmartStrip::setBrightness(String url, WiFiClient& client) {
    Serial.println("Setting brightness.");
    Serial.println(url);

    int brightness = url.substring(url.indexOf("brightness=") + 11).toInt();
    neopixelController.set_brightness(brightness);
}

void SmartStrip::setMode(String url, WiFiClient& client) {
    if(url.indexOf("/white") >= 0) {
        Serial.println("Setting shelf mode to white.");
        neopixelController.set_mode(LedStatus::WHITE);
    } else if(url.indexOf("/color") >= 0) {
        Serial.println("Setting shelf mode to color.");
        neopixelController.set_mode(LedStatus::COLOR);
    } else if(url.indexOf("/rgb-fade") >= 0) {
        Serial.println("Setting mode to rgb fade.");
        neopixelController.set_mode(LedStatus::RGB_FADE);
    }
}

void SmartStrip::setFadeSpeed(String url, WiFiClient& client) {
    Serial.println("Setting fade speed.");
    Serial.println(url);

    int speed;

    sscanf(url.c_str(), "POST /fade-speed?speed=%d", &speed);

    neopixelController.set_fade_speed(speed);
}