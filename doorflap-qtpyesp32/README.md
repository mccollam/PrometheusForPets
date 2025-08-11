# Pet Door Flap Monitor
Counts entries and exits on a pet door using an accelerometer and exposes them as Prometheus metrics

## Hardware
### Bill of Materials (BOM)
* [Adafruit QT Py ESP32-S3 with STEMMA (8GB flash, no PSRAM)](https://www.adafruit.com/product/5426)
* [Adafruit MSA301 Accelerometer](https://www.adafruit.com/product/4344)
* [Adafruit STEMMA QT cable](https://www.adafruit.com/product/4210) (any length)

### Assembly
Connect the accelerometer to the QT Py board witht the STEMMA cable, being
sure to properly align the connector.
(It has guide pins on the side -- if it doesn't feel like it fits, you're
probably inserting it upside-down.)


## Software
This project is written in [CircuitPython](https://circuitpython.org/),
Adafruit's miniature Python implementation for microcontrollers.

### Dependencies
It relies on the following libraries from the
[Adafruit CircuitPython Library Bundle](https://github.com/adafruit/Adafruit_CircuitPython_Bundle/releases):
* adafruit_bus_device
* adafruit_httpserver
* adafruit_register
* adafruit_msa3xx.mpy

### Installation
1. First, ensure that the QT Py board has the CircuitPython runtime installed.
You can verify this by plugging the board into your computer.  You should see a
USB drive called `CIRCUITPY` appear shortly after connecting the cable.  If you
do not see this, follow the instructions to
[install CircuitPython on the QT Py](https://learn.adafruit.com/adafruit-qt-py-esp32-s3/circuitpython-2).
2. Add the dependencies listed above.  Download the CircuitPython Library Bundle
and extract the listed folders and file.  Copy them to the `/lib` folder on the
`CIRCUITPY` drive.
3. Set up your WiFi credentials.  Make a copy of `settings.toml.example` called
`settings.toml`.  Edit `settings.toml` and put in your own WiFi credentials.
Copy this to the root of the `CIRCUITPY` drive.
4. Copy `code.py` to the root of the `CIRCUITPY` drive.  (Optionally take a
look in this file first to see what it is doing!)

### Door detection
We assume that the accelerometer is attached to the door with the empty STEMMA
connector pointing down.  When attached like this, the Z axis should be close
to 0 when the door is closed.

When the door is opened in the positive direction, the Z axis increases
up to a maximum of ~9.8, as it's measuring acceleration in m/s, and ~9.8 m/s
is the gravitational acceleration on the surface of Earth.  (If you use this
project on another planet, you'll have to change some values and also I really
want to hear about it!)

When the door is opened in the negative direction, the Z axis decreases to
a minimum of ~-9.8 (because it's experiencing gravitational acceleration
in the opposite direction).  If you attach this to your pet door and see that
the entries/exits are reversed, you can either flip the board over and reattach
or change the signs in the code.

Once the board detects a threshold of +/- 4m/s, it counts the door as having
been entered or exited.  This number was picked more or less arbitrarily,
and means that the door has gotten to nearly a 45 degree angle.  If you have a
particularly small pet you might need to adjust this down a bit, and if you
have a really floppy door you might need to nudge it up.

### Code Information
This project does not use a full Prometheus client library.  It doesn't
implement most of what Prometheus can provide or do.  Instead, it simply
keeps track of two integer counters (entries and exits).

When an HTTP GET request is made to the `/metrics` endpoint on the webserver,
the results are formatted the way Prometheus expects counters.

While more full-featured Prometheus libraries for Python exist, in this
case I wanted to show how simple it can be to create metrics in the Prometheus
format without much code or overhead (beyond a webserver).

## Usage
Once the CircuitPython environment, the libraries, and the code are installed
on the QT Py, connect it to power via USB-C.  It will connect to your WiFi
and count the number of entries and exits to the pet door.  These will be
available in Prometheus format at `http://your_qt_py_ip_address:8080/metrics`.

### Debugging and finding the IP address
While it's possible to log into your router (or DHCP server) to find the IP
address of the QT Py, it's simpler to see it when the device is turned on.

To do this, connect your QT Py to your computer via a USB-C cable.  (It 
*must* be a cable that carries data, not just a power cable!)  Then connect
to the serial device using these settings:
* Speed: 115200
* Bits: 8
* Stop bits: 1
* Pairity: None

When you connect you'll see a line like
```0;🐍192.168.1.100 | BLE:Off | code.py | 9.2.8```
The IP address is listed after the 🐍 emoji.

How you do this will vary depending on the OS on your computer.  On Linux
(and possibly MacOS), if your QT Py is connected as `/dev/ttyACM0` this would
look like
```minicom -b 115200 -D /dev/ttyACM0```
or
```screen /dev/ttyACM0 115200```

Your favorite search engine can help more with connecting to a serial device
from other OSes.