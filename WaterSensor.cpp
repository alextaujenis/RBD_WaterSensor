// Arduino Water Sensor Library - Measure and calibrate capacitance-based water level sensors
// Copyright 2015 Alex Taujenis
// MIT License

#include <Arduino.h>
#include <WaterSensor.h>
#include <Capacitance.h>
#include <Threshold.h>
#include <Timer.h>

WaterSensor::WaterSensor(int send_pin, int receive_pin, int level_count)
: _cap_sensor(send_pin, receive_pin), _threshold(level_count), _refresh_timer() {
  _level_count = level_count;
}

void WaterSensor::setAccuracy(int value) {
  // pass-through to the capacitance library
  _cap_sensor.setSampleSize(value);
}

void WaterSensor::setRefreshRate(int hertz) {
  _refresh_hertz = hertz;
  _real_time     = false;

  if(_shouldBeRunning()) {
    _refresh_timer.setHertz(_refresh_hertz);
    _cap_sensor.start();
  }
}

void WaterSensor::startRealTime() {
  _real_time = true;
  _cap_sensor.start();
}

void WaterSensor::update() {
  if(_shouldBeRunning()) {
    _cap_sensor.update();

    if(_isRealTime()) {
      // go full speed
      if(_cap_sensor.isFinished()) {
        _raw_value = _cap_sensor.getValue();
        _cap_sensor.start();
      }
    }
    else {
      // limit the number of times / second
      if(_refresh_timer.isExpired()) {
        if(_cap_sensor.isFinished()) {
          _raw_value = _cap_sensor.getValue();
          _cap_sensor.start();
          _refresh_timer.restart();
        }
      }
    }
  }
}

int WaterSensor::getRawValue() {
  return _raw_value;
}

void WaterSensor::setLevel(int index, int raw_value) {
  if(index > 0 && index <= _level_count) {
    _threshold.setLevel(index, raw_value);
  }
}

void WaterSensor::setMaxLevel(int raw_value) {
  // pass-through to the threshold library
  _threshold.setMaxLevel(raw_value);
}

int WaterSensor::getActiveLevel() {
  return _threshold.computeLevel(getRawValue());
}


// private

bool WaterSensor::_shouldBeRunning() {
  return _isRealTime() || _refresh_hertz != 0;
}

bool WaterSensor::_isRealTime() {
  return _real_time;
}