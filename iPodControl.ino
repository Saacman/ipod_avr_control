#include <avr/sleep.h>
#include <avr/interrupt.h>

#include "button.h"
#include "buttonEvents.h"

// Pins
const uint8_t gate_pin = 1;
const uint8_t bttn0_pin = 0; // Enable button
const uint8_t bttn1_pin = 2; // Even trigger button
const uint8_t hold_switch_pin = 3; // iPod hold Switch
const uint8_t scan_pin = 4; // Output open-drain "button"

// Wake flags
volatile bool wake_flag = false;
volatile bool bttn0_wake_flag = false;

// Initial Gate value (N-MOSFET) TODO: Change to P-MOSFET
uint8_t gate_val = LOW;

// Button instances
Button btn0(bttn0_pin); // enable button
Button btn1(bttn1_pin); // Event trigger button
ButtonController ev_btn(btn1);

void setup() {
  // initialize the Gate pin
  pinMode(gate_pin, OUTPUT);
  digitalWrite(gate_pin, gate_val);

  // Set up hold switch input & interrupt
  pinMode(hold_switch_pin, INPUT);
  attachInterrupt(digitalPinToInterrupt(hold_switch_pin), holdSwitchISR, RISING);
  
  // Wake interrupt from enable button
  attachInterrupt(digitalPinToInterrupt(bttn0_pin), bttn0WakeISR, FALLING);

  //Initialize the scan pin (oepn-drain)
  pinMode(scan_pin, INPUT);  // High-Z (released)
 
  // Set up sleep
  sleep_enable();
  sei();  // Enable global interrupts
}

void holdSwitchISR(){
  wake_flag = true;
}

void bttn0WakeISR(){
  bttn0_wake_flag = true;
}

void loop() {
  // clear flags
  if(wake_flag || bttn0_wake_flag) {
    wake_flag = false;
    bttn0_wake_flag = false;
  }

  main_control();

  // Prepare for sleep
  if(!wake_flag && !digitalRead(hold_switch_pin)){
    deepSleep();
  } else if (!bttn0_wake_flag && !btn0.isPressed()) {
    lightSleep();
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
  // NOT USED
  return;
}

void doubleClickEvent() {
  // Simulate press scan button using open-drain
//   pinMode(scan_pin, OUTPUT);
//   digitalWrite(scan_pin, LOW);  // "Press" the button (pull line low)
//   delay(1100);                    // Hold for 100 ms
//   pinMode(scan_pin, INPUT);     // "Release" the button (go high-Z)
  togglePIN(gate_pin, gate_val);
  delay(100);
  togglePIN(gate_pin, gate_val);
  delay(100);
  togglePIN(gate_pin, gate_val);
  delay(100);
  togglePIN(gate_pin, gate_val);
  delay(100);
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
  // Full power down — disable all button pullups
  btn0.disable();  // safe: will wake via interrupt
  btn1.disable();

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_cpu();
  sleep_disable();
}

void lightSleep() {
  // Allow fast wake up
  btn1.disable();  // not needed during sleep

  set_sleep_mode(SLEEP_MODE_STANDBY);
  sleep_enable();
  sleep_cpu();
  sleep_disable();
}
