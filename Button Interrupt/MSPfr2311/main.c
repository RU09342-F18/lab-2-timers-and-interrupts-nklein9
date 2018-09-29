/*Nicholas Klein
 *Created 9/26/18   Last Edit: 9/26/18
 *Button LED Blink with Interrupt on an MSP430fr2311
 */

#include <msp430.h>

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    //LED Setup
    P1SEL0 &= ~BIT0;            //Sets P1.0 as an I/O pin
    P1SEL1 &= ~BIT0;            //Sets P1.0 as an I/O pin
    P1DIR |= BIT0;              //Sets P1.0 as an output
    P1OUT |= BIT0;              //Starts the LED as off

    // Button setup
    P1SEL0 &= ~BIT1;             //Sets P1.0 as an I/O pin
    P1DIR &= ~BIT1;             // button is an input
    P1OUT |= BIT1;              // pull-up resistor
    P1REN |= BIT1;              // resistor enabled

    //Interrupt Setup
    P1IFG &= ~BIT1;             //No Interrupt Pending
    P1IE |= BIT1;               //interrupt is enabled
    P1IES |= BIT1;              //interrupt is set with high-to-low transition

    _BIS_SR(LPM0_bits + GIE);   //go to sleep
return 0;
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    P1OUT ^= BIT0;              //switches the LED on or off, depending on its current state
    P1IFG &= ~BIT1;             //No Interrupt Pending
}
