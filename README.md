# PrometheusForPets
Examples of using Prometheus and Grafana to monitor pet behavior

## Overview
This repository contains a few simple projects that use ESP-32 microcontroller
development boards and cheap, widely available sensors and output devices to
show easy and fun ways to collect information from the world around you.  In
this case, your pet!

This data is stored in [Prometheus](prometheus.io), an open-source timeseries
metric database.  Prometheus is widely used in monitoring IT infrastructure and
software, but can be used anywhere that you want to track data over time.

Once in Prometheus, the data is shown and used in
[Grafana](https://grafana.com), an open-source dashboard and alerting system.
Grafana gives you the ability to create beautiful dashboards from data stored in
many different places and send alerts when something doesn't look right.

## Projects
The projects in this repository have been designed around cheap, widely
available hardware that is easy to connect and use.  No soldering is required
to build these projects -- they either use small devices that have everything
already integrated or components that can be connected with a single (standard)
cable.

They use components from a variety of vendors to illustrate that these sorts
of projects don't rely on a single manufacturer.  And they're written in two
different languages (C++ and Python) to show that you can pick a language that
is comfortable for you to work in.

Each project is named with what it does, then a `-`, and finally the
ESP32 dev board that is used in it.

### Pet door flap monitor
*doorflap-qtpyesp32*

Uses an accelerometer to detect when a pet door is opened (and whether it was
the pet entering or exiting the door).
* *Dev board:* Adafruit [QT Py ESP32-S3](https://adafru.it/5426)
* *Sensor:* Adafruit [MSA301 accelerometer](https://adafru.it/4344)
* *Language:* CircuitPython

### Pet water usage monitor
*waterusage-m5stickc*

Uses a scale to determine how much water is in a pet's bowl and send an alert
if the level gets too low.
* *Dev board:* M5 [M5StickC Plus](https://shop.m5stack.com/products/m5stickc-plus-esp32-pico-mini-iot-development-kit)
* *Sensor:* M5 [HX711 scale](https://shop.m5stack.com/products/mini-scales-unit-hx711)
* *Language:* C++ (Arduino)

### Audio/video alert
*audiovideoalert-atommatrix*

Uses LEDs and an audio player to show alerts, e.g. when the water level gets
too low.
* *Dev board:* M5 [M5 Atom Matrix](https://shop.m5stack.com/products/atom-matrix-esp32-development-kit)
* *Audio board:* M5 [M5 Audio Player Unit](https://shop.m5stack.com/products/audio-player-unit-n9301)
* *Language:* C++ (Arduino)
