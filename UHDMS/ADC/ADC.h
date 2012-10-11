/*
 * ADC.h
 *
 *  Created on: Sep 18, 2012
 *      Author: Izaak Lee
 */

#ifndef ADC_H_
#define ADC_H_

#include "msp430f2274.h"

#define numsample 5

void InitADC(void);
unsigned int ADC_1();
unsigned int ADC_2();
unsigned int ADC_3();
unsigned int isClose(unsigned int ADC, unsigned int thresh);

#endif /* ADC_H_ */
