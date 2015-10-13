#Arduino Water Sensor Library
Measure and calibrate capacitance sensors to detect change in water levels from the outside of a plastic container. The example below uses a piece of aluminum foil, but you can attach the wire to anything conductive and calibrate the sensor accordingly.

This library measures relative capacitance so it is prone to interference. You can temporarily adjust the entire scale and then reset it when the capacitance disruption has stopped by using [setModifier()](#setmodifiervalue) and [resetModifier()](#resetmodifier). Look at the [Capacitance Library](https://github.com/alextaujenis/RBD_Capacitance) if you would like to know more about how capacitance is calculated.

##Installation
Download and install this Water Sensor Library along with these dependencies:

* [Arduino Capacitance Library](https://github.com/alextaujenis/RBD_Capacitance)
* [Arduino Threshold Library](https://github.com/alextaujenis/RBD_Threshold)

##Example Setup
0. Bridge a 10Meg Ohm resistor across the send (tx 1) and receive (rx 0) pins
0. Run a length of wire from the receive (rx 0) pin to a strip of aluminum foil
0. Tape the foil to the side of a water-tight non-conductive (plastic) container
0. Load up the example sketch and open a serial connection at 115200 baud
0. Pour water into your container and change the raw threshold values in the sketch to match the levels
0. Pour out the water, then compile and load the calibrated sketch back into the Arduino
0. Your water level is calibrated and ready to use

**Example Sketch:**

    #include <RBD_Capacitance.h>
    #include <RBD_Threshold.h>
    #include <RBD_WaterSensor.h>

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

#Documentation
##Public Methods

* [constructor(send\_pin, receive\_pin, level\_count)](#constructorsend_pin-receive_pin-level_count)
<hr />
* [setSampleSize(value)](#setsamplesizevalue)
* [setLevel(index, raw\_value)](#setlevelindex-raw_value)
* [setMaxLevel(raw\_value)](#setmaxlevelraw_value)
* [setModifier(value)](#setmodifiervalue)
* [resetModifier()](#resetmodifier)
<hr />
* [update()](#update)
* [getValue()](#getvalue)
* [getRawValue()](#getrawvalue)
* [getActiveLevel()](#getactivelevel)
* [isActiveLevel(value)](#isactivelevelvalue)
<hr />
* [onValueChange()](#onvaluechange)
* [onRawValueChange()](#onrawvaluechange)
* [onActiveLevelChange()](#onactivelevelchange)

##constructor(send\_pin, receive\_pin, level\_count)
Pass in integers for the send and receive pins to create a new instance of this class, along with an integer for the total number of levels the water sensor will detect. Example: if you want to detect low, medium, and high levels then level\_count should equal 3.

    RBD::WaterSensor water_sensor(1, 0, 3); // send, receive, level count

    void setup() {
      ...
    }

##setSampleSize(value)
Pass in an integer to change the number of readings taken to calculate the moving average [getRawValue()](#getrawvalue). This can be called inside of setup() or also safely at runtime inside of loop().

* Decrease Variation in readings by making this number larger: 5000
* Increase Variation in readings by making this number smaller: 100
* Default Value: 1000

Example:

    void setup() {
      water_sensor.setSampleSize(250);
    }

##setLevel(index, raw\_value)
Provide an integer for the level index and a raw value from [getRawValue()](#getrawvalue). The level index starts at one (it is not zero-based). You must also call [setMaxLevel()](#setmaxlevelraw_value) at the end to set an upper bounds of the last level.

    void setup() {
      water_sensor.setLevel(1, 120);
      water_sensor.setLevel(2, 154);
      water_sensor.setLevel(3, 187);
    }

##setMaxLevel(raw\_value)
Provide an integer from [getRawValue()](#getrawvalue) to set the upper-bounds threshold of the last level.

    void setup() {
      water_sensor.setMaxLevel(220);
    }

##setModifier(value)
Provide a positive or negative integer to temporarily adjust the water sensor threshold scale. The default value is 0.

For example; if you have calibrated the scale and turn on a motor near the sensor: all readings will need to be adjusted for the increased capacitance from the motor.

* If a running motor increases the difference in water sensor readings by a [getRawValue()](#getrawvalue) of +200
* Call [setModifier(-200)](#setmodifiervalue) on the water sensor when the motor turns on
* All of the calibrated water sensor thresholds will adjust -200
* The water sensor [getActiveLevel()](#getactivelevel) and [getValue()](#getvalue) will account for motor interference
* Call [resetModifier()](#resetmodifier) on the water sensor when the motor shuts off

Example:

    void loop() {
      if(...) { // adjust when motor is on
        water_sensor.setModifier(-200);
      }
      else {
        water_sensor.resetModifier();
      }

      water_sensor.update();

      // still works as expected with interference
      if(water_sensor.onActiveLevelChange()) {
        ...
      }
    }

##resetModifier()
Changes the [setModifier()](#setmodifiervalue) back to 0 and resets calibration of the water sensor threshold scale.

For example; if you have calibrated the scale and turn on a motor near the sensor: all readings will need to be adjusted for the increased capacitance from the motor.

* If a running motor increases the difference in water sensor readings by a [getRawValue()](#getrawvalue) of +200
* Call [setModifier(-200)](#setmodifiervalue) on the water sensor when the motor turns on
* All of the calibrated water sensor thresholds will adjust -200
* The water sensor [getActiveLevel()](#getactivelevel) and [getValue()](#getvalue) will account for motor interference
* Call [resetModifier()](#resetmodifier) on the water sensor when the motor shuts off

Example:

    void loop() {
      if(...) {
        water_sensor.setModifier(-200);
      }
      else { // reset when motor is off
        water_sensor.resetModifier();
      }

      water_sensor.update();

      // still works as expected with interference
      if(water_sensor.onActiveLevelChange()) {
        ...
      }
    }

##update()
Keep processing the readings and move this library along in real-time.

    void loop() {
      water_sensor.update();
    }

##getValue()
Returns the capacitance sensor reading after being adjusted with the value given to [setModifier()](#setmodifiervalue).

This will return [getRawValue()](#getrawvalue) if you have not used [setModifier()](#setmodifiervalue) yet, or if you have called [resetModifier()](#resetmodifier).

    void loop() {
      water_sensor.update();

      if(water_sensor.onValueChange()) {
        Serial.println(water_sensor.getValue());
      }
    }

##getRawValue()
Returns the raw capacitance sensor reading and ignores any value that has been passed to [setModifier()](#setmodifiervalue).

    void loop() {
      water_sensor.update();

      if(water_sensor.onRawValueChange()) {
        Serial.println(water_sensor.getRawValue());
      }
    }

##getActiveLevel()
Returns the current water level.

* 0 means there is no water
* 1 means the water is touching the first level
* 2 means the water is touching the second level
* n means the water is touching the nth level
* sizeof(n) + 1 means the water is above the max level
* -1 if the value was not found

Example:

    void loop() {
      water_sensor.update();

      if(water_sensor.onActiveLevelChange()) {
        Serial.println(water_sensor.getActiveLevel());
      }
    }

##isActiveLevel(value)
Provide an integer and this will return true if it's equal to the current [getActiveLevel()](#getactivelevel).

    void loop() {
      water_sensor.update();

      if(water_sensor.onActiveLevelChange()) {
        if(water_sensor.isActiveLevel(1)) {
          Serial.println("First level active");
        }
      }
    }

##onValueChange()
This method will return true once the sensor [getValue()](#getvalue) changes. It will then return false until the reading changes to a different value again.

    void loop() {
      water_sensor.update();

      if(water_sensor.onValueChange()) {
        // code only runs once per event
        Serial.println(water_sensor.getValue());
      }
    }

##onRawValueChange()
This method will return true once the sensor [getRawValue()](#getrawvalue) changes. It will then return false until the reading changes to a different value again.

    void loop() {
      water_sensor.update();

      if(water_sensor.onRawValueChange()) {
        // code only runs once per event
        Serial.println(water_sensor.getRawValue());
      }
    }


##onActiveLevelChange()
This method will return true once the sensor [getActiveLevel()](#getactivelevel) changes. It will then return false until the level changes to a different value again.

    void loop() {
      water_sensor.update();

      if(water_sensor.onActiveLevelChange()) {
        // code only runs once per event
        Serial.println(water_sensor.getActiveLevel());
      }
    }

#License
This code is available under the [MIT License](http://opensource.org/licenses/mit-license.php).