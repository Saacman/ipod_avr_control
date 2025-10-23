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
    State state = State::IDLE;
    BttnEvent lastEvent = BttnEvent::NONE; // Store detected event

    // button timing
    uint8_t dbl_click_gap;        // max ms between clicks for double click (max 255)
    uint16_t hold_period;       // ms hold period: how long to wait for press+hold

    // state time tracking
    unsigned long down_time = 0;    // time the button was pressed down
    unsigned long up_time = 0;      // time the button was released

  public:
    // Constructor
    ButtonController(Button &button, uint8_t dc_gap = 255, uint16_t hold_period = 1500)
      : bttn(button), dbl_click_gap(dc_gap), hold_period(hold_period) {}

    // Update method
    void update() {
      // first update the button status.
      bttn.update();
      unsigned long current_time = millis();
      BttnEvent event = BttnEvent::NONE;

      switch (state) {
      case State::IDLE:
        if (bttn.wasPressed()) {
          down_time = current_time;
          state = State::PRESSED;
        }
        break;
      case State::PRESSED:
        if (bttn.isPressed() && (current_time - down_time >= hold_period)) {
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
          state = State::IDLE; // Reset after hold release
        }
        break;
      case State::RELEASE_WAIT:
        if (bttn.wasPressed() && (current_time - up_time <= dbl_click_gap)) {
          state = State::SECOND_PRESS;
        }
        else if (current_time - up_time > dbl_click_gap) {
          event = BttnEvent::CLICK;
          state = State::IDLE; // Reset after single click
        }
        break;
      case State::SECOND_PRESS:
        if (bttn.wasReleased()) {
          event = BttnEvent::DBL_CLICK;
          state = State::IDLE; // Reset after double click
        }
        break;
      }
      // Store detected event
      if(event != BttnEvent::NONE){
        lastEvent = event;
      }
    }

    BttnEvent getEvent() {
      BttnEvent event = lastEvent;
      lastEvent = BttnEvent::NONE; // Clear event after fetching
      return event;
    }

    void reset() {
      state = State::IDLE;
      lastEvent = BttnEvent::NONE;
    }

};