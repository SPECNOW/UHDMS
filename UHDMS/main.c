/*
 * main.c
 */

#include "msp430x22x4.h"
#include "Car\Car.h"
#include "ADC\ADC.h"
#include "PWM\PWM.h"
#include "AI\AI.h"
#include <math.h>
#include <stdbool.h>

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

	PS_ = _000;

	while(1)
	{
		StateMachine();
	}
}
