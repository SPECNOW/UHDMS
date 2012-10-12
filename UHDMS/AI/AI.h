/*
 * AI.h
 *
 *  Created on: Oct 3, 2012
 *      Author: Charanpreet Parmar and Izaak Lee
 */

#ifndef AI_H_
#define AI_H_

#include "..\ADC\ADC.h"
#include "..\Car\Car.h"
#include "..\PWM\PWM.h"

extern bool F_;
extern bool R_;
extern bool L_;

extern unsigned int F_val;
extern unsigned int R_val;
extern unsigned int L_val;

extern unsigned int forward, left, right;
extern unsigned int left_lower, right_lower;

extern unsigned int acc_time;
extern unsigned int hold_time;
extern unsigned int hold_time_th;

extern CarState NS_temp;
extern CarDirection ND_temp;

extern CarState NS_;
extern CarDirection ND_;

extern CarState PS_;
extern CarDirection PD_;

extern unsigned int max_speed;

CarState NextState();
CarDirection NextDeci(CarState NS);
void StateMachine();

#endif /* AI_H_ */
