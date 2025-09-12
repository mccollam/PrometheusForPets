#include <Arduino.h>
//#include <M5Atom.h>
#include <WiFi.h>
#include <M5Unified.h>
#include <unit_audioplayer.hpp>
#include <FastLED.h>
#include <MQTTClient.h>
#include <faces.h>
#include <credentials.h>

// Audio player
AudioPlayerUnit audioplayer;

// LED matrix
#define DATA_PIN 27
#define NUM_LEDS 25
CRGB leds[NUM_LEDS];

// MQTT client
WiFiClient net;
MQTTClient mqtt;

// Track whether we're alerting or not
bool alerting = false;

// Forward definition of function to handle MQTT messages
void mqttReceived(String &topic, String &payload);

void mqttReceived(String &topic, String &payload)
{
    if (topic == MQTT_TOPIC)
    {
        alerting = payload;            
    }
}

void setup()
{
    // Set up the board
    auto cfg = M5.config();
    M5.begin(cfg);
    Serial.begin(115200);
    M5.Power.setExtOutput(true);
    Serial.println("Starting up!");

    // Set up the LED matrix
    FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);

    // Set up WiFi
    Serial.print("Connecting to WiFi...");
    draw_face(leds, FACE_CONNECTING);
    delay(1000);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println(" connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    draw_face(leds, FACE_NONE);

    // Set up the audio player
    int8_t port_a_pin1 = -1, port_a_pin2 = -1;
    port_a_pin1 = M5.getPin(m5::pin_name_t::port_a_pin1);
    port_a_pin2 = M5.getPin(m5::pin_name_t::port_a_pin2);
    Serial.printf("getPin: RX:%d TX:%d\n", port_a_pin1, port_a_pin2);
    Serial.print("Starting audio player...");
    while (!audioplayer.begin(&Serial1, port_a_pin1, port_a_pin2))
    {
        Serial.print(".");
        delay(500);
    }
    Serial.println(" done!");
    audioplayer.setVolume(20);
    audioplayer.setPlayMode(AUDIO_PLAYER_MODE_ALL_LOOP);
    audioplayer.playAudio();

    // Set up the MQTT client
    Serial.print("Connecting to MQTT server...");
    mqtt.begin(MQTT_BROKER, net);
    mqtt.onMessage(mqttReceived);
    
    while (!mqtt.connect("water-alerter"))
    {
        delay(500);
        Serial.print(".");
    }
    mqtt.subscribe(MQTT_TOPIC);
    Serial.println(" connected!");
    

}

void loop()
{
    if (alerting)
    {
        draw_face(leds, FACE_SAD);
        audioplayer.playAudio();
    }
    else
    {
        draw_face(leds, FACE_HAPPY);
        FastLED.show();
        audioplayer.stopAudio();
    }
    delay(1000);
}
