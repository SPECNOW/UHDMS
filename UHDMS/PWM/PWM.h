/*
 * PWM.h
 *
 *  Created on: Sep 18, 2012
 *      Author: Charanpreet Parmar
 */

#ifndef PWM_H_
#define PWM_H_

#include "..\Car\Car.h"
extern Car car;

void InitPWM();
__interrupt void TB1_ISR(void);

#endif /* PWM_H_ */
