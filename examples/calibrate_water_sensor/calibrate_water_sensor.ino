// Arduino Water Sensor Library v1.0.0 Example - Calibrate a three-level water sensor.
// https://github.com/alextaujenis/RBD_WaterSensor
// Copyright 2015 Alex Taujenis
// MIT License

#include <RBD_Capacitance.h> // https://github.com/alextaujenis/RBD_Capacitance
#include <RBD_Threshold.h>   // https://github.com/alextaujenis/RBD_Threshold
#include <RBD_WaterSensor.h> // https://github.com/alextaujenis/RBD_WaterSensor

RBD::WaterSensor water_sensor(1, 0, 3); // send, receive, level count

void setup() {
  Serial.begin(115200);
  water_sensor.setLevel(1, 120);
  water_sensor.setLevel(2, 154);
  water_sensor.setLevel(3, 187);
  water_sensor.setMaxLevel(220);
}

void loop() {
  water_sensor.update();

  if(water_sensor.onRawValueChange()) {
    Serial.print("Active Level: ");
    Serial.print(water_sensor.getActiveLevel());
    Serial.print("  ---  ");
    Serial.print("Raw Value: ");
    Serial.println(water_sensor.getRawValue());
  }
}