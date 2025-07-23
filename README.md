[![Parking Sensor](https://img.youtube.com/vi/6SB3PiJyosc/0.jpg)](https://www.youtube.com/watch?v=6SB3PiJyosc)


# ESP32 Parking Sensor

A visual parking assistance system using ESP32, ultrasonic sensor, and LED strip to help navigate tight garage spaces.

## Problem

My garage is barely big enough for my car (4.7m car in 4.8m garage space). Standard parking sensors just beep without showing exact distance, making precise positioning nearly impossible.


## Blog Post

Full build details, power considerations, and lessons learned: [Build a Parking sensor with ESP32 board, LEDs and Ultrasonic distance sensor](https://www.poeticoding.com/build-a-parking-sensor-with-esp32-board-leds-and-ultrasonic-distance-sensor/)


## Solution

LED strip that provides real-time visual feedback:
- 🟢 **Green**: Safe distance (>200cm)
- ⚪ **White**: Getting closer (10-200cm) 
- 🔴 **Red**: Danger zone (≤5-10cm)

LEDs illuminate from both ends toward center, creating an intuitive progress bar effect.

## Hardware

- **ESP32**: UM FeatherS3 (any ESP32 works)
- **Distance Sensor**: HC-SR04 ultrasonic sensor
- **LED Strip**: 1m WS2812B RGB strip (60 LEDs)
- **Power**: 12V adapter + buck converter to 5V
- **Wiring**: 18AWG for power distribution

![](https://poeticblog.wpenginepowered.com/wp-content/uploads/2025/07/parking_sensor.drawio-1.png)