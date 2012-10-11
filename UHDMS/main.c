/*
 * main.c
 */

#include "UHDMS.h"

void main(void)
{
	Init();
	while(1)
	{
		StateMachine();
	}
}
