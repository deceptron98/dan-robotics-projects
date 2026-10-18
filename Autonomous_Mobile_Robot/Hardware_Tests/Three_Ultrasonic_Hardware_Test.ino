```cpp
const int M1_ENC_A = 2;
const int M1_ENC_B = 4;
const int ENA = 5;
const int IN1 = 7;
const int IN2 = 8;

const int M2_ENC_A = 3;
const int M2_ENC_B = A0;
const int ENB = 6;
const int IN3 = 9;
const int IN4 = 10;

const int TRIG_PIN = 11;
const int ECHO_FRONT = 12;
const int ECHO_LEFT = A2;
const int ECHO_RIGHT = A1;

volatile long m1_ticks = 0;
volatile long m2_ticks = 0;

const int BASE_SPEED = 170;
const int FRONT_STOP_DIST = 25;
const int SIDE_NUDGE_DIST = 20;

void readM1Encoder();
void readM2Encoder();
float readDistance(int echoPin);
void driveMotors(int leftSpeed, int rightSpeed);
void turnRightOnSpot(int speed);
void turnLeftOnSpot(int speed);
void stopMotors();

void setup() {
  Serial.begin(9600);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_FRONT, INPUT);
  pinMode(ECHO_LEFT, INPUT);
  pinMode(ECHO_RIGHT, INPUT);

  pinMode(M1_ENC_A, INPUT_PULLUP);
  pinMode(M1_ENC_B, INPUT_PULLUP);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(M1_ENC_A), readM1Encoder, RISING);

  pinMode(M2_ENC_A, INPUT_PULLUP);
  pinMode(M2_ENC_B, INPUT_PULLUP);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(M2_ENC_A), readM2Encoder, RISING);
}

void loop() {
  float frontDist = readDistance(ECHO_FRONT);
  float leftDist = readDistance(ECHO_LEFT);
  float rightDist = readDistance(ECHO_RIGHT);

  Serial.print("L: ");
  Serial.print(leftDist);
  Serial.print("cm | F: ");
  Serial.print(frontDist);
  Serial.print("cm | R: ");
  Serial.println(rightDist);

  if (frontDist > 0 && frontDist <= FRONT_STOP_DIST) {
    stopMotors();
    delay(200);

    if (leftDist < SIDE_NUDGE_DIST && rightDist < SIDE_NUDGE_DIST) {
      turnRightOnSpot(BASE_SPEED);
      delay(1200);
    } else if (leftDist > rightDist) {
      turnLeftOnSpot(BASE_SPEED);
      delay(600);
    } else {
      turnRightOnSpot(BASE_SPEED);
      delay(600);
    }

    stopMotors();
    delay(200);
  } else {
    int leftCorrection = 0;
    int rightCorrection = 0;

    if (leftDist > 0 && leftDist < SIDE_NUDGE_DIST) {
      int error = SIDE_NUDGE_DIST - leftDist;
      leftCorrection = error * 4;
    }

    if (rightDist > 0 && rightDist < SIDE_NUDGE_DIST) {
      int error = SIDE_NUDGE_DIST - rightDist;
      rightCorrection = error * 4;
    }

    int leftMotorSpeed = BASE_SPEED - leftCorrection + rightCorrection;
    int rightMotorSpeed = BASE_SPEED - rightCorrection + leftCorrection;

    leftMotorSpeed = constrain(leftMotorSpeed, 80, 255);
    rightMotorSpeed = constrain(rightMotorSpeed, 80, 255);

    driveMotors(leftMotorSpeed, rightMotorSpeed);
  }

  delay(40);
}

float readDistance(int echoPin) {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(echoPin, HIGH, 25000);

  if (duration == 0) {
    return 999.0;
  }

  return (duration * 0.0343) / 2.0;
}

void driveMotors(int leftSpeed, int rightSpeed) {
  analogWrite(ENA, leftSpeed);
  analogWrite(ENB, rightSpeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnRightOnSpot(int speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnLeftOnSpot(int speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopMotors() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void readM1Encoder() {
  if (digitalRead(M1_ENC_B) == HIGH) {
    m1_ticks++;
  } else {
    m1_ticks--;
  }
}

void readM2Encoder() {
  if (digitalRead(M2_ENC_B) == HIGH) {
    m2_ticks++;
  } else {
    m2_ticks--;
  }
}
```
