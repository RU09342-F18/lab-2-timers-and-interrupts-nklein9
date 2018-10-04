# Button Based Delay
## Functionality
The button based delay detects a button press, wakes the microprocessor from low power mode and uses the timer to detect and store how long the button was pressed. The processor then goes back into low power mode. The timer then counts to the length of time the button was pressed, wakes up from low power mode, changes the on/off state of the LED, then goes back into low power mode. The timer interrupt continues in this fashion until the button is pressed again where a new length of time will be stored for the timer interrupt.

## Boards
Code is written for the MSP430G2553 and MSP430FR2311 boards.

### Differences
Major differences in the boards are the bit to which the button is associated,the bit to which the LEDs are associated, the naming conventions of the timer Registers, and the frequency of the main clock. For The G2553 the button is assigned to P1.3 and the FR2311 is assigned to P1.1. For The G2553 the LEDs are assigned to P1.0 and P 1.6 and the LEDs on the FR2311 are assigned to P1.0 and P2.0. The timer of the G2553 was Timer A and had the naming convention of TACTL, TACCR1, ect. The timer of the FR2311 was Timer B and had the naming convention of TBCTL, TB0CCR1, ect. The main clock of Timer A was 12kHz. The main clock of Timer B was 32kHz. The latter was set this way due to an error whenever the 12kHz clock was selected.
