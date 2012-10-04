/*
 * AI.h
 *
 *  Created on: Oct 3, 2012
 *      Author: Charanpreet Parmar and Izaak Lee
 */

#ifndef AI_H_
#define AI_H_

#include "ADC.h"
#include "Car.h"

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
double i = 0;

unsigned int nextState()
{
	F_val = ADC_1();
	R_val = ADC_2();
	L_val = ADC_3();
	F_ = isClose(F_val, forward);//FORWARD
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

	return 0;
}


#endif /* AI_H_ */
