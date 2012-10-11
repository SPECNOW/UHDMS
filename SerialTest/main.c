#include "msp430f2274.h"
#include <string.h>

#include "COM\COM.h"

int main(){
	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
	BCSCTL1 = CALBC1_8MHZ;                    // Set DCO
	DCOCTL = CALDCO_8MHZ;

	InitCOM();
	__enable_interrupt();
	while(1)
	{
		if(RXFLAG)
		{
			printf("HELLO ", 6);
			printf(&rx, 1);
			RXFLAG = false;
		}
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
