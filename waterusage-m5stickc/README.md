# Pet Water Usage Monitor
Uses a scale to estimate the amount of water in a pet's bowl

## IMPORTANT
*DO NOT* rely on this as the sole method of checking to see if your pet has water!

Too many things can go wrong for this to be your only source of information about
your pet's behavior or how much water they have.  You could lose power or WiFi, your
scale could malfunction, there could be coding errors... none of these are a good
reason to let your pet be thirsty.

Your pet loves you and you owe it to them to interact with them and keep them
safe and healthy.  Don't use a project like this as an excuse to neglect your
furry friend!

## Hardware
### Bill of Materials (BOM)
* M5 [M5StickC Plus](https://shop.m5stack.com/products/m5stickc-plus-esp32-pico-mini-iot-development-kit)
* M5 [HX711 scale](https://shop.m5stack.com/products/mini-scales-unit-hx711)
* M5 [Grove cable](https://shop.m5stack.com/products/4pin-buckled-grove-cable) (any length, but longer is probably better!)


### Assembly
Connect the scale to the M5StickC Plus board using the Grove cable.  The cable
will only fit one way, so if it doesn't work, try flipping it over.

## Software
This project is written in C++ using the [Arduino](https://www.arduino.cc/)
framework, a popular way of writing code for microcontrollers without dealing
with the individual differences of each processor and development board.

It's written using [PlatformIO](https://platformio.org/), an open source
platform which automates management of libraries for Arduino (and other!)
environments.

You can run this without PlatformIO using Arduino directly, but you'll need
to install the dependencies yourself.

### Dependencies
If you are using PlatformIO, these dependencies will be installed
automatically.  If you are using the Arduino IDE, you'll need to install them
yourself in the Arduino interface.

It relies on the following libraries:
* [m5stack/M5StickCPlus](https://registry.platformio.org/libraries/m5stack/M5StickCPlus)
* [m5stack/M5Unified](https://registry.platformio.org/libraries/m5stack/M5Unified)
* [m5stack/M5UnitWeightI2C](https://registry.platformio.org/libraries/m5stack/M5UnitWeightI2C)
* [grafana/PrometheusArduino](https://registry.platformio.org/libraries/grafana/PrometheusArduino)
* [grafana/PromLokiTransport](https://registry.platformio.org/libraries/grafana/PromLokiTransport)

### Installation
1. In the `/src` folder, make a copy of `credentials.h.example` and call it
`credentials.h`.  Edit this file to set your WiFi credentials and your Grafana
Cloud credentials.  See the
[Grafana Cloud Metrics documentation](https://grafana.com/docs/grafana-cloud/send-data/metrics/metrics-prometheus/)
for more details on this step.
2. If using the Arduino IDE, install the libraries listed under "Dependencies"
(above).
3. Build the project and flash it onto the M5StickC.

### Water detection
TODO: Need to include information about the scale, weight of water, taring, etc.

### Alerting
TODO

## Usage
The water dish goes on the scale... TODO: Refer to taring

The display on the M5StickC will show the current weight of the water and
a notification of roughly how much is left.

TODO: Managing alerting
