// Arduino Water Sensor Library Example - See the raw and computed output from a three-level water sensor
// Copyright 2015 Alex Taujenis
// MIT License

#include <WaterSensor.h>
#include <Capacitance.h>
#include <Threshold.h>
#include <Timer.h>

#define BAUD 115200

WaterSensor water_sensor(1, 0, 3); // send pin, receive pin, detect 3 levels

void setup() {
  Serial.begin(BAUD);
  water_sensor.setAccuracy(5000); // make it accurate
  water_sensor.setRefreshRate(2); // update twice a second

  // add or remove levels below and change the raw values to calibrate your sensor
  water_sensor.setLevel(1, 0);    // 0 - 33
  water_sensor.setLevel(2, 34);   // 34 - 66
  water_sensor.setLevel(3, 67);   // 67 - max
  water_sensor.setMaxLevel(100);  // max = 100
}

void loop() {
  // keep moving in real-time
  water_sensor.update();

  Serial.print("LEVEL: ");
  Serial.print(water_sensor.getActiveLevel());

  Serial.print("  RAW VALUE: ");
  Serial.println(water_sensor.getRawValue());
}
