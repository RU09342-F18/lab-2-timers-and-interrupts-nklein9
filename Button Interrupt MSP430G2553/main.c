/*Nicholas Klein
 *Created 9/26/18   Last Edit: 9/26/18
 *Button LED Blink with Interrupt on an MSP430G2553
 */

#include <msp430.h>

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    //LED Setup
    P1SEL &= ~BIT0;             //Sets P1.0 as an I/O pin
    P1SEL2 &= ~BIT0;            //Sets P1.0 as an I/O pin
    P1DIR |= BIT0;              //Sets P1.0 as an output

    // Button setup
    P1SEL &= ~BIT3;             //Sets P1.0 as an I/O pin
    P1SEL2 &= ~BIT3;            //Sets P1.0 as an I/O pin
    P1DIR &= ~BIT3;             // button is an input
    P1OUT |= BIT3;              // pull-up resistor
    P1REN |= BIT3;              // resistor enabled

    //Interrupt Setup
    P1IFG &= ~BIT3;             //No Interrupt Pending
    P1IE |= BIT3;               //interrupt is enabled
    P1IES |= BIT3;              //interrupt is set with high-to-low transition

    _BIS_SR(LPM0_bits + GIE);   //go to sleep
return 0;
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    P1OUT ^= BIT0;              //switches the LED on or off, depending on its current state
    P1IFG &= ~BIT3;             //No Interrupt Pending
}
