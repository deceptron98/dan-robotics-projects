# Arduino Obstacle-Avoiding Robot

## Project Overview

This project implements an obstacle-avoiding mobile robot using an Arduino Uno, an L298N motor driver, two digital infrared (IR) obstacle sensors, and an HC-SR04 ultrasonic sensor. The controller monitors the sensors and changes the robot's movement when an obstacle is detected.

## Hardware Components

- Arduino Uno
- L298N dual H-bridge motor driver
- Two digital IR obstacle sensors
- HC-SR04 ultrasonic distance sensor
- Two DC geared motors
- Robot chassis and wheels
- Battery power supply
- Buck converters for regulated power where required
- Connecting wires

## Pin Configuration

### IR Sensors

| Component | Arduino Pin |
|---|---|
| Left IR sensor output | D7 |
| Right IR sensor output | D12 |

### HC-SR04 Ultrasonic Sensor

| Sensor Pin | Arduino Pin |
|---|---|
| TRIG | D10 |
| ECHO | D11 |

### L298N Motor Driver

| Driver Pin | Arduino Pin |
|---|---|
| ENA | D3 (PWM) |
| IN1 | D5 |
| IN2 | D6 |
| ENB | D9 (PWM) |
| IN3 | A0 |
| IN4 | A1 |

The ultrasonic pin assignments are defined by the published sketch. Connect all grounds to a common ground and provide suitable regulated supplies for the Arduino and motor driver.

## Operating Principle

The robot uses the IR sensors to detect nearby obstacles on either side and the ultrasonic sensor to measure the distance to an object in front.

When an obstacle is detected, the control sequence is:

1. Stop the motors briefly.
2. Reverse for a short interval.
3. Turn according to the IR sensor readings or use a default turn when a front obstacle is detected.
4. Stop briefly before resuming sensor-based movement.

When no obstacle is detected within the configured safety distance and neither IR sensor is triggered, the robot moves forward.

## Configurable Parameters

The sketch defines the following settings:

- `MOTOR_SPEED`: forward motor speed
- `REVERSE_SPEED`: reverse motor speed
- `TURN_SPEED`: turning speed
- `SAFE_DISTANCE_CM`: ultrasonic obstacle-detection threshold, set to 25 cm
- `IR_STATE`: digital state interpreted as obstacle detection

These parameters can be adjusted to suit the sensor modules and the robot's mechanical response.

## Software Setup

1. Open `Obstacle_Avoiding_Robot.ino` in the Arduino IDE.
2. Select **Arduino Uno** under Tools → Board.
3. Select the appropriate serial port.
4. Upload the sketch.
5. Open Serial Monitor at 9600 baud to view distance and IR sensor readings.

## Skills Demonstrated

- Arduino programming in C++
- Digital and ultrasonic sensor interfacing
- Distance measurement using pulse timing
- DC motor control through an H-bridge driver
- Conditional decision-making
- Basic autonomous obstacle avoidance
- Serial debugging and sensor monitoring

## Project Scope

This project demonstrates reactive obstacle avoidance using IR detection and ultrasonic distance measurement. It uses direct sensor readings and programmed motor responses rather than mapping or a full navigation stack.
