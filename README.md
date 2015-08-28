#Arduino Water Sensor Library
Measure and calibrate capacitance water level sensors in real-time.

This library requires that you don't use delay() or interrupts to manage program flow. Check out this [Arduino Timer Library](https://github.com/alextaujenis/Timer) if you are managing millis() by hand.

##Installation
Download and install this Water Sensor Library along with these dependencies:

* [Arduino Capacitance Library](https://github.com/alextaujenis/Capacitance)
* [Arduino Threshold Library](https://github.com/alextaujenis/Threshold)
* [Arduino Timer Library](https://github.com/alextaujenis/Timer)

##Example Setup
0. Bridge a 10Meg Ohm resistor across the send (tx 1) and receive (rx 0) pins
0. Run a length of wire from the receive (rx 0) pin to a strip of aluminum foil
0. Tape the foil to the side of a water-tight non-conductive (plastic) container
0. Load up the example sketch and open a serial connection at 115200 baud
0. Pour water into your container and change the raw threshold values in the sketch to match the levels
0. Pour out the water, then compile and load the calibrated sketch back into the Arduino
0. Your water level is calibrated and ready to use

**[example.ino](https://github.com/alextaujenis/WaterSensor/blob/master/example/example.ino) <- comments included**

    #include <WaterSensor.h>
    #include <Capacitance.h>
    #include <Threshold.h>
    #include <Timer.h>

    #define BAUD        115200
    #define SEND_PIN    1
    #define RECEIVE_PIN 0
    #define LEVEL_COUNT 3

    int past_value  = 0;

    WaterSensor water_sensor(SEND_PIN, RECEIVE_PIN, LEVEL_COUNT);

    void setup() {
      Serial.begin(BAUD);
      water_sensor.setAccuracy(1000);
      water_sensor.setRefreshRate(2);
      water_sensor.setLevel(1, 120);
      water_sensor.setLevel(2, 154);
      water_sensor.setLevel(3, 187);
      water_sensor.setMaxLevel(220);
    }

    void loop() {
      water_sensor.update();

      if(water_sensor.getRawValue() != past_value) {
        printValues(water_sensor.getActiveLevel(), water_sensor.getRawValue());
        past_value = water_sensor.getRawValue();
      }
    }

    void printValues(int active_level, int raw_value) {
      Serial.print("ACTIVE LEVEL: ");
      Serial.print(active_level);
      Serial.print("  RAW VALUE: ");
      Serial.println(raw_value);
    }

**Warning(s):**

* Consumer electronics will alter with the readings if they are high-powered or too close to the sensor
* The sensor will be less accurate when you are near it, so stay away during calibration
* You should calibrate this sensor when the container is sitting exactly where it will be used
* Moving the sensor after calibration may throw off the readings because of differences in ambient electrical energy

#Documentation
##Public Methods

* [constructor(send\_pin, receive\_pin, level\_count)](#constructorsend_pin-receive_pin-level_count)
* [setAccuracy(value)](#setaccuracyvalue)
* [setRefreshRate(hertz)](#setrefreshratehertz)
* [startRealTime()](#startrealtime)
* [update()](#update)
* [getRawValue()](#getrawvalue)
* [setLevel(index, raw\_value)](#setlevelindex-raw_value)
* [setMaxLevel(raw\_value)](#setmaxlevelraw_value)
* [getActiveLevel()](#getactivelevel)

##constructor(send\_pin, receive\_pin, level\_count)
Pass in integers for the send and receive pins to create a new instance of this class, along with an integer for the total number of levels your water sensor will detect. Example: if you want to detect low, medium, and high levels then the level\_count should be 3.

    WaterSensor water_sensor(1, 0, 3);

##setAccuracy(value)
Provide an integer of how many readings to average before reporting a value. Increasing the value will increase accuracy and take longer to compute. Decreasing the value will lower accuracy and take a shorter time to compute. This can possibly affect the actual refresh rate to make it take slower readings. Don't worry, you can be as accurate as you'd like because this is done in a non-blocking manner and it won't affect your main loop() performance.

    water_sensor.setAccuracy(1000);

##setRefreshRate(hertz)
Set the maximum number of times the sensor should update per second. This is meant to be used to limit the number of readings taken per second. Calling this method will kick the sensor out of real-time readings if you have previously called [startRealTime()](#startrealtime). Set this value to 0 to completely stop the sensor from taking readings. If you make this faster than the sensor can read values (because of increasing accuracy): the library will naturally settle at taking readings in real-time.

    water_sensor.setRefreshRate(2); // about every 500ms

##startRealTime()
Start collecting readings as fast as possible and remove the [setRefreshRate()](#setrefreshratehertz) limit.

    water_sensor.startRealTime()

##update()
Keep the water sensor reading in real-time. This must be called continuously from within loop() to get water level readings.

    water_sensor.update();

##getRawValue()
Get the raw sensor value in order to calibrate each level with [setLevel()](#setlevelindex-raw_value).

    water_sensor.getRawValue()

##setLevel(index, raw\_value)
Add a value for the lower bounds of each level.

    water_sensor.setLevel(1, 0);  // 0 - 33
    water_sensor.setLevel(2, 34); // 34 - 66
    water_sensor.setLevel(3, 67); // 67 - max

##setMaxLevel(raw\_value)
Add a value for the upper bounds of the last level.

    water_sensor.setMaxLevel(100);

##getActiveLevel()
Returns the current water level.

* 0 means there is no water
* 1 means the water is touching the first level
* 2 means the water is touching the second level
* n means the water is touching the nth level
* sizeof(n) + 1 means the water is above the max level
* -1 if an error was found in the input levels (non-contiguous range)

Example:

    water_sensor.getActiveLevel()

#License
This code is available under the [MIT License](http://opensource.org/licenses/mit-license.php).