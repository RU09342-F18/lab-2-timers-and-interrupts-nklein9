# TIMER A Blink

## Functionality
The Timer A interrupt detects a clock overflow, wakes the microprocessor from low power mode and changes the on/off state of an onboard LED. After the LED has changed state, the processor goes back to low power mode until the clock overflows again. In this implimentation two LEDs at different frequencies were flashed.

## Boards
Code is written for the MSP430G2553 and MSP430FR2311 boards.

### Differences
Major differences in the boards are the bit to which the LEDs are associated, the naming conventions of the timer Registers, the frequency of the main clock,. For The G2553 the LEDs are assigned to P1.0 and P 1.6 and the LEDs on the FR2311 are assigned to P1.0 and P2.0. The timer of the G2553 was Timer A and had the naming convention of TACTL, TACCR1, ect. The timer of the FR2311 was Timer B and had the naming convention of TBCTL, TB0CCR1, ect. The main clock of Timer A was 12kHz. The main clock of Timer B was 32kHz. The latter was set this way due to an error whenever the 12kHz clock was selected.
