# Arduino Reaction Game

## Overview

The Arduino Reaction Game is an interactive embedded-systems project that measures a player's response time to a visual signal. It uses an Arduino UNO, a push button, and the built-in LED to provide a simple reaction-time challenge.

## Hardware Components

- Arduino UNO
- Push button
- Jumper wires

## Pin Configuration

| Component | Arduino Pin |
|---|---|
| Built-in LED | D13 |
| Push button | D2 |
| Button ground | GND |

The button uses the Arduino's internal pull-up resistor, so it connects between D2 and GND.

## How It Works

1. The player presses the button to start a round.
2. The program waits for a randomly selected interval.
3. A premature button press triggers a false-start message.
4. When the LED illuminates, the player presses the button as quickly as possible.
5. The reaction time is calculated and displayed in milliseconds.
6. The player can begin another round.

## Software

- Arduino IDE
- Arduino UNO board support
- Serial Monitor at 9600 baud

## Upload Instructions

1. Open `Reaction_Game.ino` in the Arduino IDE.
2. Select **Arduino UNO** as the board.
3. Select the appropriate serial port.
4. Upload the sketch.
5. Open the Serial Monitor at 9600 baud.
6. Follow the displayed instructions to play.

## Learning Outcomes

This project demonstrates digital input handling, output control, random timing, elapsed-time measurement, conditional logic, and serial communication.
