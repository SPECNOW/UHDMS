/*
 * AI.c
 *
 *  Created on: Oct 10, 2012
 *      Author: Charanpreet Parmar and Izaak Lee
 */

#include "AI.h"

bool F_;
bool R_;
bool L_;

unsigned int F_val = 0;
unsigned int R_val = 0;
unsigned int L_val = 0;

unsigned int forward = 0, left = 0, right = 0;
unsigned int left_lower = 0, right_lower =0;

unsigned int acc_time = 0;
unsigned int hold_time = 0;
unsigned int hold_time_th = 0;

CarState NS_temp = _000;
CarDirection ND_temp = STOP;

CarState NS_ = _000;
CarDirection ND_ = STOP;

CarState PS_ = _000;
CarDirection PD_ = STOP;

unsigned int max_speed =0;
CarState NextState()
{
	F_val = ADC_1();
	R_val = ADC_2();
	L_val = ADC_3();
	R_ = isClose(R_val, right);//RIGHT
	F_ = isClose(F_val, forward);//FORWARD
	L_ = isClose(L_val, left);//LEFT

	return (CarState)(L_ << 2 | F_ << 1 | R_);
}

CarDirection NextDeci(CarState NS)
{
	if(NS == _000) //000
	{
		return FORWARD;
	}
	else if(NS == _001) //001
	{
		return F_LEFT;
	}
	else if(NS == _010) //010 = make left or right decision in here
	{
		if(R_val > L_val)
		{
			return R_RIGHT;
		}
		else
		{
			return R_LEFT;
		}
	}
	else if(NS == _011) //011
	{
		return R_RIGHT;
	}
	else if(NS == _100) //100
	{
		return F_RIGHT;
	}
	else if(NS == _101) //101
	{
		return FORWARD;
	}
	else if(NS == _110) //110
	{
		return R_LEFT;
	}
	else if(NS == _111) //111
	{
		return REVERSE;
	}

	return STOP; //won't get here
}


void StateMachine()
{
	NS_ = NextState();
	ND_ = NextDeci(NS_);
	/*if(acc_time == 7000)
	{
		max_speed = car.Speed;
		setSpeed(&car, max_speed/2);
	}
	else if(acc_time > 7000)
	{
		acc_time = 7001;
	}
	if(acc_time < 10)
	{
		setSpeed(&car, max_speed);
	}*/
	if(PD_ == ND_) // no change in direction
	{
		setDir(&car, ND_);
		acc_time += 1;
		hold_time += 1;
	}
	else //get out of complete box.
	{
		if(PS_ == 7 && NS_ == 5)
		{
			acc_time += 1;
			ND_ = PD_;
			NS_ = PS_;
		}
		else if((PD_ == FORWARD || PD_ == F_RIGHT || PD_ == F_LEFT) && ND_ == REVERSE)// get out of box
		{
			if(PS_ == 7)
			{
				hard_stop(&car, acc_time);
			}
			__delay_cycles(50000);
			NS_temp = NextState();
			ND_temp = NextDeci(NS_temp);
			if (ND_temp == FORWARD) // if no obstruction go forward
			{
				ND_ = ND_temp;
				NS_ = NS_temp;
			}
			acc_time = 0;
		}
	}//get out of complete box ends.
	if((PS_ == 4 || PS_ == 1)  && (NS_ != 5))
	{
		NS_ = PS_;
		ND_ = PD_;
		acc_time += 1;
	}
	if (ND_ != FORWARD && ND_ != REVERSE) //keep car going straight with hysteresis.
	{
		acc_time += 1;
	}
	else if(ND_ == FORWARD) //keep car going straight with hysteresis.
	{
		if(isClose(L_val, left_lower))
		{
			ND_ = F_RIGHT;
		}
		else if(isClose(R_val,right_lower))
		{
			ND_ = F_LEFT;
		}
		else
		{
			ND_ = FORWARD;
		}
		acc_time += 1;
		hold_time = hold_time_th + 1;
	}
	else if(ND_ == REVERSE) //keep car going straight with hysteresis.
	{
		if(isClose(L_val, left_lower))
		{
			ND_ = R_RIGHT;
		}
		else if(isClose(R_val, right_lower))
		{
			ND_ = R_LEFT;
		}
		else
		{
			ND_ = REVERSE;
		}
		acc_time += 1;
		hold_time = hold_time_th + 1;
	}
	if(ND_ == PD_) // already the same state just increment.
	{
		hold_time += 1;
	}
	else if(hold_time > hold_time_th) // allow changing of state if greater than 200
	{
		hold_time = 0;
		PD_ = ND_;
		PS_ = NS_;
	}
	else // Hold state to present state
	{
		ND_ = PD_;
		NS_ = PS_;
		hold_time += 1;
	}
	setDir(&car, ND_);
}
