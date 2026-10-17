// Arduino LED Chaser Circuit
// Board: Arduino UNO
// LEDs: Digital pins 2-9
// Delay: 200 milliseconds

const int ledPins[] = {2, 3, 4, 5, 6, 7, 8, 9};
const int numLeds = sizeof(ledPins) / sizeof(ledPins[0]);

const int pattern[] = {0, 1, 2, 3, 4, 5, 6, 7};
const int patternSize = sizeof(pattern) / sizeof(pattern[0]);

const int timer = 200;

void setup() {
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }
}

void runPattern(int sequence[], int size) {
  for (int i = 0; i < size; i++) {
    digitalWrite(ledPins[sequence[i]], HIGH);
    delay(timer);
    digitalWrite(ledPins[sequence[i]], LOW);
  }
}

void loop() {
  // Left to right
  runPattern((int*)pattern, patternSize);

  // Right to left
  int reversePattern[] = {7, 6, 5, 4, 3, 2, 1, 0};
  runPattern(reversePattern, patternSize);
}
