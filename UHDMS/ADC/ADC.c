/*
 * ADC.c
 *
 *  Created on: Oct 10, 2012
 *      Author: Izaak Lee
 */

#include "ADC.h"

void InitADC(void)
{
	WDTCTL = WDTPW + WDTHOLD; 						  // Stop WDT

	ADC10CTL0 = ADC10SHT_2 + MSC + ADC10ON + ADC10IE; // Sample Hold Time2, Multiple Sequential Channel, ADC10ON, Interrupt Enabled
	ADC10DTC1 = 0x03; 								  //3 Conversions
  	ADC10AE0 |= 0x07; 								  // P2.0, P2.1, and P2.2 ADC option select (3 ADCs turned on)
  	//P1DIR |= 0x03;
}

unsigned int ADC_1()
{
	//1ST ADC
	int i = 0;
	unsigned int ADC1_[numsample] = {0};
	unsigned int ADC1_value = 0;
	for (i = 0; i != numsample; i++)
	{
		ADC10CTL0 &= ~ENC;                      // Disable conversion
		ADC10CTL1 = INCH_0 + CONSEQ_0;          // Select Channel 0, Single Channel single conversion
		ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
		while( ADC10CTL1 & ADC10BUSY );         // wait until conversion is done
		ADC10CTL0 &= ~ENC;
		ADC1_[i] = ADC10MEM;
		ADC10CTL1 &= ~INCH_0;

	}
	for (i = 0; i != numsample; i++)
	{
		ADC1_value += ADC1_[i];
	}
	ADC1_value = ADC1_value/numsample;
	return ADC1_value;
}

unsigned int ADC_2()
{
 	//2ND ADC
	int i = 0;
	unsigned int ADC2_[numsample] = {0};
	unsigned int ADC2_value = 0;
	for (i = 0; i != numsample; i++)
	{
		ADC10CTL0 &= ~ENC;                      // Disable conversion
		ADC10CTL1 = INCH_1 + CONSEQ_0;          // Select Channel 1, Single Channel single conversion
		ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
		while( ADC10CTL1 & ADC10BUSY );         // wait until conversion is done
		ADC10CTL0 &= ~ENC;
		ADC2_[i] = ADC10MEM;
		ADC10CTL1 &= ~INCH_1;
	}
	for (i = 0; i != numsample; i++)
	{
		ADC2_value += ADC2_[i];
	}
	ADC2_value = ADC2_value/numsample;
  	return ADC2_value;
}

unsigned int ADC_3()
{
	//3RD ADC
	int i = 0;
	unsigned int ADC3_[numsample] = {0};
	unsigned int  ADC3_value = 0;
	for (i = 0; i != numsample; i++)
	{
		ADC10CTL0 &= ~ENC;                      // Disable conversion
		ADC10CTL1 = INCH_2 + CONSEQ_0;          // Select Channel 2, Single Channel single conversion
		ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
		while( ADC10CTL1 & ADC10BUSY );         // wait until conversion is done
		ADC3_[i] = ADC10MEM;
		ADC10CTL1 &= ~INCH_2;
	}
	for (i = 0; i != numsample; i++)
	{
		ADC3_value += ADC3_[i];
	}
	ADC3_value = ADC3_value/numsample;
  	return ADC3_value;
}
unsigned int isClose(unsigned int ADC, unsigned int thresh)
{
	if(ADC < thresh)
	{
		return 0;
	}
	return 1;
}
