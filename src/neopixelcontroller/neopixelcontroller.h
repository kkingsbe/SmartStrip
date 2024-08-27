#ifndef NEOPIXEL_CONTROLLER_H
#define NEOPIXEL_CONTROLLER_H

#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

#define BRIGHTNESS 50

#define PURPLE 56000
#define BLUE 40000

#define SPEED 2

enum LedStatus {
    WHITE,
    COLOR,
    RGB_FADE
};

class NeopixelController {
public:
    NeopixelController(int pin, int pixels);
    void init();
    void setup_strip(Adafruit_NeoPixel*);
    void tick();
    void white(Adafruit_NeoPixel*);
    void turn_on();
    void turn_off();
    void set_brightness(int);
    void set_color(uint16_t hue, uint8_t sat, uint8_t val);
    void color(Adafruit_NeoPixel*);
    void set_mode(LedStatus);
    void rgb_fade();
    void set_fade_speed(int);
    LedStatus get_status();
private:
    Adafruit_NeoPixel strip;
    LedStatus status;
    int pin;
    int pixels;
    int brightness;
    uint16_t h;
    uint8_t s;
    uint8_t v;
    uint16_t target_h;
    uint8_t target_s;
    uint8_t target_v;
    int target_brightness;
    int rgbfade_speed;
};

#endif