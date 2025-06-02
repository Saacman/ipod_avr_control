#include "button.h"
#include "buttonEvents.h"

// LED pins
const uint8_t ledR_pin = 0;
const uint8_t ledG_pin = 4;
const uint8_t ledB_pin = 3;
// Input buttons pins
const uint8_t bttn0_pin = 1;
const uint8_t bttn1_pin = 2;
// Initial LED values
uint8_t ledR_val = LOW;
uint8_t ledG_val = LOW;
uint8_t ledB_val = LOW;
// Button debounced handles and event controller
Button btn0(bttn0_pin);
Button btn1(bttn1_pin);
ButtonController ev_btn(btn1);

void setup() {
  // initialize the LED pins as an output:
  pinMode(ledR_pin, OUTPUT);
  pinMode(ledG_pin, OUTPUT);
  pinMode(ledB_pin, OUTPUT);

  // set initial LED state
  digitalWrite(ledR_pin, ledR_val);
  digitalWrite(ledG_pin, ledG_val);
  digitalWrite(ledB_pin, ledB_val);
}

void loop() {
  btn0.update();
  // Use btn0 as blocking button
  if (btn0.isPressed()) {
    switch (ev_btn.update()) {
      case BttnEvent::CLICK:
        clickEvent();
        break;
      case BttnEvent::DBL_CLICK:
        doubleClickEvent();
        break;
      case BttnEvent::HOLD:
        holdEvent();
        break;
      default:
        break;
    }
  }
}


// Events to trigger
void clickEvent() {
  toggleLED(ledR_pin, ledR_val);
}
void doubleClickEvent() {
  toggleLED(ledG_pin, ledG_val);
}
void holdEvent() {
  toggleLED(ledB_pin, ledB_val);
}

// Utility functions
void toggleLED(const uint8_t &pin, uint8_t &val) {
  val = !val;
  digitalWrite(pin, val);
}
