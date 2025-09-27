#include <Arduino.h>
#include <M5StickCPlus.h>
#include <M5UnitWeightI2C.h>
#include <Wire.h>
#include <PromLokiTransport.h>
#include <PrometheusArduino.h>

#include "credentials.h"  // Wifi and Grafana Cloud credentials
#include "certificates.h" // Root CA for Grafana certificates

/*** Some measurements taken with the water bowl (in grams) ***/
#define BOWL_WEIGHT 40
#define WATER_ONE_THIRD 260
#define WATER_TWO_THIRDS 520
#define WATER_FULL 820

M5UnitWeightI2C scale;

PromLokiTransport transport;
PromClient client(transport);
WriteRequest req(3, 1024);
TimeSeries ts_weight(5, "weight_grams", "{job=\"pet_monitor\",device=\"water_scale\"}");
TimeSeries ts_uptime(5, "uptime_milliseconds_total", "{job=\"pet_monitor\",device=\"water_scale\"}");
TimeSeries ts_heap(5, "heap_free_bytes", "{job=\"pet_monitor\",device=\"water_scale\"}");

void setup()
{
    /*** Set up the hardware ***/
    M5.begin();            // Set up the board
    Serial.begin(115200);  // Serial output
    Wire.begin(32, 33);    // I2C

    Serial.println("Starting up!");

    /*** Set up the M5.Lcd ***/
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setRotation(3);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(10, 10);
    M5.Lcd.setTextColor(ORANGE, BLACK);
    M5.Lcd.printf("==  Grafana Labs ==");
    M5.Lcd.setTextColor(WHITE, BLACK);
    
    /*** Set up the scale ***/
    Serial.print("Setting up scale... ");
    M5.Lcd.setCursor(10,40);
    M5.Lcd.printf("Scale ");
    while (!scale.begin(&Wire, 32, 33, DEVICE_DEFAULT_ADDR, 100000U))
    {
        Serial.println("I2C error! ");
        delay(100);
    }
    Serial.println("done!");
    M5.Lcd.printf("found!");


    /*** Set up wifi ***/
    Serial.print("Setting up wifi... ");
    M5.Lcd.setCursor(10, 70);
    M5.Lcd.printf("Wifi ");


    // Uncomment the below for Grafana Cloud
    //transport.setUseTls(true);
    //transport.setCerts(grafanaCert, strlen(grafanaCert));
    transport.setWifiSsid(WIFI_SSID);
    transport.setWifiPass(WIFI_PASSWORD);
    transport.setDebug(Serial);
    if (!transport.begin())
    {
        Serial.println(transport.errmsg);
        while (true);
    }
    Serial.println("done!");
    M5.Lcd.printf("connected!");
    delay(1000);

    /*** Set up the Prometheus client ***/
    Serial.print("Setting up Prometheus client... ");
    client.setUrl(GC_URL);
    client.setPath((char*)GC_PATH);
    client.setPort(GC_PORT);

    // Uncomment the below for Grafana Cloud
    //client.setUser(GC_USER);
    //client.setPass(GC_PASS);
    client.setDebug(Serial);
    if (!client.begin())
    {
        Serial.println(client.errmsg);
        while (true);
    }
    Serial.println("done!");

    /*** Add the timeseries to the write request ***/
    req.addTimeSeries(ts_weight);
    req.addTimeSeries(ts_uptime);
    req.addTimeSeries(ts_heap);
    req.setDebug(Serial);
}


void loop()
{
    int64_t time = transport.getTimeMillis();
    float weight = scale.getWeight();

    /*** Show the measurement on the serial line for debugging ***/
    Serial.print("Time: ");
    Serial.println(time);
    Serial.print("Weight: ");
    Serial.printf("%.2fg\n", weight);

    /*** Show the measurement on the display ***/
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextSize(5);
    M5.Lcd.setCursor(10, 10);
    M5.Lcd.printf("%.1fg", weight);
    M5.Lcd.setCursor(10, 70);
    if (weight < WATER_ONE_THIRD + BOWL_WEIGHT)
        M5.Lcd.printf("Empty!");
    else if (weight < WATER_TWO_THIRDS + BOWL_WEIGHT)
        M5.Lcd.printf("Good");
    else
        M5.Lcd.printf("Full!");

    /*** Add the measurement ***/
    if (!ts_weight.addSample(time, weight))
        Serial.println(ts_weight.errmsg);
    if (!ts_uptime.addSample(time, millis()))
        Serial.println(ts_uptime.errmsg);
    if (!ts_heap.addSample(time, freeMemory()))
        Serial.println(ts_heap.errmsg);

    /*** Send the measurement ***/
    PromClient::SendResult res = client.send(req);
    if (!res == PromClient::SendResult::SUCCESS)
        Serial.println(client.errmsg);
    else
        ts_weight.resetSamples();
        ts_uptime.resetSamples();
        ts_heap.resetSamples();

    /*** Wait 5 seconds before taking next measurement ***/
    delay(5000);
}
