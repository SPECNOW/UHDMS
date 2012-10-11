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

static bool F_;
static bool R_;
static bool L_;

static unsigned int F_val = 0;
static unsigned int R_val = 0;
static unsigned int L_val = 0;

static unsigned int forward = 0, left = 0, right = 0;
static unsigned int left_lower = 0, right_lower =0;

static unsigned int acc_time = 0;
static unsigned int hold_time = 0;
static unsigned int hold_time_th = 0;

static CarState NS_temp = _000;
static CarDirection ND_temp = STOP;

static CarState NS_ = _000;
static CarDirection ND_ = STOP;

static CarState PS_ = _000;
static CarDirection PD_ = STOP;


CarState NextState();
CarDirection NextDeci(CarState NS);
void StateMachine();

#endif /* AI_H_ */
