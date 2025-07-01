#include <avr/sleep.h>
#include <avr/interrupt.h>

#include "button.h"
#include "buttonEvents.h"

// MOSFET Gate pin
const uint8_t gate_pin = 0;
// Input buttons pins
const uint8_t bttn0_pin = 1;
const uint8_t bttn1_pin = 2;
// Interrupt pin
const uint8_t isr_pin = 3;
const uint8_t scan_pin = 4;
// Wake flag
volatile bool wake_flag = false;
// Initial Gate value (N-MOSFET) TODO: Change to P-MOSFET
uint8_t gate_val = LOW;
// Button debounced handles and event controller
Button btn0(bttn0_pin); // enable button
Button btn1(bttn1_pin); // Event trigger button
ButtonController ev_btn(btn1);

void setup() {
  // initialize the Gate pin as an output & intialize
  pinMode(gate_pin, OUTPUT);
  digitalWrite(gate_pin, gate_val);

  // Set up wake up interrupt
  pinMode(isr_pin, INPUT);
  attachInterrupt(digitalPinToInterrupt(isr_pin), wakeISR, RISING);

  //Initialize the scan pin
  pinMode(scan_pin, INPUT);  // High-Z (released)

  // Set up sleep
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);  /* Set sleep mode to POWER DOWN mode */
  sleep_enable();
  // Enable global interrupts
  sei();
}
void wakeISR(){
  wake_flag = true;
}

void loop() {

  if(wake_flag) {
    wake_flag = false;
  }

  main_control();

  // Prepare for sleep
  if(!wake_flag && !digitalRead(isr_pin)){
    sleep_cpu();
  }
}

void main_control(){
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
  return;
}

void doubleClickEvent() {
  // Simulate press scan button using open-drain
  pinMode(scan_pin, OUTPUT);
  digitalWrite(scan_pin, LOW);  // "Press" the button (pull line low)
  delay(1100);                    // Hold for 100 ms
  pinMode(scan_pin, INPUT);     // "Release" the button (go high-Z)
}

void holdEvent() {
  // Toggle bt on/off
  toggleLED(gate_pin, gate_val);
}

// Utility functions
void toggleLED(const uint8_t &pin, uint8_t &val) {
  val = !val;
  digitalWrite(pin, val);
}
