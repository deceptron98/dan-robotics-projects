```cpp
const byte LEFT_IR_PIN = 7;
const byte RIGHT_IR_PIN = 12;
const byte TRIG_PIN = 8;
const byte ECHO_PIN = 9;

const unsigned long ECHO_TIMEOUT_US = 30000UL;

float readDistanceCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long duration = pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);

  if (duration == 0) {
    return -1.0f;
  }

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
  int leftIR = digitalRead(LEFT_IR_PIN);
  int rightIR = digitalRead(RIGHT_IR_PIN);
  float distanceCm = readDistanceCm();

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
