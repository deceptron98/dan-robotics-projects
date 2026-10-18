// Motor 1 Pins (Left Motor)
const int M1_ENC_A = 2; // Hardware Interrupt 0
const int M1_ENC_B = 4;
const int ENA = 5;      // PWM Speed Control
const int IN1 = 7;      // Direction 1
const int IN2 = 8;      // Direction 2

// Motor 2 Pins (Right Motor)
const int M2_ENC_A = 3; // Hardware Interrupt 1
const int M2_ENC_B = A0;
const int ENB = 6;      // PWM Speed Control
const int IN3 = 9;      // Direction 1
const int IN4 = 10;     // Direction 2

// Ultrasonic Sensor Pins
const int TRIG_PIN   = 11; // Shared trigger for all 3 sensors
const int ECHO_FRONT = 12; // Front sensor echo
const int ECHO_LEFT  = A2; // Left sensor echo
const int ECHO_RIGHT = A1; // Right sensor echo

// Encoders & Speed Management
volatile long m1_ticks = 0;
volatile long m2_ticks = 0;

const int BASE_SPEED        = 170; // Cruise speed (0-255)
const int FRONT_STOP_DIST   = 25;  // Stop and turn distance in cm
const int SIDE_NUDGE_DIST   = 20;  // Threshold distance to start nudging in cm

void setup() {
  Serial.begin(9600);

  // Ultrasonic Setup
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_FRONT, INPUT);
  pinMode(ECHO_LEFT, INPUT);
  pinMode(ECHO_RIGHT, INPUT);

  // Motor 1 Setup
  pinMode(M1_ENC_A, INPUT_PULLUP);
  pinMode(M1_ENC_B, INPUT_PULLUP);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(M1_ENC_A), readM1Encoder, RISING);

  // Motor 2 Setup
  pinMode(M2_ENC_A, INPUT_PULLUP);
  pinMode(M2_ENC_B, INPUT_PULLUP);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(M2_ENC_A), readM2Encoder, RISING);
}

void loop() {
  // Read distance from all three sensors using shared trigger
  float frontDist = readDistance(ECHO_FRONT);
  float leftDist  = readDistance(ECHO_LEFT);
  float rightDist = readDistance(ECHO_RIGHT);

  Serial.print("L: "); Serial.print(leftDist);
  Serial.print("cm | F: "); Serial.print(frontDist);
  Serial.print("cm | R: "); Serial.println(rightDist);

  // 1. Front Obstacle Avoidance (Priority Handling)
  if (frontDist > 0 && frontDist <= FRONT_STOP_DIST) {
    stopMotors();
    delay(200);

    // If both side paths are blocked, execute 180° turn
    if (leftDist < SIDE_NUDGE_DIST && rightDist < SIDE_NUDGE_DIST) {
      turnRightOnSpot(BASE_SPEED);
      delay(1200); // Adjust duration for 180 degree rotation
    } 
    // If left is clearer, turn left
    else if (leftDist > rightDist) {
      turnLeftOnSpot(BASE_SPEED);
      delay(600);  // Adjust duration for 90 degree rotation
    } 
    // Otherwise turn right
    else {
      turnRightOnSpot(BASE_SPEED);
      delay(600);  // Adjust duration for 90 degree rotation
    }

    stopMotors();
    delay(200);
  } 
  // 2. Proportional Nudging (Side Obstacle Alignment)
  else {
    int leftCorrection = 0;
    int rightCorrection = 0;

    // Object close on the left: steer right by slowing left motor & boosting right
    if (leftDist > 0 && leftDist < SIDE_NUDGE_DIST) {
      int error = SIDE_NUDGE_DIST - leftDist;
      leftCorrection = error * 4; 
    }

    // Object close on the right: steer left by slowing right motor & boosting left
    if (rightDist > 0 && rightDist < SIDE_NUDGE_DIST) {
      int error = SIDE_NUDGE_DIST - rightDist;
      rightCorrection = error * 4;
    }

    // Calculate individual motor target speeds
    int leftMotorSpeed  = BASE_SPEED - leftCorrection + rightCorrection;
    int rightMotorSpeed = BASE_SPEED - rightCorrection + leftCorrection;

    // Constrain speeds within valid PWM limits (0 - 255)
    leftMotorSpeed  = constrain(leftMotorSpeed, 80, 255);
    rightMotorSpeed = constrain(rightMotorSpeed, 80, 255);

    driveMotors(leftMotorSpeed, rightMotorSpeed);
  }

  delay(40); // Small interval between sonar sweeps
}

// Function to trigger ultrasound pulse and measure pulse length on specified Echo pin
float readDistance(int echoPin) {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(echoPin, HIGH, 25000); // 25ms timeout (~4m max range)
  if (duration == 0) return 999.0; // Return out-of-range if no pulse back

  return (duration * 0.0343) / 2.0;
}

// Low-Level Motor Functions
void driveMotors(int leftSpeed, int rightSpeed) {
  analogWrite(ENA, leftSpeed);
  analogWrite(ENB, rightSpeed);
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
}

void turnRightOnSpot(int speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);  // Left forward
  digitalWrite(IN3, HIGH);  digitalWrite(IN4, LOW); // Right reverse
}

void turnLeftOnSpot(int speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, HIGH);  digitalWrite(IN2, LOW); // Left reverse
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);  // Right forward
}

void stopMotors() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}

// Encoder ISRs
void readM1Encoder() {
  if (digitalRead(M1_ENC_B) == HIGH) m1_ticks++; else m1_ticks--;
}

void readM2Encoder() {
  if (digitalRead(M2_ENC_B) == HIGH) m2_ticks++; else m2_ticks--;
}
