// Custom LED Chaser Pattern
// Arduino UNO
// LEDs connected to digital pins 2-9

const int ledPins[] = {2, 3, 4, 5, 6, 7, 8, 9};
const int numLeds = 8;
const int timer = 200;

// LED sequence: 0, 4, 1, 7, 3, 5, 2, 6
const int pattern[] = {0, 4, 1, 7, 3, 5, 2, 6};
const int patternSize = sizeof(pattern) / sizeof(pattern[0]);

void setup() {
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }
}

void loop() {
  for (int i = 0; i < patternSize; i++) {
    int led = pattern[i];

    digitalWrite(ledPins[led], HIGH);
    delay(timer);
    digitalWrite(ledPins[led], LOW);
  }
}
