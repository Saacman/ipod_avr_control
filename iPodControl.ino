#include "button.h"
#include "buttonEvents.h"
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
ButtonController ev_btn0(btn0);

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
  switch (ev_btn0.update()) {
    case BttnEvent::CLICK:
      ledRState = !ledRState;
      break;
    case BttnEvent::DBL_CLICK:
      ledGState = !ledGState;
      break;
    case BttnEvent::HOLD:
      ledBState = !ledBState;
      break;
    default:
      break;
  }
  // set  LED state
  digitalWrite(ledPinR, ledRState);
  digitalWrite(ledPinG, ledGState);
  digitalWrite(ledPinB, ledBState);
}
