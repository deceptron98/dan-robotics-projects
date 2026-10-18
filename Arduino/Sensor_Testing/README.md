# Sensor Testing and Integration

## Overview

This project uses an Arduino UNO to read two digital infrared (IR) obstacle sensors and an HC-SR04 ultrasonic sensor. Sensor readings are displayed through the Serial Monitor for observation and troubleshooting.

## Hardware Components

- Arduino UNO
- Two digital IR obstacle sensor modules
- HC-SR04 ultrasonic distance sensor
- Breadboard and jumper wires

## Pin Configuration

| Component | Arduino Pin |
|---|---|
| Left IR sensor output | D7 |
| Right IR sensor output | D12 |
| HC-SR04 TRIG | D8 |
| HC-SR04 ECHO | D9 |

**Note:** Confirm the ultrasonic pin assignments against the physical circuit before uploading.

## Operation

1. Configure the Arduino IDE for Arduino UNO.
2. Open `IT_Sensor_Test.ino`.
3. Upload the sketch.
4. Open the Serial Monitor at 9600 baud.
5. Observe the IR sensor output states and ultrasonic distance readings.

The ultrasonic measurement is displayed in centimetres. If no echo is received within the timeout period, the output displays `No echo`.

IR sensor output polarity depends on the module used.

## Project Scope

This sketch provides a basic sensor-reading and monitoring interface. It does not control motors or implement autonomous obstacle avoidance.
