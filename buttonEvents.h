enum class State: uint8_t {
    IDLE,
    PRESSED,
    HELD,
    RELEASE_WAIT,
    SECOND_PRESS
  };

enum class BttnEvent: uint8_t{
  NONE = 0,
  CLICK,
  DBL_CLICK,
  HOLD
};

class ButtonController {
  private:

    Button &bttn;
    State state;
    // button timing
    uint8_t dbl_click_gap;        // max ms between clicks for double click (max 255)
    uint16_t hold_time;       // ms hold period: how long to wait for press+hold

    // state time tracking
    unsigned long down_time = 0;    // time the button was pressed down
    unsigned long up_time = 0;      // time the button was released

  public:
    // Constructor
    ButtonController(Button &button, uint8_t dc_gap = 255, uint16_t hold_time = 1500)
      : bttn(button), dbl_click_gap(dc_gap), hold_time(hold_time) {}

    // Update method
    BttnEvent update() {
      // first update the button status.
      bttn.update();

      BttnEvent event = BttnEvent::NONE;
      unsigned long current_time = millis();

      switch (state) {
      case State::IDLE:
        if (bttn.wasPressed()) {
          down_time = current_time;
          state = State::PRESSED;
        }
        break;
      case State::PRESSED:
        if (bttn.isPressed() && (current_time - down_time >= hold_time)) {
          event = BttnEvent::HOLD;
          state = State::HELD;
        }
        else if (bttn.wasReleased()){
          up_time = current_time;
          state = State::RELEASE_WAIT;
        }
        break;
      case State::HELD:
        if (bttn.wasReleased()) {
          state = State::IDLE;
        }
        break;
      case State::RELEASE_WAIT:
        if (bttn.wasPressed() && (current_time - up_time <= dbl_click_gap)) {
          state = State::SECOND_PRESS;
        }
        else if (current_time - up_time > dbl_click_gap) {
          event = BttnEvent::CLICK;
          state = State::IDLE;
        }
        break;
      case State::SECOND_PRESS:
        if (bttn.wasReleased()) {
          event = BttnEvent::DBL_CLICK;
          state = State::IDLE;
        }
        break;
      }
      return event;
    }

};