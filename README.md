# SmartStrip

SmartStrip is an Arduino library that turns your ESP8266 into a smart-light controller. It provides a simple API accessible over mDNS, allowing you to control multiple LED strips with different configurations via HTTP requests. Whether you're looking to control the power, adjust the color, or change the mode of your LED strips, SmartStrip makes it easy to create a custom lighting experience.

## Features

- **mDNS Support**: Automatically discover and connect to the SmartStrip controller on your network without needing to know the IP address.
- **Multi-strip Support**: Control multiple LED strips independently or simultaneously.
- **Customizable API**: Access the controller via an HTTP API to manage power, colors, brightness, modes, and fade speed.
- **Flexible Configuration**: Supports various LED strip types (e.g., RGB, RGBW) with customizable pin and LED count settings.

## Getting Started

### Prerequisites

- **Hardware**: ESP8266 with one or more WS2812B (or similar) LED strips.
- **Software**: Arduino IDE with the required libraries:
  - [Adafruit NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel)
  - [ESP8266WiFi](https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi)
  - [ESP8266mDNS](https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266mDNS)

### Installation

1. Clone or download the SmartStrip library from the repository.
2. Copy the `smartstrip` folder into your Arduino `libraries` directory.
3. Install the necessary dependencies via the Arduino Library Manager.

### Basic Usage

```cpp
#include <smartstrip.h>

LedStripType stripType = RGBW;
LedStripConfig stripConfig = {
  .pin = 5,
  .ledCount = 144,
  .type = stripType
};

LedStripConfig stripConfigArr[1] = { stripConfig };

SmartStrip smartStrip("desk-lights", 10, 1, stripConfigArr);

void setup() {
  Serial.begin(115200);
  smartStrip.init();
}

void loop() {
  smartStrip.tick();
}
```

### API Endpoints
SmartStrip exposes the following HTTP endpoints, accessible via mDNS:

- `POST /power/on`: Turn on all connected LED strips.
- `POST /power/off`: Turn off all connected LED strips.
- `POST /color/hsv?h=<hue>&s=<saturation>&v=<value>`: Set the color of all strips using HSV values.
- `POST /brightness?brightness=<level>`: Adjust the brightness of all strips.
- `POST /mode/white`: Set the mode to solid white.
- `POST /mode/color`: Set the mode to solid color (defined by the current HSV values).
- `POST /mode/rgb-fade`: Set the mode to a smooth RGB fade.
- `POST /fade-speed?speed=<ms>`: Adjust the fade speed of the RGB fade mode.

### Advanced Configuration
SmartStrip allows you to control multiple LED strips with different configurations. Simply pass an array of LedStripConfig objects during initialization:

```cpp
LedStripConfig stripConfig1 = { .pin = 5, .ledCount = 144, .type = RGBW };
LedStripConfig stripConfig2 = { .pin = 6, .ledCount = 72, .type = RGB };
LedStripConfig stripConfigArr[2] = { stripConfig1, stripConfig2 };

SmartStrip smartStrip("living-room-lights", 10, 2, stripConfigArr);
```

### Additional Information
- Delay between Ticks: The speed parameter in the constructor controls the delay between ticks in milliseconds. Smaller values result in faster updates.