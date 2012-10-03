/*
 * main.c
 */

#include "msp430x22x4.h"
#include "Car.h"
#include "ADC.h"
#include "PWM.h"
#include "math.h"
#include <stdbool.h>

bool F_;
bool R_;
bool L_;
unsigned int F_val = 0;
unsigned int R_val = 0;
unsigned int L_val = 0;
unsigned int forward = 0, left = 0, right = 0;
unsigned int F_temp = 0;
unsigned int R_temp = 0;
unsigned int L_temp = 0;
double j = 0.0;
unsigned int isClose(unsigned int ADC, unsigned int thresh);

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
	double i = 0;
	forward = 405;
	left = 400;
	right = 400;
	__bis_SR_register(GIE);
	while(1)
	{
		//__delay_cycles(10000);
		F_val = ADC_1();
		R_val = ADC_2();
		L_val = ADC_3();
		F_ = isClose(F_val, forward);//Forward
		R_ = isClose(R_val, right);//RIGHT
		L_ = isClose(L_val, left);//LEFT


		if(!L_ && !F_  && !R_) //000
		{
			setDir(&car, FORWARD);
			j += 1.0;
		}
		else if(!L_ && !F_ && R_) //001
		{
			setDir(&car, F_LEFT);
			j += 1.0;
		}
		else if(!L_ && F_  && !R_) //010 = make left or right decision in here
		{
			setDir(&car, REVERSE);
			if(R_val > L_val)
			{
				setDir(&car, F_RIGHT);
			}
			else
			{
				setDir(&car, F_LEFT);
			}
		}
		else if(!L_ &&  F_ &&  R_) //011
		{
			setDir(&car, R_RIGHT);
		}
		else if(L_ && !F_ && !R_) //100
		{
			setDir(&car, F_RIGHT);
			j += 1.0;
		}
		else if(L_ && !F_ && R_) //101
		{
			setDir(&car, FORWARD);
			j+= 1.0;
		}
		else if(L_ && F_ && !R_) //110
		{
			setDir(&car, R_LEFT);
		}
		else if(L_ && F_ && R_) //111
		{
			i += 1;
			hard_stop(&car, j);
			j = 0.0;
			if(i == 15)
			{
				if((fabs((double)L_val - (double)R_val) <= 125.0))
				{
					setDir(&car,R_RIGHT);
					__delay_cycles(400000);
					setDir(&car,F_LEFT);
					__delay_cycles(10000);
				}
				else if(R_val > L_val)
				{
					setDir(&car,R_LEFT);
					__delay_cycles(400000);
					setDir(&car,F_RIGHT);
					__delay_cycles(10000);
				}
				else if(L_val > R_val)
				{
					setDir(&car,R_RIGHT);
					__delay_cycles(400000);
					setDir(&car,F_LEFT);
					__delay_cycles(10000);
				}
				else
				{
					setDir(&car, REVERSE);
				}
				i = 0;
			}

		}
	}
		/*if( (F_ < forward)) //if no threshold surpassed, drive forward.
		{
			setDir(&car,FORWARD);

		}
		if(R_ > right) // if ADC2 is above threshold, turn left
		{
			setDir(&car,F_LEFT);

		}
		if(L_ > left) // if ADC3 is above threshold, turn right
		{
			setDir(&car,F_RIGHT);

		}
		if( (F_ > forward))
		{
			hard_stop(&car);
			i += 1;
			if(i == 15) //Get out of corner Algorithm
			{
				F_ = ADC_1();//Forward
				R_ = ADC_2();//Left
				L_ = ADC_3();//Right
				if((fabs((double)L_ - (double)R_) <= 125.0))
				{
					setDir(&car,R_RIGHT);
					__delay_cycles(400000);
					setDir(&car,F_LEFT);
					__delay_cycles(10000);
				}
				else if(R_ > L_)
				{
					setDir(&car,R_LEFT);
					__delay_cycles(400000);
					setDir(&car,F_RIGHT);
					__delay_cycles(10000);
				}
				else if(L_ > R_)
				{
					setDir(&car,R_RIGHT);
					__delay_cycles(400000);
					setDir(&car,F_LEFT);
					__delay_cycles(10000);
				}
				else
				{
					setDir(&car, REVERSE);
				}
				i = 0;
			}
		}*/
		/*else
		{
			setDir(&car,STOP);
		}*/
		/*else if(F_> forward && R_> right && L_ > left) // if all thresholds is surpassed, initiate stop and find route out
		{
			setDir(&car,STOP);
			F_temp = ADC_1();
			R_temp = ADC_2();
			L_temp = ADC_3();
			if(R_temp > R_)//see which way has option LEFT or RIGHT
			{
				setDir(&car,R_RIGHT);
			}
			else if(L_temp > L_)
			{
				setDir(&car,R_LEFT);
			}
			else					// if no option stop
			{
				setDir(&car,STOP);
				F_temp = 0;
				R_temp = 0;
				L_temp = 0;
				break;
			}
			F_temp = 0;
			R_temp = 0;
			L_temp = 0;

		}
		if(F_ < forward) //if forward is above threshold, slow down car
		{
			setDir(&car,STOP);
			F_temp = ADC_1();
			R_temp = ADC_2();
			L_temp = ADC_3();
			if(F_temp > F_) // check to see if object is coming closer
			{
				if(R_temp > R_)
				{
					setDir(&car,F_LEFT);
				}
				else if(L_temp > L_)
				{
					setDir(&car,F_RIGHT);
				}
				F_temp = 0;
				R_temp = 0;
				L_temp = 0;
				break;
			}
		}


	}*/
}
unsigned int isClose(unsigned int ADC, unsigned int thresh)
{
	if(ADC < thresh)
	{
		return 0;
	}
	return 1;
}
