/*
 * main.c
 */

#include "msp430x22x4.h"

#define STOP 0
#define FORWARD 1
#define REVERSE 2
#define F_LEFT 3
#define F_RIGHT 4
#define R_LEFT 5
#define R_RIGHT 6

typedef struct{
	char Speed; //0-255
	char Dir; // Use defines above
	int lane;
}Car;

int setSpeed(Car* pThis, char speed)
{
	if(speed <= 255)
	{
		pThis->Speed = speed;
		TBCCR1 = speed << 4;
		return 1;
	}

	return 0;
}
int setDir(Car* pThis,char dir)
{
	if(dir <= 6)
	{
		pThis->Dir = dir;
		return 1;
	}

	return 0;
}

void InitPWM();
void ADC_1(void);
void ADC_2(void);
void init(void);

unsigned int PWM1_value, PWM2_value;
Car car;
void main(void)
{
	init();
	InitPWM();
	setDir(&car,STOP);
	setSpeed(&car,128); //50%

	__bis_SR_register(GIE);
	while(1)
	{
		ADC_1();
		ADC_2();

	}
}

void InitPWM()
{
	/*	Initialize the Pins for PWM output (Pin 2) */
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

void init(void)
{
	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

	ADC10CTL0 = ADC10SHT_2 + MSC + ADC10ON + ADC10IE; // Sample Hold Time2, Multiple Seq Channel, ADC10ON, Interrupt Enabled
  	ADC10DTC1 = 0x02;
  	ADC10AE0 |= 0x07;                         // P2.0, P2.1, and P2.2 ADC option select
/*  P2DIR |= 0x18;                            // Set P2.3 and P2.4 to output direction for PWM
  	P2SEL |= 0x18;                            // Select P2.3 and P2.4 for for PWM output
  	P4DIR |= 0x30;                            // Set P4.4 and P4.5 to output direction for PWM
  	P4SEL |= 0x30; 							// Select P4.4 and P4.5 for for PWM output
  	P4DIR |= 0x40; //turns on 4.6 for backward (virtual break)
  	//int i = 0;*/
  	P1DIR |= 0x03;
}

void ADC_1(void)
{
  	// P1OUT = 0x03;						// Turn on both LED, Ports 11 and 14.
	ADC10CTL0 &= ~ENC;                      // Disable conversion
	ADC10CTL1 = INCH_0 + CONSEQ_0;          // Select Channel 0 , Single Channel single conversion
	ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
	while( ADC10CTL1 & ADC10BUSY );         // wait until conversion is done
	ADC10CTL0 &= ~ENC;
//	TACCR0 = 760 - 1;                       // PWM Period (760 maximum input from the IR sensor)
//    TACCTL1 = OUTMOD_7;             		// PWM reset/set
//    TACCR1 = 760 - ADC10MEM;                // Duty cycle for PWM (closer the range, less the duty cycle)
    PWM1_value = 760 - ADC10MEM;
//  	TACTL = TASSEL_2 + MC_1;				// Set Timer A Control with UP TO CCRO and SMCLK
	if ( PWM1_value <= 320)
	{
		setDir(&car,FORWARD);
		P1OUT ^=0x01;
	}
	else
	{
		setDir(&car,REVERSE);
	}
}

void ADC_2(void)
{
 	//2ND ADC
  	ADC10CTL0 &= ~ENC;                      // Disable conversion
	ADC10CTL1 = INCH_1 + CONSEQ_0;          // Select Channel 1 , Single Channel single conversion
	ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
	while( ADC10CTL1 & ADC10BUSY );         // wait until conversion is done
	ADC10CTL0 &= ~ENC;
//	TACCR0 = 760 - 1;                       // PWM Period (760 maximum input from the IR sensor)
//    TACCTL2 = OUTMOD_7;             		// PWM reset/set
//    TACCR2 = 760 - ADC10MEM;                // Duty cycle for PWM (closer the range, less the duty cycle)
    PWM2_value = 760 - ADC10MEM;
//  	TACTL = TASSEL_2 + MC_1;				// Set Timer A Control with UP TO CCRO and SMCLK
  	if (PWM2_value <= 500)
  	{
  		P1OUT ^=0x02;
  	}
}

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
			switch(TBIV)
			{
				case 0x02:
					P4OUT |= BIT4;
					P4OUT &= ~BIT3;
					break;
				case 0x0E:
					P4OUT &= ~(BIT4);
					break;
			}
			break;
		case F_RIGHT:
			break;
		case F_LEFT:
			break;
		case R_RIGHT:
			break;
		case R_LEFT:
			break;
	}
}
