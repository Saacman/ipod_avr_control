#include "button.h"
#include "buttonEvents.h"

// MOSFET Gate pin
const uint8_t gate_pin = 0;
// Input buttons pins
const uint8_t bttn0_pin = 3;
const uint8_t bttn1_pin = 2;

// Initial Gate value
uint8_t gate_val = LOW;

// Button debounced handles and event controller
Button btn0(bttn0_pin); // enable button
Button btn1(bttn1_pin); // Event trigger button
ButtonController ev_btn(btn1);
// bool enable = false;

void setup() {
  // initialize the Gate pin as an output:
  pinMode(gate_pin, OUTPUT);

  // set initial LED state
  digitalWrite(gate_pin, gate_val);
}

void loop() {
  // Update buttons. Avoid halting the FSM
  btn0.update();
  ev_btn.update();

  if (btn0.wasReleased() || btn0.wasPressed()) {
    // Cancel event sequence if the enable button was released or
    // Clear previous state if the enable button was just pressed (ignore previous events)
    ev_btn.reset();
  }

  if (btn0.isPressed()) {
    switch (ev_btn.getEvent()) {
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
  // Don't do annything
  return;
}
void doubleClickEvent() {
  toggleLED(gate_pin, gate_val);
  delay(100);
  toggleLED(gate_pin, gate_val);
  delay(100);
}
void holdEvent() {
  toggleLED(gate_pin, gate_val);
  delay(100);
  toggleLED(gate_pin, gate_val);
  delay(100);
  toggleLED(gate_pin, gate_val);
  delay(100);
  toggleLED(gate_pin, gate_val);
  delay(100);
  toggleLED(gate_pin, gate_val);
  delay(100);
  toggleLED(gate_pin, gate_val);
  delay(100);
}

// Utility functions
void toggleLED(const uint8_t &pin, uint8_t &val) {
  val = !val;
  digitalWrite(pin, val);
}
