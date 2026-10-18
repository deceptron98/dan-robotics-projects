
/*
  Arduino Line-Following Robot
  Board: Arduino UNO
  Motor driver: L298N
  Sensors: Left and right digital IR line sensors

  Sensor outputs:
    Left sensor  -> D2
    Right sensor -> D4

  Motor driver:
    ENA -> D5 (PWM)
    IN1 -> D7
    IN2 -> D8
    ENB -> D6 (PWM)
    IN3 -> D9
    IN4 -> D10

  Assumption: LOW means the sensor detects the line.
  Adjust LINE_STATE to HIGH if required by your sensors.
*/

const byte LEFT_SENSOR  = 2;
const byte RIGHT_SENSOR = 4;

const byte ENA = 5;
const byte IN1 = 7;
const byte IN2 = 8;

const byte ENB = 6;
const byte IN3 = 9;
const byte IN4 = 10;

const int BASE_SPEED = 150;
const int TURN_SPEED = 135;

const int LINE_STATE = LOW;

void setMotor(int leftSpeed, int rightSpeed) {
  // Left motor direction
  digitalWrite(IN1, leftSpeed >= 0 ? HIGH : LOW);
  digitalWrite(IN2, leftSpeed >= 0 ? LOW : HIGH);

  // Right motor direction
  digitalWrite(IN3, rightSpeed >= 0 ? HIGH : LOW);
  digitalWrite(IN4, rightSpeed >= 0 ? LOW : HIGH);

  analogWrite(ENA, constrain(abs(leftSpeed), 0, 255));
  analogWrite(ENB, constrain(abs(rightSpeed), 0, 255));
}

void stopMotors() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void setup() {
  pinMode(LEFT_SENSOR, INPUT);
  pinMode(RIGHT_SENSOR, INPUT);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  Serial.begin(9600);
  stopMotors();

  Serial.println(F("Line-Following Robot Ready"));
}

void loop() {
  bool leftOnLine =
      digitalRead(LEFT_SENSOR) == LINE_STATE;

  bool rightOnLine =
      digitalRead(RIGHT_SENSOR) == LINE_STATE;

  if (leftOnLine && rightOnLine) {
    // Both sensors detect the line
    setMotor(BASE_SPEED, BASE_SPEED);
  } else if (leftOnLine && !rightOnLine) {
    // Correct toward the left
    setMotor(TURN_SPEED / 2, TURN_SPEED);
  } else if (!leftOnLine && rightOnLine) {
    // Correct toward the right
    setMotor(TURN_SPEED, TURN_SPEED / 2);
  } else {
    // Neither sensor detects the line
    stopMotors();
  }

  delay(10);
}
