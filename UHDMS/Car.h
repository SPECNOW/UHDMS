/*
 * Car.h
 *
 *  Created on: Sep 18, 2012
 *      Author: Charanpreet Parmar
 */

#ifndef CAR_H_
#define CAR_H_

// Define Direction Macros
#define STOP 0
#define FORWARD 1
#define REVERSE 2
#define F_LEFT 3
#define F_RIGHT 4
#define R_LEFT 5
#define R_RIGHT 6

//Create a Data Structure for Car Data
typedef struct{
	char Speed; //0-255
	char Dir; // Use defines above
	int lane;
}Car;

//Function to set Car Speed, between 0 and 255 (0x00 and 0xFF). Returns 1 if successful.
int setSpeed(Car* pThis, char speed)
{
	if(speed <= 255)
	{
		pThis->Speed = 255 - speed;
		TBCCR1 = speed << 4;
		return 1;
	}

	return 0;
}

//Function to set Car Direction using the Direction Macros (Defined in Car.h). Returns 1 if successful.
int setDir(Car* pThis,char dir)
{
	if(dir <= 6)
	{
		pThis->Dir = dir;
		return 1;
	}

	return 0;
}

//Function to set Current lane of Car.
void setLane(Car* pThis, int lane)
{
	pThis->lane = lane;
}


#endif /* CAR_H_ */
