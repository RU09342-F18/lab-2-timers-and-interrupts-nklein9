# Button Interrupt

## Functionality
The button interrupt detects a button press, wakes the microprocessor from low power mode and changed the on/off state of an onboard LED. After the LED has changed state, the processor goes back to low power mode until the button is pressed again.

## Boards
Code is written for the MSP430G2553 and MSP430FR2311 boards.

### Differences
Major differences in the boards are the bit to which the button is associated. For The G2553 the button is assigned to P1.3 and the FR2311 is assigned to P1.1.
