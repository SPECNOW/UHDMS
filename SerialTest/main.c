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
