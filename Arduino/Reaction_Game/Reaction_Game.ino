```cpp
const byte LED_PIN = LED_BUILTIN;
const byte BUTTON_PIN = 2;

const unsigned long MIN_WAIT_MS = 2000;
const unsigned long MAX_WAIT_MS = 5000;

bool buttonPressed() {
  return digitalRead(BUTTON_PIN) == LOW;
}

void waitForButtonRelease() {
  while (buttonPressed()) {
    delay(10);
  }
  delay(30);
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  digitalWrite(LED_PIN, LOW);

  Serial.begin(9600);
  randomSeed(analogRead(A0));

  Serial.println(F("Arduino Reaction Game"));
  Serial.println(F("Press the button to start."));
}

void loop() {
  digitalWrite(LED_PIN, LOW);

  while (!buttonPressed()) {
    delay(5);
  }

  waitForButtonRelease();

  Serial.println(F("Get ready..."));
  Serial.println(F("Wait for the LED!"));

  unsigned long waitTime =
      random(MIN_WAIT_MS, MAX_WAIT_MS + 1);

  unsigned long waitStart = millis();

  while (millis() - waitStart < waitTime) {
    if (buttonPressed()) {
      Serial.println(F("False start! Try again."));
      waitForButtonRelease();
      delay(500);
      return;
    }
    delay(1);
  }

  digitalWrite(LED_PIN, HIGH);
  unsigned long signalTime = micros();

  while (!buttonPressed()) {
    delayMicroseconds(100);
  }

  unsigned long reactionTime = micros() - signalTime;

  digitalWrite(LED_PIN, LOW);

  Serial.print(F("Reaction time: "));
  Serial.print(reactionTime / 1000.0, 2);
  Serial.println(F(" ms"));

  waitForButtonRelease();
  Serial.println(F("Press the button to play again."));
  delay(500);
}
```
