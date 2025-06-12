# ipod_avr_control
The initial plan was to use `UART` to control the `KCX BT module`. But this has some downsides:
- Pin usage: It needs 2 pins for `RX/TX`, and an additional pin to wake up the BT module.
- Power Usage: Clock frequency needs to be at least `5MHz` to use achieve 9600 baud.
- Reliability: The serial interface of the BT Module is very unreliable.
- Complexity: It would be needed to track the states of the BT module with internal variables. A mismatch would be hard to debug in a permanent solution.

I could revisit this option later.