# Three-Ultrasonic AMR Hardware Test

## Overview

This sketch tests the Arduino-based hardware control logic for an Autonomous Mobile Robot (AMR). It reads distance measurements from three ultrasonic sensors and uses them to control a two-motor drive system for basic obstacle avoidance and side-distance steering.

## Hardware Components

- Arduino-compatible microcontroller
- Three ultrasonic distance sensors
- Two DC drive motors
- Dual-motor driver
- Two wheel encoders
- Robot chassis and suitable power supply

## Pin Configuration

| Component | Arduino Pin |
|---|---|
| Shared ultrasonic trigger | D11 |
| Front ultrasonic echo | D12 |
| Left ultrasonic echo | A2 |
| Right ultrasonic echo | A1 |
| Left encoder A | D2 |
| Left encoder B | D4 |
| Left motor PWM (ENA) | D5 |
| Left motor direction | D7, D8 |
| Right encoder A | D3 |
| Right encoder B | A0 |
| Right motor PWM (ENB) | D6 |
| Right motor direction | D9, D10 |

The three ultrasonic sensors share one trigger pin and use separate echo pins.

## Operating Logic

### 1. Distance Measurement

The sketch reads the front, left and right sensor distances and reports them through the Serial Monitor at **9600 baud**.

### 2. Front Obstacle Avoidance

When a valid front measurement is at or below **25 cm**, the robot stops briefly and selects a turn based on the side measurements:

- If both side measurements are below 20 cm, it turns right for approximately 1,200 ms.
- If the left measurement is greater than the right measurement, it turns left for approximately 600 ms.
- Otherwise, it turns right for approximately 600 ms.

These durations are timing-based estimates; the resulting rotation angle depends on the motors, battery voltage, floor surface and robot load.

### 3. Side-Distance Steering

When no front obstacle is detected within the stopping threshold, the sketch adjusts the left and right motor speeds according to nearby objects detected by the side sensors. The base PWM speed is 170, with calculated motor commands constrained to the range 80–255.

### 4. Encoder Interrupts

The sketch configures both wheel encoders and updates tick counters using interrupt handlers. However, those counts are **not used to determine the turn angles** in this version.

## Key Parameters

| Parameter | Value |
|---|---:|
| Serial baud rate | 9600 |
| Base motor PWM | 170 |
| Front obstacle threshold | 25 cm |
| Side-steering threshold | 20 cm |
| Approximate standard turn duration | 600 ms |
| Approximate blocked-sides turn duration | 1,200 ms |

## Scope and Limitations

This is a hardware-test sketch for basic ultrasonic obstacle avoidance and side-distance steering. It uses timed turns rather than calibrated encoder-based angular control. It does not implement a fixed sequence of 45°, 90° and 135° turns.

Before testing on the floor, secure the robot and verify the motor directions, sensor readings, wiring and motor-driver power arrangement. Use an appropriate motor supply and a common ground between the controller and motor driver.

## Purpose

This project documents the Arduino-level sensing and motor-control stage of the AMR development work. It provides a reference for the pin configuration, obstacle thresholds, steering behavior and current limitations of this test sketch.
