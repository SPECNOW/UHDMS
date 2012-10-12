/*
 * PWM.c
 *
 *  Created on: Oct 10, 2012
 *      Author: Charanpreet Parmar
 */

#include "PWM.h"
#include "..\Car\Car.h"

Car car;

void InitPWM()
{
	/*	Initialize the Pins for PWM output (PORT 4) */
	P4DIR |= BIT3+BIT4+BIT5+BIT6;
	P4OUT &= ~(BIT3+BIT4+BIT5+BIT6);//Initially LOW

	/*	Initialize Timer B Control */
	TBCTL = TBSSEL_2 + TBCLR + TBIE + MC_1 + ID_0 + TBIFG;/* Select Source 2 (SMCLK) + TimerB Clear
														   * + TimerB Interrupt Enable + Continuous Mode
														   * + Clock Prescaler 1 + TimerB Interrupt Flag */
	// TimerB will count clock ticks up to 0xFFFF (Period)
	TBCCR0 = 0x0FF0;
	TBCCR1 = 0x00F0; //PWM 1 (50%) // TimerB Control Register 1
	//TBCCR2 = 0xFFFF; //PWM 2 (25%) // TimerB Control Register 2

	TBCCTL1 = OUTMOD_6 + CCIE; //Toggle/Set + Interrupt Enable
	TBCCTL2 = OUTMOD_6 + CCIE; //Toggle/Set + Interrupt Enable
}

//Interrupt Function for PWM. Requires Car.h for Car related parameters and structures.
#pragma vector = TIMERB1_VECTOR
__interrupt void TB1_ISR(void)
{
	switch(car.Dir)
	{
		case STOP:
			P4OUT &= ~(BIT3 +BIT4 + BIT5 + BIT6); //TURN EVERTHING LOW
			TBIV = 0x00;
			break;
		case FORWARD:
		case F_RIGHT:
		case F_LEFT:
			switch(car.Dir)
			{
				case FORWARD:
					P4OUT &= ~(BIT5 + BIT6);
					break;
				case F_RIGHT:
					P4OUT |= BIT5;
					P4OUT &= ~BIT6;
					break;
				case F_LEFT:
					P4OUT |= BIT6;
					P4OUT &= ~BIT5;
					break;
			}
			switch(TBIV)
			{
				case 0x02:
					P4OUT |= BIT3;//BIT6 Goes High
					P4OUT &= ~BIT4;//BIT7 Stays Low
					break;
				case 0x0E:
					P4OUT &= ~(BIT3);
					break;
			}
			break;
		case REVERSE:
		case R_RIGHT:
		case R_LEFT:
			switch(car.Dir)
			{
				case REVERSE:
					P4OUT &= ~(BIT5 + BIT6);
					break;
				case R_RIGHT:
					P4OUT |= BIT5;
					P4OUT &= ~BIT6;
					break;
				case R_LEFT:
					P4OUT |= BIT6;
					P4OUT &= ~BIT5;
					break;
			}
			switch(TBIV)
			{

				case 0x02:
					P4OUT |= BIT4;
					P4OUT &= ~BIT3;
					break;
				case 0x0E:
					P4OUT |= BIT3;//P4OUT &= ~(BIT4); //See if this corrects the reverse being super fast
					break;
			}
			break;
	}
}
