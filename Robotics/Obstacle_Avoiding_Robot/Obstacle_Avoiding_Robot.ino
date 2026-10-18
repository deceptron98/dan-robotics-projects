/*
  Arduino Obstacle-Avoiding Robot
  Board: Arduino UNO
  Motor driver: L298N
  Sensors: Two digital IR sensors and one HC-SR04

  IR sensors:
    Left OUT  -> D7
    Right OUT -> D12

  HC-SR04:
    TRIG -> D10
    ECHO -> D11

  L298N:
    ENA -> D3 (PWM)
    IN1 -> D5
    IN2 -> D6
    ENB -> D9 (PWM)
    IN3 -> A0
    IN4 -> A1

  IR_STATE defines the digital level indicating
  obstacle detection. Change it to HIGH if needed.
*/

const byte LEFT_IR = 7;
const byte RIGHT_IR = 12;

const byte TRIG_PIN = 10;
const byte ECHO_PIN = 11;

const byte ENA = 3;
const byte IN1 = 5;
const byte IN2 = 6;

const byte ENB = 9;
const byte IN3 = A0;
const byte IN4 = A1;

const int MOTOR_SPEED = 160;
const int REVERSE_SPEED = 140;
const int TURN_SPEED = 155;

const int SAFE_DISTANCE_CM = 25;
const int IR_STATE = LOW;

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

void setMotor(int leftSpeed, int rightSpeed) {
  digitalWrite(IN1, leftSpeed >= 0 ? HIGH : LOW);
  digitalWrite(IN2, leftSpeed >= 0 ? LOW : HIGH);

  digitalWrite(IN3, rightSpeed >= 0 ? HIGH : LOW);
  digitalWrite(IN4, rightSpeed >= 0 ? LOW : HIGH);

  analogWrite(ENA, constrain(abs(leftSpeed), 0, 255));
  analogWrite(ENB, constrain(abs(rightSpeed), 0, 255));
}

void stopMotors() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void avoidObstacle(bool leftBlocked, bool rightBlocked) {
  stopMotors();
  delay(150);

  // Reverse briefly before changing direction.
  setMotor(-REVERSE_SPEED, -REVERSE_SPEED);
  delay(250);

  if (leftBlocked && !rightBlocked) {
    // Obstacle on the left: turn right.
    setMotor(TURN_SPEED, -TURN_SPEED);
  } else if (rightBlocked && !leftBlocked) {
    // Obstacle on the right: turn left.
    setMotor(-TURN_SPEED, TURN_SPEED);
  } else {
    // Front obstacle or both sides blocked.
    setMotor(TURN_SPEED, -TURN_SPEED);
  }

  delay(350);
  stopMotors();
  delay(100);
}

void setup() {
  pinMode(LEFT_IR, INPUT);
  pinMode(RIGHT_IR, INPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  Serial.begin(9600);
  stopMotors();

  Serial.println(F("Obstacle-Avoiding Robot Ready"));
}

void loop() {
  bool leftBlocked = digitalRead(LEFT_IR) == IR_STATE;
  bool rightBlocked = digitalRead(RIGHT_IR) == IR_STATE;

  long distance = readDistanceCM();

  bool frontBlocked =
      distance > 0 && distance <= SAFE_DISTANCE_CM;

  Serial.print(F("Distance: "));
  Serial.print(distance);
  Serial.print(F(" cm | Left IR: "));
  Serial.print(leftBlocked);
  Serial.print(F(" | Right IR: "));
  Serial.println(rightBlocked);

  if (frontBlocked || leftBlocked || rightBlocked) {
    avoidObstacle(leftBlocked, rightBlocked);
  } else {
    setMotor(MOTOR_SPEED, MOTOR_SPEED);
  }

  delay(50);
}
