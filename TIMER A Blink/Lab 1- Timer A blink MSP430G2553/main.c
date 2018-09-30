/*Nicholas Klein
 *Created 9/29/18   Last Edit: 9/29/18
 *Timer_A LED Blink from Interrupt on an MSP430G2553
 */


#include <msp430.h>

int setFreq(int hz)
{
    //counts pos edge of 12kHz clock to the ratio, giving a response at the desired frequency
    int new = 12000/hz;

    return new;
}

void main(void)
{
    BCSCTL3 = LFXT1S_2;               // Set ACLK to VLOCLK at 12kHZ
    WDTCTL = WDTPW | WDTHOLD;           // stop watchdog timer

    //LED Setup
    P1SEL &= ~0x41;                    //Sets P1.0 & P1.6 as an I/O pin
    P1SEL2 &= ~0x41;                   //Sets P1.0 & P1.6 as an I/O pin
    P1DIR |= 0x41;                     //Sets P1.0 & P1.6 as an output
    //P1OUT &= ~0x41;                    //Sets LED1&2 as off to start

    //Clock Setup
    TACTL = TASSEL_1 + MC_1 + TACLR;   //ACLK, Up Mode, clear TAR

    //Interrupt flag timing frequency setup
    TACCR1 = setFreq(2);                //LED 1 interrupt set for 2Hz
    TACCR2 = setFreq(5);                //LED 2 interrupt set for 5Hz

    //Enable interrupts
    TACCTL1 = CCIE;                     //Capture/Compare Interrupt enabled for LED1
    TACCTL2 = CCIE;                     //Capture/Compare Interrupt enabled for LED2

    _BIS_SR(LPM0_bits + GIE);           //Enter Low Power Mode
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
            break;
        }
        case 4:                         //If TACCR2 is read
        {
            P1OUT ^= BIT6;              //Switch state of LED2
            break;
        }
        default:
        {
            break;
        }
    }
}




//-----------------------------------------
//Old Attempts below



/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2012, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************
 *
 *                       MSP430 CODE EXAMPLE DISCLAIMER
 *
 * MSP430 code examples are self-contained low-level programs that typically
 * demonstrate a single peripheral function or device feature in a highly
 * concise manner. For this the code may rely on the device's power-on default
 * register values and settings such as the clock configuration and care must
 * be taken when combining code from several examples to avoid potential side
 * effects. Also see www.ti.com/grace for a GUI- and www.ti.com/msp430ware
 * for an API functional library-approach to peripheral configuration.
 *
 * --/COPYRIGHT--*/
//******************************************************************************
//  MSP430G2xx3 Demo - Timer_A, Toggle P1.0, Overflow ISR, 32kHz ACLK
//
//  Description: Toggle P1.0 using software and the Timer_A overflow ISR.
//  In this example an ISR triggers when TA overflows. Inside the ISR P1.0
//  is toggled. Toggle rate is exactly 0.5Hz. Proper use of the TA0IV interrupt
//  vector generator is demonstrated.
//  ACLK = TACLK = 32768Hz, MCLK = SMCLK = default DCO
//  //* An external watch crystal on XIN XOUT is required for ACLK *//
//
//           MSP430G2xx3
//         ---------------
//     /|\|            XIN|-
//      | |               | 32kHz
//      --|RST        XOUT|-
//        |               |
//        |           P1.0|-->LED
//
//  D. Dang
//  Texas Instruments Inc.
//  December 2010
//   Built with CCS Version 4.2.0 and IAR Embedded Workbench Version: 5.10
//******************************************************************************
/*
#include <msp430.h>

int setFreq(int Hz)
{
    int new = 12000/Hz;
    return new;
}

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  //LED Setup
  P1SEL &= ~0x41;                           //Sets P1.0, P1.6 as an I/O pin
  P1SEL2 &= ~0x41;                          //Sets P1.0, P1.6  as an I/O pin
  P1DIR |= 0x41;                            //Sets P1.0, P1.6  as an output
  P1OUT &= ~0x41;                           //LED1 and LED2 off by default

  //Clock Setup
  TACTL = TASSEL_1 + MC_2 + TAIE;           // ACLK, contmode, interrupt

  //Setup Overflow timers
  TACCR1 = setFreq(2);                      //LED 1 blinks at 2Hz
  TACCR2 = setFreq(5);                      //LED 2 blinks at 5Hz
  TACCTL1 = CCIE;                           // Enable interrupt for LED1
  TACCTL2 = CCIE;                           // Enable interrupt for LED2

  __bis_SR_register(LPM3_bits + GIE);       // Enter LPM3 w/ interrupt
}

//interrupt service routine
#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer0_A1(void)
{
    // Switch TAIV, automatically resets the flags
    //switch(__even_in_range(TAIV, 10))
    //{
   // case 2:
        // Flip LED1
        P1OUT ^= BIT0;
      //  break;
  //  case 4:
        // Flip LED2
        P1OUT ^= BIT6;
        //break;
   // default:
        // Spin in circles
      //  for (;;) {} // false interrupt, disaster loop
   // }
        TACCR1 += setFreq(2);
        TACCR2 += setFreq(5);
}
// First try
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
    //LED Setup
    P1SEL &= ~BIT0;             //Sets P1.0 as an I/O pin
    P1SEL2 &= ~BIT0;            //Sets P1.0 as an I/O pin
    P1DIR |= BIT0;              //Sets P1.0 as an output
	//Clock Setup
	TACTL = TASSEL_2|ID_3|MC_2|TACLR|TAIE;
	    //Above: SMCLK is used, divided by 8, clock is in continuous mode, TimerA is cleared, and TimerA interrupt is enabled
	//Compare Register Setup
	TACCR1 = 0x2000;            //TimerA will count to 0x2000
	TACCTL1 = CCIE;             //Capture/Compare interrupt is enabled

	_BIS_SR(LPM0_bits + GIE);   //go to sleep
	return 0;
}

//interrupt service routine
#pragma vector=Timer0_A1_Vector
__interrupt void Timer0_A1(void)
    {
    // Switch TAIV, automatically resets the flags
    switch(__even_in_range(TAIV, 10))
    {
    case TAIV_CCIFG1:
        // Turn the LED off
        P1OUT &= ~BIT0;
        break;
    case TAIV_TAIFG:
        // Turn the LED on
        P1OUT |= BIT0;
        break;
    default:
        // Spin in circles
        for (;;) {} // false interrupt, disaster loop
    }
}*/
