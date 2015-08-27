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
#define LEVEL_COUNT 10

WaterSensor water_sensor(SEND_PIN, RECEIVE_PIN, LEVEL_COUNT);

Timer print_timer;

void setup() {
  Serial.begin(BAUD);
  water_sensor.setAccuracy(1000);
  water_sensor.startRealTime();

  // add or remove levels below and change the raw values to calibrate your sensor
  water_sensor.setLevel(1, 100);
  water_sensor.setLevel(2, 110);
  water_sensor.setLevel(3, 120);
  water_sensor.setLevel(4, 130);
  water_sensor.setLevel(5, 140);
  water_sensor.setLevel(6, 150);
  water_sensor.setLevel(7, 160);
  water_sensor.setLevel(8, 170);
  water_sensor.setLevel(9, 180);
  water_sensor.setLevel(10, 190);
  water_sensor.setMaxLevel(200);

  print_timer.setTimeout(10);
}

void loop() {
  // keep moving in real-time
  water_sensor.update();

  if(print_timer.isExpired()) {
    Serial.print("LEVEL: ");
    Serial.print(water_sensor.getActiveLevel());
    Serial.print("  RAW VALUE: ");
    Serial.println(water_sensor.getRawValue());
    print_timer.restart();
  }
}
