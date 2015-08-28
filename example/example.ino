// Arduino Water Sensor Library Example - See the raw and computed output from a three-level water sensor
// Copyright 2015 Alex Taujenis
// MIT License

#include <WaterSensor.h>
#include <Capacitance.h>
#include <Threshold.h>
#include <Timer.h>

#define BAUD        115200
#define SEND_PIN    1
#define RECEIVE_PIN 0
#define LEVEL_COUNT 3

int past_value  = 0;

WaterSensor water_sensor(SEND_PIN, RECEIVE_PIN, LEVEL_COUNT);

void setup() {
  Serial.begin(BAUD);
  water_sensor.setAccuracy(1000);  // how many samples to take for a moving average
  water_sensor.setRefreshRate(2);  // hertz, or readings / second
  // water_sensor.startRealTime(); // uncomment to go as fast as the sensor can read

  // add or remove levels below and change the raw values to calibrate your sensor
  water_sensor.setLevel(1, 120);
  water_sensor.setLevel(2, 154);
  water_sensor.setLevel(3, 187);
  water_sensor.setMaxLevel(220);
}

void loop() {
  water_sensor.update();

  if(water_sensor.getRawValue() != past_value) {
    printValues(water_sensor.getActiveLevel(), water_sensor.getRawValue());
    past_value = water_sensor.getRawValue();
  }
}

void printValues(int active_level, int raw_value) {
  Serial.print("ACTIVE LEVEL: ");
  Serial.print(active_level);
  Serial.print("  RAW VALUE: ");
  Serial.println(raw_value);
}