#include <avr/sleep.h>
#include <avr/interrupt.h>

#include "button.h"
#include "buttonEvents.h"

// Pins
const uint8_t gate_pin = 1;
const uint8_t bttn0_pin = 0; // Enable button
const uint8_t bttn1_pin = 2; // Event trigger button
const uint8_t hold_switch_pin = 3; // iPod hold Switch
const uint8_t scan_pin = 4; // Output open-drain "button"

// Wake flags
volatile bool wake_flag = false;
volatile bool bttn0_wake_flag = false;

// Initial Gate value
uint8_t gate_val = LOW;

// Button instances
Button btn0(bttn0_pin); // enable button
Button btn1(bttn1_pin); // Event trigger button
ButtonController ev_btn(btn1);


void holdSwitchISR() {
    // digitalRead(hold_switch_pin);
}

void bttn0WakeISR() {
    bttn0_wake_flag = true;
}

void setup() {
  // initialize the Gate pin
    pinMode(gate_pin, OUTPUT);
    digitalWrite(gate_pin, gate_val);

    //Initialize the scan pin (open-drain)
    pinMode(scan_pin, INPUT); // High-Z by default
    
    // Configure interrupts
    pinMode(hold_switch_pin, INPUT);
    attachInterrupt(digitalPinToInterrupt(hold_switch_pin), holdSwitchISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(bttn0_pin), bttn0WakeISR, FALLING);
    
    // Enable global interrupts
    sei();
}

void loop() {
  if(digitalRead(hold_switch_pin)){
    main_control();
    for (uint8_t i = 0; i < 4; i++) {
        togglePIN(gate_pin, gate_val);
        delay(100);
        togglePIN(gate_pin, gate_val);
        delay(100);
    }
  }
  // else {
  //   deepSleep();
  // }
}

void main_control() {
    // Update button states. Avoid halting the FSM
    btn0.update();
    ev_btn.update();

    // Reset event controller on button0 release
    if (btn0.wasReleased()) {
        ev_btn.reset();
    }

    // Process events only when button0 is pressed
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
    } else {
      lightSleep();
    }
}

void doubleClickEvent() {
    // Simulate press scan button using open-drain
//   pinMode(scan_pin, OUTPUT);
//   digitalWrite(scan_pin, LOW);  // "Press" the button (pull line low)
//   delay(1100);                    // Hold for 100 ms
//   pinMode(scan_pin, INPUT);     // "Release" the button (go high-Z)
    // Blink gate 4 times
    for (uint8_t i = 0; i < 4; i++) {
        togglePIN(gate_pin, gate_val);
        delay(100);
        togglePIN(gate_pin, gate_val);
        delay(100);
    }
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

void deepSleep() {
  // Prepare for power-down
  btn0.disable();
  btn1.disable();
  ev_btn.reset();
    
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_cpu();
  sleep_disable();
}

void lightSleep() {
  // Prepare for standby
  btn1.disable();
  ev_btn.reset();
    
  set_sleep_mode(SLEEP_MODE_STANDBY);
  sleep_enable();
  sleep_cpu();
  sleep_disable();
}