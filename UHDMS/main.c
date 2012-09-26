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
unsigned int ADC1_temp = 0;
unsigned int ADC2_temp = 0;
unsigned int ADC3_temp = 0;

//TODO Create an AI header file and accompanying decision functions to keep main function as abstract as possible.

void main(void)
{
	InitADC();
	InitPWM();
	setDir(&car,STOP); //STOP Car
	setSpeed(&car, 135); // 0 - 255
	setLane(&car, 1);
	__bis_SR_register(GIE);

//AI CODE *******************

	//DEFINE VARIABLES
	//int i = 0;
	forward = 405;
	left = 400;
	right = 400;
	__bis_SR_register(GIE);
	while(1)
	{
		//__delay_cycles(100000);
		ADC1_ = ADC_1();//Forward
		ADC2_ = ADC_2();//Left
		ADC3_ = ADC_3();//Right

		if( (ADC1_ < forward)) //if no threshold surpassed, drive forward.
		{
			setDir(&car,FORWARD);
		}
		if(ADC2_ > right) // if ADC2 is above threshold, turn left
		{
			setDir(&car,F_LEFT);
		}
		if(ADC3_ > left) // if ADC3 is above threshold, turn right
		{
			setDir(&car,F_RIGHT);
		}
		if( (ADC1_ > forward))
		{
			hard_stop(&car);
		}
		/*else
		{
			setDir(&car,STOP);
		}*/
		/*else if(ADC1_> forward && ADC2_> right && ADC3_ > left) // if all thresholds is surpassed, initiate stop and find route out
		{
			setDir(&car,STOP);
			ADC1_temp = ADC_1();
			ADC2_temp = ADC_2();
			ADC3_temp = ADC_3();
			if(ADC2_temp > ADC2_)//see which way has option LEFT or RIGHT
			{
				setDir(&car,R_RIGHT);
			}
			else if(ADC3_temp > ADC3_)
			{
				setDir(&car,R_LEFT);
			}
			else					// if no option stop
			{
				setDir(&car,STOP);
				ADC1_temp = 0;
				ADC2_temp = 0;
				ADC3_temp = 0;
				break;
			}
			ADC1_temp = 0;
			ADC2_temp = 0;
			ADC3_temp = 0;

		}
		if(ADC1_ < forward) //if forward is above threshold, slow down car
		{
			setDir(&car,STOP);
			ADC1_temp = ADC_1();
			ADC2_temp = ADC_2();
			ADC3_temp = ADC_3();
			if(ADC1_temp > ADC1_) // check to see if object is coming closer
			{
				if(ADC2_temp > ADC2_)
				{
					setDir(&car,F_LEFT);
				}
				else if(ADC3_temp > ADC3_)
				{
					setDir(&car,F_RIGHT);
				}
				ADC1_temp = 0;
				ADC2_temp = 0;
				ADC3_temp = 0;
				break;
			}
		}
*/

	}
}
