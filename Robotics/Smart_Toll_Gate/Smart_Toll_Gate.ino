/*
  Smart Toll Gate System
  Board: Arduino UNO
  Sensor: HC-SR04 ultrasonic sensor
  Actuator: Servo motor
  Indicator: Buzzer

  HC-SR04:
    TRIG -> D7
    ECHO -> D8

  Servo signal -> D9
  Buzzer       -> D4

  This implementation automatically opens the barrier
  when a vehicle is detected within the set distance.
*/

#include <Servo.h>

const byte TRIG_PIN = 7;
const byte ECHO_PIN = 8;
const byte SERVO_PIN = 9;
const byte BUZZER_PIN = 4;

const int DETECTION_DISTANCE_CM = 15;
const int GATE_CLOSED_ANGLE = 0;
const int GATE_OPEN_ANGLE = 90;

const unsigned long GATE_OPEN_TIME_MS = 3000;

Servo gateServo;

long readDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long duration = pulseIn(ECHO_PIN, HIGH, 30000UL);

  if (duration == 0) {
    return -1;
  }

  return duration / 58;
}

void closeGate() {
  gateServo.write(GATE_CLOSED_ANGLE);
  Serial.println(F("Gate closed"));
}

void openGate() {
  gateServo.write(GATE_OPEN_ANGLE);

  tone(BUZZER_PIN, 1000, 200);
  Serial.println(F("Vehicle detected - gate opened"));

  delay(GATE_OPEN_TIME_MS);

  closeGate();
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  gateServo.attach(SERVO_PIN);
  closeGate();

  Serial.begin(9600);
  Serial.println(F("Smart Toll Gate System Ready"));
}

void loop() {
  long distance = readDistanceCM();

  Serial.print(F("Vehicle distance: "));
  Serial.print(distance);
  Serial.println(F(" cm"));

  if (distance > 0 &&
      distance <= DETECTION_DISTANCE_CM) {
    openGate();

    // Wait for the vehicle to move away before
    // accepting another detection.
    while (true) {
      distance = readDistanceCM();

      if (distance < 0 ||
          distance > DETECTION_DISTANCE_CM) {
        break;
      }

      delay(100);
    }
  }

  delay(100);
}
