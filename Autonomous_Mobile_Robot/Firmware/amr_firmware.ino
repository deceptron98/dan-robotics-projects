/*
  AMR Physical Baseline Firmware
  Board: Arduino Nano (ATmega328P)
  Serial baud: 115200

  Enabled: wheel encoders and three ultrasonic sensors.
  Disabled: motor actuation and MPU6050 IMU.

  Serial packet:
  left_encoder,right_encoder,left_cm,front_cm,right_cm

  Reconstructed replacement; not recovered verbatim from
  the original Jetson firmware.
*/

const unsigned long SERIAL_BAUD = 115200;

const uint8_t LEFT_ENA = 5;
const uint8_t LEFT_IN1 = 7;
const uint8_t LEFT_IN2 = 8;

const uint8_t RIGHT_ENB = 6;
const uint8_t RIGHT_IN3 = 9;
const uint8_t RIGHT_IN4 = 10;

const bool MOTORS_ENABLED = false;

const uint8_t LEFT_ENC_A = 2;
const uint8_t LEFT_ENC_B = 4;

const uint8_t RIGHT_ENC_A = 3;
const uint8_t RIGHT_ENC_B = A0;

const uint8_t LEFT_TRIG = A3;
const uint8_t LEFT_ECHO = A2;

const uint8_t FRONT_TRIG = 13;
const uint8_t FRONT_ECHO = A1;

const uint8_t RIGHT_TRIG = 11;
const uint8_t RIGHT_ECHO = 12;

volatile long leftEncoderCount = 0;
volatile long rightEncoderCount = 0;

const unsigned long SENSOR_INTERVAL_MS = 100;
const unsigned long ECHO_TIMEOUT_US = 25000UL;

unsigned long lastSensorUpdate = 0;

char commandBuffer[64];
uint8_t commandIndex = 0;

void leftEncoderISR() {
  if (digitalRead(LEFT_ENC_B) == HIGH) {
    leftEncoderCount++;
  } else {
    leftEncoderCount--;
  }
}

void rightEncoderISR() {
  if (digitalRead(RIGHT_ENC_B) == HIGH) {
    rightEncoderCount++;
  } else {
    rightEncoderCount--;
  }
}

float readUltrasonicCM(uint8_t trigPin, uint8_t echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(3);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  unsigned long duration = pulseIn(
    echoPin,
    HIGH,
    ECHO_TIMEOUT_US
  );

  if (duration == 0) {
    return -1.0f;
  }

  float distanceCM = duration * 0.0343f / 2.0f;

  if (distanceCM < 2.0f || distanceCM > 400.0f) {
    return -1.0f;
  }

  return distanceCM;
}

void getEncoderCounts(long &leftCount, long &rightCount) {
  noInterrupts();

  leftCount = leftEncoderCount;
  rightCount = rightEncoderCount;

  interrupts();
}

void readSerialCommands() {
  while (Serial.available() > 0) {
    char incoming = (char)Serial.read();

    if (incoming == '\r') {
      continue;
    }

    if (incoming == '\n') {
      commandBuffer[commandIndex] = '\0';
      commandIndex = 0;
    } else {
      if (commandIndex < sizeof(commandBuffer) - 1) {
        commandBuffer[commandIndex++] = incoming;
      } else {
        commandIndex = 0;
      }
    }
  }
}

void publishSensorPacket() {
  long leftCount;
  long rightCount;

  getEncoderCounts(leftCount, rightCount);

  float leftCM = readUltrasonicCM(LEFT_TRIG, LEFT_ECHO);
  float frontCM = readUltrasonicCM(FRONT_TRIG, FRONT_ECHO);
  float rightCM = readUltrasonicCM(RIGHT_TRIG, RIGHT_ECHO);

  Serial.print(leftCount);
  Serial.print(',');

  Serial.print(rightCount);
  Serial.print(',');

  Serial.print(leftCM, 1);
  Serial.print(',');

  Serial.print(frontCM, 1);
  Serial.print(',');

  Serial.println(rightCM, 1);
}

void setup() {
  Serial.begin(SERIAL_BAUD);

  pinMode(LEFT_ENC_A, INPUT_PULLUP);
  pinMode(LEFT_ENC_B, INPUT_PULLUP);

  pinMode(RIGHT_ENC_A, INPUT_PULLUP);
  pinMode(RIGHT_ENC_B, INPUT_PULLUP);

  pinMode(LEFT_TRIG, OUTPUT);
  pinMode(LEFT_ECHO, INPUT);

  pinMode(FRONT_TRIG, OUTPUT);
  pinMode(FRONT_ECHO, INPUT);

  pinMode(RIGHT_TRIG, OUTPUT);
  pinMode(RIGHT_ECHO, INPUT);

  digitalWrite(LEFT_TRIG, LOW);
  digitalWrite(FRONT_TRIG, LOW);
  digitalWrite(RIGHT_TRIG, LOW);

  attachInterrupt(
    digitalPinToInterrupt(LEFT_ENC_A),
    leftEncoderISR,
    RISING
  );

  attachInterrupt(
    digitalPinToInterrupt(RIGHT_ENC_A),
    rightEncoderISR,
    RISING
  );

  delay(300);

  Serial.println(F("AMR ARDUINO FIRMWARE - VERSION 1"));
  Serial.println(F("SAFE SENSOR TEST"));
  Serial.println(F("MOTORS DISABLED"));
  Serial.println(F("ENCODERS ON"));
  Serial.println(F("ULTRASONICS ON"));
  Serial.println(F("IMU OFF"));
  Serial.println(F("LEFT_COUNT,RIGHT_COUNT,LEFT_CM,FRONT_CM,RIGHT_CM"));

  lastSensorUpdate = millis();
}

void loop() {
  readSerialCommands();

  unsigned long now = millis();

  if (now - lastSensorUpdate >= SENSOR_INTERVAL_MS) {
    lastSensorUpdate = now;
    publishSensorPacket();
  }
}
