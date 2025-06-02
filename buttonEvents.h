class ButtonEvents {
  private:
    Button &bttn;
    // button timing
    uint8_t dbl_click_gap;        // max ms between clicks for double clic
    uint8_t hold_time;       // ms hold period: how long to wait for press+hold

    // state time tracking
    unsigned long down_time = 0;    // time the button was pressed down
    unsigned long up_time = 0;      // time the button was released

    // Booleans
    bool dbl_click_waiting = false; // waiting for a souble click (down)
    bool dbl_click_on_up = false;   // whether to register a double clic on next releas, or wait and click
    bool snl_click_ok = true;       // whether it's ok to do a single click
    bool ignore_up = false;         // whether to ignore the button release because click+hold was triggered
    bool wait_up = false;           // when held, whether wait for the up event 
    bool hold_event_past = false;   // whether or not the hold event happened already

  public:
    enum BttnEvent_t {
      None = 0,
      Click,
      DoubleClick,
      Hold
    };

    // Constructor
    ButtonEvents(Button &button, uint8_t dc_gap = 250, uint8_t hold_time = 1000)
      : bttn(button), dbl_click_gap(dc_gap), hold_time(hold_time) {}

    // Update method
    BttnEvent_t update() {
      BttnEvent_t event = None;

      bttn.update();
      unsigned long current_time = millis();

      if (bttn.wasPressed()) {
        down_time = current_time;
        ignore_up = false;
        wait_up = false;
        snl_click_ok = true;
        hold_event_past = false;

        if ((current_time - up_time) < dbl_click_gap && dbl_click_waiting) {
          dbl_click_on_up = true;
        } else {
          dbl_click_on_up = false;
        }

      }
    }

};