/*
 * Car.c
 *
 *  Created on: Oct 10, 2012
 *      Author: Charanpreet Parmar and Izaak Lee
 */

#include <stdbool.h>
#include <math.h>
#include "msp430f2274.h"
#include "Car.h"

//Function to set Car Speed, between 0 and 255 (0x00 and 0xFF). Returns 1 if successful.
bool setSpeed(Car* pThis, char speed)
{
	if(speed <= 255)
	{
		pThis->Speed = speed;
		TBCCR1 &= 0x0000;
		TBCCR1 = (255 - speed) * 16;
		return true;
	}
	return false;
}

//Function to set Car Direction using the Direction Macros (Defined in Car.h). Returns 1 if successful.
bool setDir(Car* pThis, CarDirection dir)
{
	if(dir <= R_RIGHT)
	{
		pThis->Dir = dir;
		return true;
	}

	return false;
}

//Function to set Current lane of Car.
void setLane(Car* pThis, int lane)
{
	pThis->lane = lane;
}

void hard_stop(Car* pThis, unsigned int j)
{
	unsigned int i = 0;
	if (pThis->Dir == FORWARD || pThis->Dir == F_RIGHT ||pThis->Dir == F_LEFT )
	{
		setDir(pThis, STOP);
		setDir(pThis, REVERSE);
		while(i != 1000)//multiple
			i += 1;
		setDir(pThis, STOP);
	}
	else if(pThis->Dir == REVERSE || pThis->Dir == R_RIGHT ||pThis->Dir == R_LEFT)
	{
		setDir(pThis, STOP);
		setDir(pThis, FORWARD);
		while(i != 1000)
			i += 1;
		setDir(pThis, STOP);
	}
}
