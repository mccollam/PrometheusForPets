# Basic libraries:
import time
import board
import adafruit_msa3xx

# WiFi support:
from os import getenv
import ipaddress
import wifi

# Webserver for Prometheus:
import socketpool
from adafruit_httpserver import Server, Request, Response, Redirect

### Set up the hardware
i2c = board.STEMMA_I2C()
msa = adafruit_msa3xx.MSA301(i2c)

### Connect to wifi
ssid = getenv("CIRCUITPY_WIFI_SSID")
password = getenv("CIRCUITPY_WIFI_PASSWORD")
print("Connecting to wifi")
try:
	wifi.radio.connect(ssid, password)
except TypeError:
	print("Could not find wifi credentials in settings.toml")
	raise
print(f"Connected!  IP address: {wifi.radio.ipv4_address}")


### Set up a server for Prometheus to scrape
pool = socketpool.SocketPool(wifi.radio)
server = Server(pool, debug=True)
server.start(str(wifi.radio.ipv4_address), port=8080)

@server.route("/metrics")
def metrics_handler(request: Request):
	global entries, exits
	
	data = (
		   '# HELP door_use_counter number of uses of the pet door\n'
		+  '# TYPE door_use_counter counter\n'
		+ f'door_use_counter_total{{door="front_dog_door", type="entries"}} {entries}\n'
		+ f'door_use_counter_total{{door="front_dog_door", type="exits"}} {exits}\n'
		)
		
	return Response(request, data, content_type="text/plain; version=0.0.4")

@server.route("/")
def metrics_redirect(request: Request):
	return Redirect(request, "/metrics")


### Main code starts here

# Count entries and exits (and the previous state of the door)
entries = 0
exits = 0
prev_state = None

while True:
	# Look for uses of the pet door
	x, y, z = msa.acceleration
	if z > 4: # Door is in 'entry' position
		if prev_state != "entry":
			print("Entry detected")
			entries = entries + 1
			prev_state = "entry"
	elif z < -4: # Door is in 'exit' position
		if prev_state != "exit":
			print("Exit detected")
			exits = exits + 1
			prev_state = "exit"
	else: # Door is closed
		prev_state = "closed"
	
	# Serve the data to Prometheus
	try:
		server.poll()
	except Exception as e:
		print("Error: ", e)

	# Sleep a bit.  This prevents a tight loop in the webserver and also
	# acts as a cheap 'debounce' on the door so we don't see a bunch of
	# entries or exits when it's right on the margin of the threshold
	time.sleep(0.25)

