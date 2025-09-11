#include <Arduino.h>
//#include <M5Atom.h>
#include <M5Unified.h>
#include <unit_audioplayer.hpp>
#include <FastLED.h>
#include <faces.h>

AudioPlayerUnit audioplayer;

#define DATA_PIN 27
#define NUM_LEDS 25
CRGB leds[NUM_LEDS];


void setup()
{
    auto cfg = M5.config();
    M5.begin(cfg);
    Serial.begin(115200);
    M5.Power.setExtOutput(true);
    Serial.println("Starting up!");

    FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);


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
    audioplayer.setVolume(10);
    audioplayer.setPlayMode(AUDIO_PLAYER_MODE_ALL_LOOP);
    audioplayer.playAudio();

}

void loop()
{
    draw_face(leds, FACE_SAD);
    FastLED.show();
    delay(1000);

    draw_face(leds, FACE_HAPPY);
    FastLED.show();

    delay(1000);
}
