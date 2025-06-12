#include "button.h"
#include "buttonEvents.h"

//<--  IMPORTANT -->
// To use UART on the attiny402
// Make sure to plug the correct pins
// according to USART0
// or switch to USART1 before Serial.begin() is called

// Input buttons pins
const uint8_t bttn0_pin = 3;
const uint8_t bttn1_pin = 2;

// Button debounced handles and event controller
Button btn0(bttn0_pin);
Button btn1(bttn1_pin);
ButtonController ev_btn(btn1);

void setup() {
  // initialize the LED pins as an output:
  Serial.begin(9600);
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
  Serial.println("AT+SCAN");
  delay(1000);
}
void doubleClickEvent() {
  Serial.println("AT+RESET");
  delay(1000);
}
void holdEvent() {
  Serial.println("AT+POWER_OFF");
  delay(1000);
}