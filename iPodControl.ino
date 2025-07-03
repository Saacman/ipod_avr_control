#include <avr/sleep.h>
#include <avr/interrupt.h>

#include "button.h"
#include "buttonEvents.h"

// Pins
const uint8_t scan_pin = 0;        // Output open-drain "button"
const uint8_t bttn0_pin = 1;       // Enable button
const uint8_t bttn1_pin = 2;       // Event trigger button
const uint8_t hold_switch_pin = 3; // iPod hold switch
const uint8_t gate_pin = 4;        // MOSFET Gate control pin

// Wake flag
volatile bool wake_flag = false;

// Initial Gate value
uint8_t gate_val = HIGH; // (P-MOSFET)

// Button instances
Button btn0(bttn0_pin); // Enable button
Button btn1(bttn1_pin); // Event trigger button
ButtonController ev_btn(btn1);

void setup() {
  // initialize the Gate pin 
  pinMode(gate_pin, OUTPUT);
  digitalWrite(gate_pin, gate_val);

  // Set up wake up interrupt
  pinMode(hold_switch_pin, INPUT);
  attachInterrupt(digitalPinToInterrupt(hold_switch_pin), wakeISR, RISING);

  //Initialize the scan pin
  pinMode(scan_pin, INPUT);  // High-Z (released)

  // Set up sleep
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sei(); // Enable global interrupts
}
void wakeISR(){
  wake_flag = true;
}

void loop() {
  // clear flags
  if(wake_flag) {
    wake_flag = false;
    ev_btn.reset();
  }

  main_control();

  // Prepare for sleep
  if(!wake_flag && !digitalRead(hold_switch_pin)){
    btn0.disable();
    btn1.disable();
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


void doubleClickEvent() {
  // Simulate press scan button using open-drain
  pinMode(scan_pin, OUTPUT);
  digitalWrite(scan_pin, LOW);  // "Press" the button (pull line low)
  delay(1100);                    // Hold for 100 ms
  pinMode(scan_pin, INPUT);     // "Release" the button (go high-Z)
}

void holdEvent() {
  // Toggle bt on/off
  togglePIN(gate_pin, gate_val);
}

// Utility functions
void togglePIN(const uint8_t &pin, uint8_t &val) {
  val = !val;
  digitalWrite(pin, val);
}
