/*Nicholas Klein
 *Created 9/30/18   Last Edit: 10/1/18
 *Timer_A LED Blink from Interrupt on an MSP430FR2311
 */


#include <msp430.h>

int setFreq(int hz)
{
    //counts pos edge of 12kHz clock to the ratio, giving a response at the desired frequency
    int new = 16000/hz;

    return new;
}

void main(void)
{
    CSCTL4 = 0x0100;                    // Set ACLK to REFO at 32kHZ, SELA_VLO doesn't exist
    WDTCTL = WDTPW | WDTHOLD;           // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;               //Unlocks GPIO pins

    //LED Setup
    P1SEL0 &= ~BIT0;                     //Sets P1.0 as an I/O pin
    P1SEL1 &= ~BIT0;                    //Sets P1.0 as an I/O pin
    P1DIR |= BIT0;                      //Sets P1.0 as an output
    P2SEL0 &= ~BIT0;                     //Sets P2.0 as an I/O pin
    P2SEL1 &= ~BIT0;                    //Sets P2.0 as an I/O pin
    P2DIR |= BIT0;                      //Sets P2.0 as an output

    //Clock Setup
    TBCTL = TBSSEL_1 + MC_2 + TBCLR;    //ACLK, Continuous Mode, clear TAR

    //Interrupt flag timing frequency setup
    TB0CCR1 = setFreq(2);                //LED 1 interrupt set for 2Hz
    TB0CCR2 = setFreq(5);                //LED 2 interrupt set for 5Hz

    //Enable interrupts
    TB0CCTL1 = CCIE;                     //Capture/Compare Interrupt enabled for LED1
    TB0CCTL2 = CCIE;                     //Capture/Compare Interrupt enabled for LED2

    _BIS_SR(LPM3_bits + GIE);           //Enter Low Power Mode
}

//interrupt service routine
#pragma vector=TIMER0_B1_VECTOR
__interrupt void Timer0_B1(void)
{
    //Switch TAIV, automatically resets the flags
    switch(__even_in_range(TB0IV, 10))
    {
        case 2:                         //If TACCR1 is read
        {
            P1OUT ^= BIT0;              //switch state of LED1
            TB0CCR1 += setFreq(2);
            break;
        }
        case 4:                         //If TACCR2 is read
        {
            P2OUT ^= BIT0;              //Switch state of LED2
            TB0CCR2 += setFreq(5);
            break;
        }
        default:
        {
            break;
        }
    }
}
