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
	setSpeed(&car, 135); // 0 - 255
	setLane(&car, 1);
	__bis_SR_register(GIE);

	forward = 405;
	left = 400;
	right = 400;
	PS_ = STOP;
	while(1)
	{
		NS_ = NextState();
		ND_ = NextDeci(NS_);
		if(PD_ == ND_)
		{
			setDir(&car, ND_);
			acc_time += 1;
		}
		else
		{
			if(PS_ == 7 && NS_ == 5)
			{
				acc_time += 1;
				ND_ = PD_;
				NS_ = PS_;
			}
			else if((PD_ == FORWARD || PD_ == F_RIGHT || PD_ == F_LEFT) && ND_ == REVERSE)
			{
				//F_val_old = F_val;
				if(PD_ == ND_ == 7)
				{
					hard_stop(&car, acc_time);
					acc_time = 0;
				}
				__delay_cycles(50000);
				NS_temp = NextState();
				ND_temp = NextDeci(NS_temp);
				if (ND_temp == FORWARD)
				{
					ND_ = ND_temp;
					NS_ = NS_temp;
				}
			}

		}
		setDir(&car, ND_);
		PD_ = ND_;
		PS_ = NS_;
	}
}
