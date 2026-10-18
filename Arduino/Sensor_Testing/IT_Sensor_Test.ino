```cpp
/*
  IT Sensor Testing
  Board: Arduino UNO

  Purpose:
  Read two digital IR obstacle sensors and one
  HC-SR04 ultrasonic sensor, then display readings
  through the Serial Monitor.

  Left IR sensor  -> D7
  Right IR sensor -> D12
  HC-SR04 TRIG    -> D8
  HC-SR04 ECHO    -> D9
*/

const byte LEFT_IR_PIN  = 7;
const byte RIGHT_IR_PIN = 12;
const byte TRIG_PIN     = 8;
const byte ECHO_PIN     = 9;

const unsigned long ECHO_TIMEOUT_US = 30000UL;

float readDistanceCm() {
  // Ensure a clean trigger pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  // Send a 10-microsecond trigger pulse
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measure the returning echo pulse
  unsigned long duration =
      pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);

  // Return -1 if no echo is received
  if (duration == 0) {
    return -1.0f;
  }

  // Convert pulse duration to distance in centimetres
  return (duration * 0.0343f) / 2.0f;
}

void setup() {
  pinMode(LEFT_IR_PIN, INPUT);
  pinMode(RIGHT_IR_PIN, INPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  digitalWrite(TRIG_PIN, LOW);

  Serial.begin(9600);

  Serial.println(F("Arduino Sensor Testing"));
  Serial.println(F("-----------------------"));
}

void loop() {
  // Read the IR sensor output states
  int leftIR = digitalRead(LEFT_IR_PIN);
  int rightIR = digitalRead(RIGHT_IR_PIN);

  // Measure ultrasonic distance
  float distanceCm = readDistanceCm();

  // Display the readings
  Serial.print(F("Left IR: "));
  Serial.print(leftIR);

  Serial.print(F(" | Right IR: "));
  Serial.print(rightIR);

  Serial.print(F(" | Distance: "));

  if (distanceCm < 0.0f) {
    Serial.println(F("No echo"));
  } else {
    Serial.print(distanceCm, 1);
    Serial.println(F(" cm"));
  }

  delay(250);
}
```
