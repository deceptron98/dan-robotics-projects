# Arduino Smart Toll Gate System

## Project Overview

This project demonstrates an automatic toll gate barrier using an Arduino Uno, an HC-SR04 ultrasonic sensor, a servo motor, and a buzzer. The system detects an approaching vehicle and operates the barrier according to a predefined control sequence.

## Hardware Components

- Arduino Uno
- HC-SR04 ultrasonic distance sensor
- Servo motor
- Buzzer
- Breadboard and jumper wires
- Suitable power supply

## Pin Configuration

| Component | Arduino Pin |
|---|---|
| HC-SR04 TRIG | D7 |
| HC-SR04 ECHO | D8 |
| Servo signal | D9 |
| Buzzer | D4 |

Connect the sensor and actuator grounds to a common ground. Ensure the servo receives an appropriate power supply for its current requirements.

## Operating Principle

1. The ultrasonic sensor measures the distance to an object approaching the gate.
2. When an object is detected within 15 cm, the Arduino commands the servo to open the barrier.
3. The buzzer produces a brief audible signal.
4. The barrier remains open for approximately three seconds before the servo returns it to the closed position.
5. The controller waits for the object to move outside the detection range before accepting another detection.

Distance readings and gate status messages are printed to the Serial Monitor at 9600 baud.

## Configurable Parameters

- `DETECTION_DISTANCE_CM`: detection threshold, set to 15 cm.
- `GATE_CLOSED_ANGLE`: servo position for the closed barrier.
- `GATE_OPEN_ANGLE`: servo position for the open barrier.
- `GATE_OPEN_TIME_MS`: time the barrier remains open, set to 3000 milliseconds.

These values can be adjusted to suit the barrier mechanism and installation.

## Software Setup

1. Open `Smart_Toll_Gate.ino` in the Arduino IDE.
2. Select **Arduino Uno** under Tools → Board.
3. Select the appropriate serial port.
4. Upload the sketch.
5. Open Serial Monitor at 9600 baud to observe distance readings and gate status.

The sketch uses the standard Arduino `Servo` library.

## Skills Demonstrated

- Ultrasonic distance measurement
- Servo motor control
- Digital output and buzzer control
- Embedded C++ programming
- Conditional logic and timing
- Basic automated access-control mechanisms
- Serial monitoring and debugging

## Project Scope

This implementation demonstrates automatic barrier operation based on proximity detection. It does not implement payment processing, vehicle identification, or transaction records.
