/*Nicholas Klein
 *Created 10/1/18   Last Edit: 10/1/18
 *Button LED Delay from Interrupt on an MSP430G2553
 */


#include <msp430.h>

int setFreq(int hz)
{
    //counts pos edge of 12kHz clock to the ratio, giving a response at the desired frequency
    int new = 750/hz;

    return new;
}

void main(void)
{
    BCSCTL3 = LFXT1S_2;                 // Set ACLK to VLOCLK at 12kHZ
    WDTCTL = WDTPW | WDTHOLD;           // stop watchdog timer

    //LED Setup
    P1SEL &= ~BIT0;                     //Sets P1.0 as an I/O pin
    P1SEL2 &= ~BIT0;                    //Sets P1.0 as an I/O pin
    P1DIR |= BIT0;                      //Sets P1.0 as an output

    // Button setup
    P1SEL &= ~BIT3;                     //Sets P1.0 as an I/O pin
    P1SEL2 &= ~BIT3;                    //Sets P1.0 as an I/O pin
    P1DIR &= ~BIT3;                     // button is an input
    P1OUT |= BIT3;                      // pull-up resistor
    P1REN |= BIT3;                      // resistor enabled

    //Clock Setup
    TACTL = TASSEL_1 + ID_3 + MC_1 + TACLR + TAIE;    //ACLK, /8, Continuous Mode, clear TAR, Enable Interrupt

    //Interrupt flag timing frequency setup
    TACCR0 = setFreq(10);                //LED 1 interrupt set for 10Hz

    //Interrupt Setup
    P1IFG &= ~BIT3;                     //No Interrupt Pending
    P1IE |= BIT3;                       //interrupt is enabled
    P1IES |= BIT3;                      //interrupt is set with high-to-low transition

    _BIS_SR(LPM3_bits + GIE);           //Enter Low Power Mode
}

//interrupt service routine
#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A1(void)
{
    // Switch TAIV, automatically resets the flags
    switch (__even_in_range(TAIV, 10))
    {
    case 10:                                            // Timer overflow
        P1OUT ^= BIT0;
        break;
    }
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    if (P1IES & BIT3)                                   // Button is pressed
    {
        TACTL = TASSEL_1 + ID_3 + MC_2 + TACLR;         // ACLK, /8, continuous, clear TAR
    }
    else                                                // Button is not pressed
    {
        TACCR0 = TAR >> 1;                              // Update the timer length
        TACTL = TASSEL_1 + ID_3 + MC_1 + TACLR + TAIE;  // ACLK, /8, up, clear TAR, enable interrupt
    }

    P1IES ^= BIT3; // P1.3 toggle transition
    P1IFG &= ~BIT3; // Clear the P1.3 interrupt
}
