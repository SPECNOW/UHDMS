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
		pThis->Speed = 255 - speed;
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

void setLane(Car* pThis, int lane)
{
	pThis->lane = lane;
}

void InitPWM();
void init(void);
unsigned int ADC_1();
unsigned int ADC_2();
unsigned int ADC_3();
unsigned int ADC1_ = 0;
unsigned int ADC2_ = 0;
unsigned int ADC3_ = 0;
unsigned int forward = 0, left = 0, right = 0;

Car car;
void main(void)
{
	init();
	InitPWM();
	setDir(&car,STOP); //STOP Car
	setSpeed(&car,26); //50%
	setLane(&car, 1);

	//DEFINE VARIABLES
	int i = 0;
	int j = 0;
	forward = 250;
	left = 150;
	right = 150;
	__bis_SR_register(GIE);
	while(1)
	{
		ADC1_ = ADC_1();//Forward
		ADC2_ = ADC_2();//Left
		ADC3_ = ADC_3();//Right


		if(ADC1_ > left && ADC1_ > right)//tweak
			{
				if(ADC1_ > forward)
				{
					setDir(&car,STOP);
					if(ADC1_ > 300)
					{
						setDir(&car,REVERSE);
						setDir(&car,STOP);
					}
					i += 1;
					if(i == 50 && j != 5)
					{
						setDir(&car,REVERSE);
						i = 0;
						//__delay_cycles(50000);
						//j += 1;
					}
					else if(j == 5)
					{
						if(ADC2_ > ADC3_)
						{
							setDir(&car,R_LEFT);
							__delay_cycles(50000);
							j = 0;
						}
					}
				}
				else
				{
					setDir(&car, FORWARD);
				}
			}
			else if(ADC2_ > left && ADC3_ < right)
			{
				setDir(&car, F_LEFT);
			}
			else if(ADC2_ < left && ADC3_ > right)
			{
				setDir(&car, F_RIGHT);
			}
	}



}



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

void init(void)
{
	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

	ADC10CTL0 = ADC10SHT_2 + MSC + ADC10ON + ADC10IE; // Sample Hold Time2, Multiple Seq Channel, ADC10ON, Interrupt Enabled
  	ADC10DTC1 = 0x03;						//3 Conversions
  	ADC10AE0 |= 0x07;                         // P2.0, P2.1, and P2.2 ADC option select (3 ADCs turned on)
  	P1DIR |= 0x03;
}

unsigned int ADC_1()
{
	int ADC1_value;
	ADC10CTL0 &= ~ENC;                      // Disable conversion
	ADC10CTL1 = INCH_0 + CONSEQ_0;          // Select Channel 0 , Single Channel single conversion
	ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
	while( ADC10CTL1 & ADC10BUSY );         // wait until conversion is done
	ADC10CTL0 &= ~ENC;
    ADC1_value = ADC10MEM;
//	if ( ADC1_value >= 320)
//	{
//		setDir(&car,FORWARD);
//		P1OUT ^=0x02;
//	}
	ADC10CTL1 &= ~INCH_0;
	return ADC1_value;
}

unsigned int ADC_2()
{
 	//2ND ADC
	unsigned int ADC2_value;
  	ADC10CTL0 &= ~ENC;                      // Disable conversion
	ADC10CTL1 = INCH_1 + CONSEQ_0;          // Select Channel 1 , Single Channel single conversion
	ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
	while( ADC10CTL1 & ADC10BUSY );         // wait until conversion is done
	ADC10CTL0 &= ~ENC;
    ADC2_value = ADC10MEM;
  	if (ADC2_value <= 500)
  	{
  		//P1OUT ^=0x01;
  	}
  	ADC10CTL1 &= ~INCH_1;
  	return ADC2_value;
}

unsigned int ADC_3()
{
	//3rd ADC
	unsigned int  ADC3_value;
	ADC10CTL0 &=~ENC;
	ADC10CTL1 = INCH_2 + CONSEQ_0;
	ADC10CTL0 |= ENC + ADC10SC;
	while( ADC10CTL1 & ADC10BUSY );
    ADC3_value = ADC10MEM;
  	if (ADC3_value <= 500)
  	{
  		//P1OUT ^=0x02;
  	}
  	ADC10CTL1 &= ~INCH_2;
  	return ADC3_value;
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
					P4OUT &= ~(BIT4);
					break;
			}
			break;
	}
}
/*	if(ADC1_ > forward && ADC2_ > left && ADC3_ > right)
		{

		}
		else if(ADC1_ < forward)
		{
			//slow down
			if(ADC2_ > ADC3_)
			{
				//Turn Right
			}
			else//NOTE: When ADC2_ = ADC3_, it turns left
			{
				//Turn Left
			}
		}
		else if(ADC1_ < forward && ADC2_ < left && ADC3_ < right)
		{
			//Stop
			//delay
			//check ADC again, turn left or right or reverse
		}*/
		//delay
		//__delay_cycles(10000);
