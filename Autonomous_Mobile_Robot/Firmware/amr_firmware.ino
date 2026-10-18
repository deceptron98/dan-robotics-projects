/*
  AMR Physical Baseline Firmware - Reconstructed Version
  Board: Arduino Nano (ATmega328P)
  Serial baud: 115200

  Enabled:
    - Left wheel encoder
    - Right wheel encoder
    - Left ultrasonic sensor
    - Front ultrasonic sensor
    - Right ultrasonic sensor

  Disabled:
    - Motor actuation
    - MPU6050 IMU

  Serial sensor packet:
    left_encoder,right_encoder,left_cm,front_cm,right_cm

  This is a reconstructed replacement, NOT the original
  firmware recovered verbatim from the Jetson.
*/

// ---------------- SERIAL CONFIGURATION ----------------

const unsigned long SERIAL_BAUD = 115200;

// ---------------- MOTOR PINS ----------------
// Defined for reference only. Motor actuation is disabled.
// This sketch never writes motor drive signals.

const uint8_t LEFT_ENA  = 5;
const uint8_t LEFT_IN1  = 7;
const uint8_t LEFT_IN2  = 8;

const uint8_t RIGHT_ENB = 6;
const uint8_t RIGHT_IN3 = 9;
const uint8_t RIGHT_IN4 = 10;

const bool MOTORS_ENABLED = false;

// ---------------- LEFT ENCODER ----------------

const uint8_t LEFT_ENC_A = 2;  // External interrupt
const uint8_t LEFT_ENC_B = 4;

// ---------------- RIGHT ENCODER ----------------

const uint8_t RIGHT_ENC_A = 3; // External interrupt
const uint8_t RIGHT_ENC_B = A0;

// ---------------- ULTRASONIC SENSORS ----------------

const uint8_t LEFT_TRIG  = A3;
const uint8_t LEFT_ECHO  = A2;

const uint8_t FRONT_TRIG = 13;
const uint8_t FRONT_ECHO = A1;

const uint8_t RIGHT_TRIG = 11;
const uint8_t RIGHT_ECHO = 12;

// ---------------- ENCODER COUNTERS ----------------

volatile long leftEncoderCount = 0;
volatile long rightEncoderCount = 0;

// ---------------- TIMING ----------------

const unsigned long SENSOR_INTERVAL_MS = 100;
const unsigned long ECHO_TIMEOUT_US = 25000UL;

unsigned long lastSensorUpdate = 0;

// ---------------- SERIAL COMMAND BUFFER ----------------

// The ROS bridge may send CMD,linear_x,angular_z.
// We consume these lines to keep the serial receive buffer
// clear, but intentionally do not actuate the motors.

char commandBuffer[64];
uint8_t commandIndex = 0;

// ======================================================
// ENCODER INTERRUPTS
// ======================================================

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

// ======================================================
// ULTRASONIC DISTANCE
// Returns centimetres, or -1.0 if no valid echo is found.
// ======================================================

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

  // Approximate speed-of-sound conversion to centimetres.
  float distanceCM = duration * 0.0343f / 2.0f;

  if (distanceCM < 2.0f || distanceCM > 400.0f) {
    return -1.0f;
  }

  return distanceCM;
}

// ======================================================
// ENCODER SNAPSHOT
// Copy both 32-bit counters atomically on the 8-bit AVR.
// ======================================================

void getEncoderCounts(long &leftCount, long &rightCount) {
  noInterrupts();

  leftCount = leftEncoderCount;
  rightCount = rightEncoderCount;

  interrupts();
}

// ======================================================
// SERIAL COMMAND HANDLING
// Consume complete command lines without enabling motors.
// ======================================================

void readSerialCommands() {
  while (Serial.available() > 0) {
    char incoming = (char)Serial.read();

    if (incoming == '\r') {
      continue;
    }

    if (incoming == '\n') {
      commandBuffer[commandIndex] = '\0';

      // Commands are deliberately ignored because motors
      // must remain disabled in the integration baseline.
      commandIndex = 0;
    } else {
      if (commandIndex < sizeof(commandBuffer) - 1) {
        commandBuffer[commandIndex++] = incoming;
      } else {
        // Discard an overlong line safely.
        commandIndex = 0;
      }
    }
  }
}

// ======================================================
// SENSOR PACKET
// Exactly five comma-separated fields, with no labels.
// ======================================================

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

// ======================================================
// SETUP
// ======================================================

void setup() {
  Serial.begin(SERIAL_BAUD);

  // Encoder inputs
  pinMode(LEFT_ENC_A, INPUT_PULLUP);
  pinMode(LEFT_ENC_B, INPUT_PULLUP);

  pinMode(RIGHT_ENC_A, INPUT_PULLUP);
  pinMode(RIGHT_ENC_B, INPUT_PULLUP);

  // Ultrasonic sensor pins
  pinMode(LEFT_TRIG, OUTPUT);
  pinMode(LEFT_ECHO, INPUT);

  pinMode(FRONT_TRIG, OUTPUT);
  pinMode(FRONT_ECHO, INPUT);

  pinMode(RIGHT_TRIG, OUTPUT);
  pinMode(RIGHT_ECHO, INPUT);

  digitalWrite(LEFT_TRIG, LOW);
  digitalWrite(FRONT_TRIG, LOW);
  digitalWrite(RIGHT_TRIG, LOW);

  // Motor pins are intentionally not configured as outputs.
  // This prevents this sketch from driving the motor driver.

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

// ======================================================
// MAIN LOOP
// ======================================================

void loop() {
  readSerialCommands();

  unsigned long now = millis();

  if (now - lastSensorUpdate >= SENSOR_INTERVAL_MS) {
    lastSensorUpdate = now;
    publishSensorPacket();
  }
}
