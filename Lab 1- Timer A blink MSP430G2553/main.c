/*Nicholas Klein
 *Created 9/26/18   Last Edit: 9/26/18
 *Button LED Blink with Interrupt on an MSP430G2553
 */
#include <msp430.h> 

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	

    //LED Setup
    P1SEL &= ~BIT0;             //Sets P1.0 as an I/O pin
    P1SEL2 &= ~BIT0;            //Sets P1.0 as an I/O pin
    P1DIR |= BIT0;              //Sets P1.0 as an output
	//Clock Setup
	TACTL = TASSEL_2|ID_3|MC_2|TACLR|TAIE;
	//Compare Register Setup
	TACCR1 = 0x2000;
	TACCTL1 = CCIE;

	return 0;
}

//interrupt service routine
#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_VECTOR(void)
{
    //switch TAIV
    switch (__even_in_range(TAIV,101))
    {
        case TAIV_CCIFG1:   //Vector 2 for CCIFG
            P1.0 = 0;
            break
        case TAIV_TAIFG:    //Vector A
            P1.0=1;
            break
        default             //False Interrupt, Disaster Loop
            for(;;)
            {}
    }                   //End Switch
    }
}
