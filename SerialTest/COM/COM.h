/*
 * COM.h
 *
 *  Created on: Oct 10, 2012
 *      Author: Charanpreet Parmar
 */

#ifndef COM_H_
#define COM_H_

#include "msp430f2274.h"
#include <stdbool.h>

static char rx;
static volatile bool RXFLAG;

void InitCOM();
void printf(char* tx, int length);
//__interrupt void USCI0RX_ISR(void);

#endif /* COM_H_ */
