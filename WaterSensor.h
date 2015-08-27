// Arduino Water Sensor Library - Measure and calibrate capacitance-based water level sensors
// Copyright 2015 Alex Taujenis
// MIT License

#ifndef WATER_SENSOR
#define WATER_SENSOR

#include <Arduino.h>
#include <WaterSensor.h>
#include <Capacitance.h>
#include <Threshold.h>
#include <Timer.h>

class WaterSensor {
  public:
    WaterSensor(int send_pin, int receive_pin, int level_count);
    void setAccuracy(int value);             // set the moving average number of readings
    void setRefreshRate(int hertz);          // set the number of times to update each second
    void startRealTime();                    // refresh the sensor with no limit
    void update();                           // keep things moving in real-time
    int getRawValue();                       // get the raw capacitance value
    void setLevel(int index, int raw_value); // assign the raw capacitance value to a level
    void setMaxLevel(int raw_value);         // set the raw capacitance upper bounds for the last level
    int getActiveLevel();                    // return the current level of water based upon the calibration
  private:
    int _level_count;         // total number of water levels for the sensor
    int _refresh_hertz;       // max number of readings / second
    bool _real_time;          // sensor collects at max speed
    int _raw_value;           // holds the raw capacitance value
    int _refreshRateDelay();  // calculate ms timeout based upon hertz
    bool _isRealTime();       // if this is reading at max speed
    Capacitance _cap_sensor;  // capacitance library helper object
    Threshold _threshold;     // threshold library helper object
    Timer _refresh_timer;     // timer library helper object
};

#endif