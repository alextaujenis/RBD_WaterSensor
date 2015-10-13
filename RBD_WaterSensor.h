// Arduino RBD Water Sensor Library - Measure and calibrate capacitance-based water level sensors
// Copyright 2015 Alex Taujenis
// MIT License

#ifndef RBD_WATER_SENSOR
#define RBD_WATER_SENSOR

#include <Arduino.h>
#include <RBD_Capacitance.h>
#include <RBD_Threshold.h>

namespace RBD {
  class WaterSensor {
    public:
      WaterSensor(int send_pin, int receive_pin, int level_count);
      void update();
      void setAccuracy(int value);
      void setLevel(int index, int raw_value);
      void setMaxLevel(int raw_value);
      int getRawValue();
      int getActiveLevel();
      bool isActiveLevel(int value);
      bool onRawValueChange();
      bool onActiveLevelChange();
    private:
      int _level_count;
      int _raw_value;
      int _temp_raw_value;
      int _prev_raw_value1;
      int _prev_raw_value2;
      int _saved_level;
      int _prev_level;
      int _temp_level;
      bool _raw_value_changed = false;
      Capacitance _cap_sensor;
      Threshold _threshold;
  };
}
#endif