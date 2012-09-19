/*
 * main.c
 */

#include "msp430x22x4.h"
#include "Car.h"
#include "ADC.h"
#include "PWM.h"

unsigned int ADC1_ = 0;
unsigned int ADC2_ = 0;
unsigned int ADC3_ = 0;
unsigned int forward = 0, left = 0, right = 0;

//TODO Create an AI header file and accompanying decision functions to keep main function as abstract as possible.

void main(void)
{
	InitADC();
	InitPWM();
	setDir(&car,STOP); //STOP Car
	setSpeed(&car,6); // 0 - 255
	setLane(&car, 1);

	//DEFINE VARIABLES
	int i = 0;
//	int j = 0;
	forward = 250;
	left = 150;
	right = 150;
	__bis_SR_register(GIE);
	while(1)
	{
		ADC1_ = ADC_1();//Forward
		ADC2_ = ADC_2();//Left
		ADC3_ = ADC_3();//Right

		if(ADC1_ > left && ADC1_ > right && ADC1_ > 500)//tweak
		{
			if(ADC1_ > forward)
			{
				setDir(&car,STOP); //stop to avoid crash
				if(ADC1_ > 300)
				{
					while(ADC1_ > 300)
					{
						ADC1_ = ADC_1();
						setDir(&car,REVERSE); //back up a little
						i += 1;
					}
					if(i == 20)
					{
						if(ADC2_ > ADC3_) // see which side has space, left or right. This case is for left having less space
						{
							while(ADC1_ > 300 || ADC3_ > 200)
							{
								ADC1_ = ADC_1();
								ADC3_ = ADC_3();
								setDir(&car,R_LEFT);
							}
							if(ADC3_ < 300)
							{
								setDir(&car,F_RIGHT); // turn RIGHT
							}
							i = 0;
						}
						else if(ADC3_ > ADC2_) // THIS CASE FOR RIGHT HAVING LESS SPACE
						{
							while(ADC1_ > 300 || ADC2_ > 200)
							{
								ADC1_ = ADC_1();
								ADC2_ = ADC_2();
								setDir(&car,R_RIGHT);
							}
							if(ADC2_ < 300)
							{
								setDir(&car,F_LEFT); //TURN LEFT
							}
							i = 0;
						}
					}
				}

			}
			else
			{
				setDir(&car, FORWARD);
			}
		}
		else if (ADC1_ <250)
		{
			setDir(&car, FORWARD);
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
