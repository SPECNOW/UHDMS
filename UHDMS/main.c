/*
 * main.c
 */

#include "UHDMS.h"

void main(void)
{
	Init();
	while(1)
	{
		StateMachine();
	}
}
void Init()
{
	InitADC();
	InitPWM();
	setDir(&car,STOP); //STOP Car
	setSpeed(&car, 200); // 0 - 255
	setLane(&car, 1);

	forward = 420;
	left = 250;
	right = 250;
	left_lower = 730;
	right_lower = 730;
	hold_time_th = 150;

	PS_ = _000;

	__bis_SR_register(GIE);
}
