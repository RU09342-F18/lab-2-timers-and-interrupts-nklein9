/*Nicholas Klein
 *Created 9/29/18   Last Edit: 10/1/18
 *Timer_A LED Blink from Interrupt on an MSP430G2553
 */


#include <msp430.h>

int setFreq(int hz)
{
    //counts pos edge of 12kHz clock to the ratio, giving a response at the desired frequency
    int new = 6000/hz;

    return new;
}

void main(void)
{
    BCSCTL3 = LFXT1S_2;                 // Set ACLK to VLOCLK at 12kHZ
    WDTCTL = WDTPW | WDTHOLD;           // stop watchdog timer

    //LED Setup
    P1SEL &= ~0x41;                     //Sets P1.0 & P1.6 as an I/O pin
    P1SEL2 &= ~0x41;                    //Sets P1.0 & P1.6 as an I/O pin
    P1DIR |= 0x41;                      //Sets P1.0 & P1.6 as an output
    //P1OUT &= ~0x41;                   //Sets LED1&2 as off to start

    //Clock Setup
    TACTL = TASSEL_1 + MC_2 + TACLR;    //ACLK, Continuous Mode, clear TAR

    //Interrupt flag timing frequency setup
    TACCR1 = setFreq(2);                //LED 1 interrupt set for 2Hz
    TACCR2 = setFreq(5);                //LED 2 interrupt set for 5Hz

    //Enable interrupts
    TACCTL1 = CCIE;                     //Capture/Compare Interrupt enabled for LED1
    TACCTL2 = CCIE;                     //Capture/Compare Interrupt enabled for LED2

    _BIS_SR(LPM3_bits + GIE);           //Enter Low Power Mode
}

//interrupt service routine
#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer0_A1(void)
{
    //Switch TAIV, automatically resets the flags
    switch(__even_in_range(TAIV, 10))
    {
        case 2:                         //If TACCR1 is read
        {
            P1OUT ^= BIT0;              //switch state of LED1
            TACCR1 += setFreq(2);
            break;
        }
        case 4:                         //If TACCR2 is read
        {
            P1OUT ^= BIT6;              //Switch state of LED2
            TACCR2 += setFreq(5);
            break;
        }
        default:
        {
            break;
        }
    }
}
