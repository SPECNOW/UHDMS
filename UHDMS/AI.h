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
unsigned int left_lower = 0, right_lower =0;
unsigned int F_temp = 0;
unsigned int R_temp = 0;
unsigned int L_temp = 0;
unsigned int acc_time = 0;
unsigned int hold_time = 0;
unsigned int hold_time_th = 0;
double i = 0;
unsigned int F_val_old = 0;
unsigned int R_val_old = 0;
unsigned int L_val_old = 0;
unsigned int NS_temp = 0;
unsigned int ND_temp = 0;
unsigned int NS_ = 0;
unsigned int ND_ = 0;
unsigned int PS_ = 0;
unsigned int PD_ = 0;

unsigned int NextState()
{
	F_val = ADC_1();
	R_val = ADC_2();
	L_val = ADC_3();
	F_ = isClose(F_val, forward);//FORWARD
	R_ = isClose(R_val, right);//RIGHT
	L_ = isClose(L_val, left);//LEFT

	return (L_ << 2 | F_ << 1 | R_);
}

unsigned int NextDeci(unsigned int NS)
{
	if(NS == 0) //000
	{
		return FORWARD;
	}
	else if(NS == 1) //001
	{
		return F_LEFT;
	}
	else if(NS == 2) //010 = make left or right decision in here
	{
		if(R_val > L_val)
		{
			return R_LEFT;
		}
		else
		{
			return R_RIGHT;
		}
	}
	else if(NS == 3) //011
	{
		return R_RIGHT;
	}
	else if(NS == 4) //100
	{
		return F_RIGHT;
	}
	else if(NS == 5) //101
	{
		return FORWARD;
	}
	else if(NS == 6) //110
	{
		return R_LEFT;
	}
	else if(NS == 7) //111
	{
		return REVERSE;
		/*i += 1;
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
		}*/

	}

	return 0;
}


#endif /* AI_H_ */
