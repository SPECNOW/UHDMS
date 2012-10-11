/*
 * Car.h
 *
 *  Created on: Sep 18, 2012
 *      Author: Charanpreet Parmar
 */

#ifndef CAR_H_
#define CAR_H_

#include <stdbool.h>
#include <math.h>
#include "msp430f2274.h"

//Car Directions
typedef enum{
	STOP,
	FORWARD,
	REVERSE,
	F_LEFT,
	F_RIGHT,
	R_LEFT,
	R_RIGHT
}CarDirection;

//Car States
typedef enum{
	_000,
	_001,
	_010,
	_011,
	_100,
	_101,
	_110,
	_111
}CarState;

//Car
typedef struct{
	unsigned int Speed; //0-255
	CarDirection Dir; // Use defines above
	int lane;
}Car;

bool setSpeed(Car* pThis, char speed);
bool setDir(Car* pThis, CarDirection dir);
void setLane(Car* pThis, int lane);
void hard_stop(Car* pThis, unsigned int j);

#endif /* CAR_H_ */

