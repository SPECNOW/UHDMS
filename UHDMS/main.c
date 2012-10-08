/*
 * main.c
 */

#include "msp430x22x4.h"
#include "Car.h"
#include "ADC.h"
#include "PWM.h"
#include "AI.h"
#include "math.h"
#include <stdbool.h>

//TODO Create an AI header file and accompanying decision functions to keep main function as abstract as possible.

void main(void)
{
	InitADC();
	InitPWM();
	setDir(&car,STOP); //STOP Car
	setSpeed(&car, 100); // 0 - 255
	setLane(&car, 1);
	__bis_SR_register(GIE);

	forward = 420;
	left = 420;
	right = 420;
	left_lower = 730;
	right_lower = 730;
	hold_time_th = 150;

	PS_ = STOP;
	while(1)
	{
		NS_ = NextState();
		ND_ = NextDeci(NS_);
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
				//F_val_old = F_val;
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
		if((PS_ == 4 || PS_ == 1)  && (NS_ != 5) && (NS_&0x02 !=0x02))
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
}
