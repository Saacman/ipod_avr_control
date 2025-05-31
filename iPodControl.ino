#include "button.h"

// constants won't change. They're used here to set pin numbers:
const int ledPinR = 0;
const int ledPinG = 4;
const int ledPinB = 3;

const int button0Pin = 1;
const int button1Pin = 2;

int ledRState = LOW;
int ledGState = LOW;
int ledBState = LOW;

Button btn0(button0Pin);
Button btn1(button1Pin);

void setup() {
  // initialize the LED pins as an output:
  pinMode(ledPinR, OUTPUT);
  pinMode(ledPinG, OUTPUT);
  pinMode(ledPinB, OUTPUT);

  // set initial LED state
  digitalWrite(ledPinR, ledRState);
  digitalWrite(ledPinG, ledGState);
  digitalWrite(ledPinB, ledBState);
}

void loop() {
  btn0.update();
  btn1.update();
  if(btn0.wasReleased()) ledRState = !ledRState;
  if(btn1.wasReleased()) ledGState = !ledGState;
  // if(btn0.wasPressed()) ledBState = !ledBState;
  // if(btn1.wasPressed()) ledBState = !ledBState;

  // set  LED state
  digitalWrite(ledPinR, ledRState);
  digitalWrite(ledPinG, ledGState);
  // digitalWrite(ledPinB, ledBState);
}
