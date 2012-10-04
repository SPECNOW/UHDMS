/*
 * ADC.h
 *
 *  Created on: Sep 18, 2012
 *      Author: Izaak Lee
 */

#ifndef ADC_H_
#define ADC_H_

void InitADC(void)
{
	WDTCTL = WDTPW + WDTHOLD; 						  // Stop WDT

	ADC10CTL0 = ADC10SHT_2 + MSC + ADC10ON + ADC10IE; // Sample Hold Time2, Multiple Sequential Channel, ADC10ON, Interrupt Enabled
	ADC10DTC1 = 0x03; 								  //3 Conversions
  	ADC10AE0 |= 0x07; 								  // P2.0, P2.1, and P2.2 ADC option select (3 ADCs turned on)
  	P1DIR |= 0x03;
}

unsigned int ADC_1()
{
	//1ST ADC
	int ADC1_value;
	ADC10CTL0 &= ~ENC;                      // Disable conversion
	ADC10CTL1 = INCH_0 + CONSEQ_0;          // Select Channel 0, Single Channel single conversion
	ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
	while( ADC10CTL1 & ADC10BUSY );         // wait until conversion is done
	ADC10CTL0 &= ~ENC;
    ADC1_value = ADC10MEM;
	ADC10CTL1 &= ~INCH_0;
	return ADC1_value;
}

unsigned int ADC_2()
{
 	//2ND ADC
	unsigned int ADC2_value;
  	ADC10CTL0 &= ~ENC;                      // Disable conversion
	ADC10CTL1 = INCH_1 + CONSEQ_0;          // Select Channel 1, Single Channel single conversion
	ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
	while( ADC10CTL1 & ADC10BUSY );         // wait until conversion is done
	ADC10CTL0 &= ~ENC;
    ADC2_value = ADC10MEM;
  	ADC10CTL1 &= ~INCH_1;
  	return ADC2_value;
}

unsigned int ADC_3()
{
	//3RD ADC
	unsigned int  ADC3_value;
	ADC10CTL0 &= ~ENC;                      // Disable conversion
	ADC10CTL1 = INCH_2 + CONSEQ_0;          // Select Channel 2, Single Channel single conversion
	ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
	while( ADC10CTL1 & ADC10BUSY );         // wait until conversion is done
    ADC3_value = ADC10MEM;
  	ADC10CTL1 &= ~INCH_2;
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
#endif /* ADC_H_ */
