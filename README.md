# Control a mini fog machine with a motion sensor

Designed for ESP32F-based boards, specifically a NodeMCU board.

A [PIR motion sensor](https://www.adafruit.com/product/189) is used to detect
motion. Once motion is detected, the sensor raises the interrupt pin (`D1`) to
activate the remote.

Once the remote is activated, this lets the smoke generator start working.

The system is configured to run for 10 minutes and then to be in the off mode
for 15 minutes. These values are managed by `RUN_TIME` and `OFF_TIME` macros in
the code.

The remote control is activated via the `D5` pin, which goes to an optocoupler.
The NodeMCU is a 3.3V system and the remote is a ~ 12V system, hence the need
for the optocoupler.

# Developer Setup

Development is done with Arudino IDE. The system is configured to talk to
NodeMCU, using 115200 serial baud.

The rest is default.
