/*
 * COM.c
 *
 *  Created on: Oct 10, 2012
 *      Author: Charanpreet Parmar
 */

#include "COM.h"

char rx;
bool RXFLAG;

void InitCOM()
{
	P3SEL |= 0x30;                            // P3.4,5 = USCI_A0 TXD/RXD
	UCA0CTL1 = UCSSEL_2;                      // SMCLK
	UCA0BR0 = 0x41;                           // 9600 from 8Mhz
	UCA0BR1 = 0x03;
	UCA0MCTL = UCBRS_2;
	UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
	IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
	RXFLAG = 1;
}
void printf(char* tx, int length)
{
	unsigned int pointer;
	for(pointer = 0; pointer < length; pointer++)
	{
		UCA0TXBUF = tx[pointer];
		while (!(IFG2 & UCA0TXIFG));
	}
}
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
	//It looks like Global Static Variables like rx and RXFLAG
	//can be used in 1 .c file only. Otherwise multiple rxs are created
	//all with different addresses for some reason...
	rx= UCA0RXBUF;
	RXFLAG = true;
}
