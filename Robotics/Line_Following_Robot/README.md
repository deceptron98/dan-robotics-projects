# Arduino Line-Following Robot

## Project Overview

This project implements a two-sensor line-following robot using an Arduino Uno, digital infrared (IR) sensors, and an L298N dual H-bridge motor driver. The robot reads the sensor outputs and adjusts the speed of its left and right motors to follow a marked path.

## Hardware Components

- Arduino Uno
- L298N dual H-bridge motor driver
- Two digital IR line sensors
- Two DC geared motors
- Robot chassis and wheels
- Battery power supply
- Jumper wires

## Pin Configuration

### IR Sensors

| Component | Arduino Pin |
|---|---|
| Left IR sensor output | D2 |
| Right IR sensor output | D4 |

### L298N Motor Driver

| Driver Pin | Arduino Pin |
|---|---|
| ENA | D5 (PWM) |
| IN1 | D7 |
| IN2 | D8 |
| ENB | D6 (PWM) |
| IN3 | D9 |
| IN4 | D10 |

Connect the sensor and motor-driver grounds to the Arduino ground. Supply the motors using an appropriate motor power source.

## Operating Principle

The Arduino continuously reads both IR sensor outputs and compares them with the configured line-detection state.

- **Both sensors detect the line:** Both motors move forward at the base speed.
- **Only the left sensor detects the line:** The motor speeds are adjusted to steer toward the left.
- **Only the right sensor detects the line:** The motor speeds are adjusted to steer toward the right.
- **Neither sensor detects the line:** The motors stop.

The sensor detection state is configured using `LINE_STATE` in the sketch. The motor speeds are controlled using PWM outputs.

## Software Setup

1. Open `Line_Following_Robot.ino` in the Arduino IDE.
2. Select **Arduino Uno** under Tools → Board.
3. Select the correct serial port.
4. Upload the sketch.
5. Power the robot and observe its response to the line.

The sketch initializes serial communication at 9600 baud and prints a startup message when the controller is ready.

## Skills Demonstrated

- Digital sensor interfacing
- Arduino programming in C++
- Motor-driver control
- PWM speed regulation
- Conditional logic for navigation
- Basic autonomous robotic control

## Project Scope

This project demonstrates a basic reactive line-following algorithm using two digital sensors. Its steering behavior is determined by the current sensor readings, without mapping or a navigation framework.
