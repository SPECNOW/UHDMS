#include "msp430f2274.h"
#include <string.h>

__interrupt void USCI0RX_ISR(void);

char rx = 0;
char tx = 0;

int main(){
	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
	BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
	DCOCTL = CALDCO_1MHZ;
	P3SEL |= 0x30;                            // P3.4,5 = USCI_A0 TXD/RXD
	UCA0CTL1 = UCSSEL_2;                      // SMCLK
	UCA0BR0 = 104;                           // 9600 from 8Mhz
	UCA0BR1 = 0;
	UCA0MCTL = UCBRS_0;
	UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
	IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
	__enable_interrupt();
	while(1)
	{
		if(tx)
		{
		    UCA0TXBUF = tx;
		    while (!(IFG2 & UCA0TXIFG));              // USCI_A0 TX buffer ready?
		    tx = 0;
		}
	}

}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
  rx = UCA0RXBUF;
  tx = rx;
}

