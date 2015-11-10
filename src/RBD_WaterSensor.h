// Arduino Water Sensor Library v2.0.0 - Measure and calibrate capacitance water sensors.
// https://github.com/alextaujenis/RBD_WaterSensor
// Copyright 2015 Alex Taujenis
// MIT License

#ifndef RBD_WATER_SENSOR
#define RBD_WATER_SENSOR

#include <Arduino.h>
#include <RBD_Capacitance.h> // https://github.com/alextaujenis/RBD_Capacitance
#include <RBD_Threshold.h>   // https://github.com/alextaujenis/RBD_Threshold

namespace RBD {
  class WaterSensor {
    public:
      WaterSensor(int send_pin, int receive_pin, int level_count);
      void update();
      void setSampleSize(int value); // default 1000
      void setLevel(int index, int raw_value);
      void setMaxLevel(int raw_value);
      void setModifier(int value);
      void resetModifier();
      int getValue();
      int getRawValue();
      int getActiveLevel();
      bool onValueChange();
      bool onRawValueChange();
      bool onActiveLevelChange();
    private:
      int _level_count;
      int _raw_value;
      int _temp_value;
      int _prev_value1;
      int _prev_value2;
      int _saved_level;
      int _prev_level;
      int _temp_level;
      int _raw_value_modifier;
      bool _raw_value_changed = false;
      Capacitance _cap_sensor;
      Threshold _threshold;
  };
}
#endif