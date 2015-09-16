// Arduino Water Sensor Library Example - See the raw and computed output from a three-level water sensor
// Copyright 2015 Alex Taujenis
// MIT License

#include <RBD_WaterSensor.h>
#include <RBD_Capacitance.h>
#include <RBD_Threshold.h>
#include <RBD_Timer.h>

#define BAUD        115200
#define SEND_PIN    1
#define RECEIVE_PIN 0
// change this to how many levels your sensor should read
#define LEVEL_COUNT 3

// this helps print only the changed values to serial
int past_value  = 0;

// you can define many real-time sensors and they will all play nice
RBD::WaterSensor water_sensor(SEND_PIN, RECEIVE_PIN, LEVEL_COUNT);
// WaterSensor water_sensor2(5, 6, 10);

void setup() {
  // set up a serial connection
  Serial.begin(BAUD);

  // accuracy is how many samples to take for the moving average
  water_sensor.setAccuracy(1000);
  // refresh rate is in hertz, or readings / second
  water_sensor.setRefreshRate(2);

  // if you want the sensor to read as fast as possible:
  // lower the setAccuracy(1) above
  // comment out the line setRefreshRate(2) above
  // and uncomment the line below
  // water_sensor.startRealTime();

  // add / remove levels below and change the raw values to calibrate your sensor
  // you must change LEVEL_COUNT (above) to the number of levels you provide
  water_sensor.setLevel(1, 120); // first level:  [120 - 153]
  water_sensor.setLevel(2, 154); // second level: [154 - 186]
  water_sensor.setLevel(3, 187); // third level:  [187 - 220]
  water_sensor.setMaxLevel(220);
}

void loop() {
  // keep moving in real-time
  water_sensor.update();

  // printing to serial as fast as possible will slow the main loop() enough to impair sensor performance
  // check if the sensor value has changed before printing to serial
  if(water_sensor.getRawValue() != past_value) {
    // print both the computed level and the raw value from the sensor (this will help you calibrate the sensor)
    printValues(water_sensor.getActiveLevel(), water_sensor.getRawValue());
    // save the 'current' sensor value as the 'past value' for the next loop
    past_value = water_sensor.getRawValue();
  }
}

// helper method to print values to serial
void printValues(int active_level, int raw_value) {
  Serial.print("ACTIVE LEVEL: ");
  Serial.print(active_level);
  Serial.print("  RAW VALUE: ");
  Serial.println(raw_value);
}