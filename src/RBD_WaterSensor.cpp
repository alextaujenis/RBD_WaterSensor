// Arduino Water Sensor Library v2.0.0 - Measure and calibrate capacitance water sensors.
// https://github.com/alextaujenis/RBD_WaterSensor
// Copyright 2015 Alex Taujenis
// MIT License

#include <Arduino.h>
#include <RBD_Capacitance.h> // https://github.com/alextaujenis/RBD_Capacitance
#include <RBD_Threshold.h>   // https://github.com/alextaujenis/RBD_Threshold
#include <RBD_WaterSensor.h> // https://github.com/alextaujenis/RBD_WaterSensor

namespace RBD {

  WaterSensor::WaterSensor(int send_pin, int receive_pin, int level_count)
  : _cap_sensor(send_pin, receive_pin), _threshold(level_count){
    _level_count = level_count;
    setSampleSize(1000); // default 1000
    _cap_sensor.start();
  }

  void WaterSensor::update() {
    _cap_sensor.update();

    if(_cap_sensor.isFinished()) {
      _raw_value = _cap_sensor.getValue();

      // set the flag if the value has changed
      if(_prev_value1 != _raw_value) {
        _prev_value1 = _raw_value;
        _raw_value_changed = true;
      }

      _cap_sensor.start();
    }
  }

  void WaterSensor::setSampleSize(int value) {
    _cap_sensor.setSampleSize(value);
  }

  void WaterSensor::setLevel(int index, int raw_value) {
    _threshold.setLevel(index, raw_value);
  }

  void WaterSensor::setMaxLevel(int raw_value) {
    _threshold.setMaxLevel(raw_value);
  }

  int WaterSensor::getRawValue() {
    return _raw_value;
  }

  int WaterSensor::getActiveLevel() {
    // freeze the raw value for comparison
    _temp_value = getValue();

    // memozie the threshold for performance reasons
    if(_prev_value2 != _temp_value) {
      _prev_value2 = _temp_value;
      _saved_level = _threshold.computeLevel(_temp_value);
    }

    // return the current level
    return _saved_level;
  }

  bool WaterSensor::onValueChange() {
    return onRawValueChange();
  }

  bool WaterSensor::onRawValueChange() {
    if(_raw_value_changed) {
      _raw_value_changed = false;
      return true;
    }
    return false;
  }

  bool WaterSensor::onActiveLevelChange() {
    // freeze the level for comparison
    _temp_level = getActiveLevel();

    // compare it
    if(_prev_level != _temp_level) {
      _prev_level = _temp_level;
      return true;
    }
    return false;
  }

  void WaterSensor::setModifier(int value) {
    _raw_value_modifier = value;
  }

  void WaterSensor::resetModifier() {
    setModifier(0);
  }

  int WaterSensor::getValue() {
    return _raw_value + _raw_value_modifier;
  }
}