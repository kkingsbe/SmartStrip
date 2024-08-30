#include "neopixelcontroller.h"

NeopixelController::NeopixelController(int _pin, int _pixels) : 
                                           pin(_pin),
                                           pixels(_pixels),
                                           status(WHITE),
                                           prev_status(WHITE),
                                           brightness(0),
                                           rgbfade_speed(10),
                                           strip(_pixels, _pin, NEO_GRBW + NEO_KHZ800)
{ }

void NeopixelController::init() {
    pinMode(pin, OUTPUT);
    this->setup_strip(&this->strip);
}

void NeopixelController::setup_strip(Adafruit_NeoPixel* strip) {
  strip->begin();
  strip->show();
  strip->setBrightness(BRIGHTNESS);
}

void NeopixelController::tick() {
    // Smoothly interpolate between the previous color and new color
    if(h != target_h || s != target_s || v != target_v) {
        int16_t h_diff = (int16_t)target_h - (int16_t)h;
    
        // Adjust h_diff to always represent the shortest path
        if (h_diff > 32767) h_diff -= 65536;
        else if (h_diff < -32768) h_diff += 65536;
        
        uint16_t abs_h_diff = abs(h_diff);
        uint16_t h_speed = max((uint16_t)ceil(abs_h_diff / 20), (uint16_t)1);


        if (abs_h_diff <= 32768) {
            // Move in the direction of h_diff
            if (abs_h_diff <= h_speed) {
                h = target_h;
            } else {
                h = (uint16_t)((int32_t)h + (h_diff > 0 ? h_speed : -h_speed));
            }
        } else {
            // Move in the opposite direction (shorter path)
            if (65536 - abs_h_diff <= h_speed) {
                h = target_h;
            } else {
                h = (uint16_t)((int32_t)h + (h_diff > 0 ? -h_speed : h_speed));
            }
        }

        if(target_s > s) {
            if(s + SPEED > target_s || s + SPEED < s) s = target_s;
            else s += SPEED;
        } else {
            if(s - SPEED < target_s || s - SPEED > s) s = target_s;
            else s -= SPEED;
        }

        if(target_v > v) {
            if(v + SPEED > target_v || v + SPEED < v) v = target_v;
            else v += SPEED;
        } else {
            if(v - SPEED < target_v || v - SPEED > v) v = target_v;
            else v -= SPEED;
        }
    }
    
    if(brightness != target_brightness) {
        if(target_brightness > brightness) {
            brightness += SPEED;
        } else {
            brightness -= SPEED;
        }

        brightness = constrain(brightness, 0, 255);
    }

    switch (status) {
        case WHITE:
            this->white(&this->strip);
            break;
        case COLOR:
            this->color(&this->strip);
            break;
        case RGB_FADE:
            this->rgb_fade();
            this->color(&this->strip);
            break;
    }
    
    this->send_update();
}

void NeopixelController::send_update() {
    if (status != prev_status || brightness != prev_brightness || h != prev_h || s != prev_s || v != prev_v) {
        prev_status = status;
        prev_brightness = brightness;
        prev_h = h;
        prev_s = s;
        prev_v = v;

        strip.show();
    }
}

void NeopixelController::white(Adafruit_NeoPixel* strip) {
    strip->setBrightness(brightness);
    strip->fill(strip->Color(0, 0, 0, strip->gamma8(brightness)));
}

void NeopixelController::turn_on() {
    this->target_brightness = 255;
    this->white(&this->strip);
}

void NeopixelController::turn_off() {
    this->target_brightness = 0;
    this->white(&this->strip);
}

void NeopixelController::set_color(uint16_t hue, uint8_t sat, uint8_t val) {
    this->target_h = hue;
    this->target_s = sat;
    this->target_v = val;
    this->status = COLOR;
}

void NeopixelController::color(Adafruit_NeoPixel* strip) {
    strip->setBrightness(brightness);

    for (int i = 0; i < strip->numPixels(); i++) {
        strip->setPixelColor(i, strip->ColorHSV(h, s, v));
    }
}

LedStatus NeopixelController::get_status() {
    return status;
}

void NeopixelController::set_brightness(int brightness) {
    this->target_brightness = brightness;
}

void NeopixelController::set_mode(LedStatus status) {
    this->status = status;
}

void NeopixelController::set_fade_speed(int speed) {
    this->rgbfade_speed = speed;
}

void NeopixelController::rgb_fade() {
    target_h += rgbfade_speed;
    target_s = 255;
    target_v = 255;
}