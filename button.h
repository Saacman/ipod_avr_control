#ifndef button_h
#define button_h

#include "Arduino.h"

class Button
// Handle button debounce using pull-up configuration
{
  private:
    uint8_t buttonPin;               // the number of the LED pin 0, 3, 4
    uint8_t debounceDelay;           // the debounce time; increase if the output flickers

    uint8_t buttonState = HIGH;       // the current reading from the input pin
    uint8_t lastButtonState = HIGH;  // the previous reading from the input pin (pull-up)
    bool buttonChanged = false;

    // time measured in milliseconds, will quickly become a bigger number than can be stored in an int.
    unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled

  public:

    // Constructor 
    Button(uint8_t pin, uint8_t delay = 20):
      buttonPin(pin),
      debounceDelay(delay) {
        pinMode(buttonPin, INPUT); // Using external pull-up
      }

    bool update() {
      // read the state of the button
      uint8_t reading = digitalRead(buttonPin);

      // check to see if you just pressed the button, 
      // and you've waited long enough
      // since the last press to ignore any noise:

      // Timestamp the state change, due to noise or pressing
      if (reading != lastButtonState) {
        // reset debouncing timer
        lastDebounceTime = millis();
      }

      if ((millis() - lastDebounceTime) > debounceDelay) {
        // reading lasted longer than debounce delay

        // check if button state has changed
        if (reading != buttonState) {
          buttonState = reading;
          buttonChanged = true;
        } else buttonChanged = false;

      } else buttonChanged = false;

      // save the reading
      lastButtonState = reading;
    }

    bool isPressed() const {
      return buttonState == LOW; // With pull-up, low is a pressed button
    }
    
    bool wasPressed() const {
      return buttonChanged && buttonState == LOW; // High -> Low
    }

    bool wasReleased() const {
      return buttonChanged && buttonState == HIGH; // Low -> High
    }

};

#endif