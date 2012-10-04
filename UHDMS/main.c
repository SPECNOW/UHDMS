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

	while(1)
	{
		nextState();
	}
}
