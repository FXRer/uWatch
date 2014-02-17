#include <msp430.h>
#include "../cocoOS/cocoos.h"


void clock_init(void)
{
	// start up a hardware clock here
	TA0CCTL0 = CCIE;
	TA0CCR0 = 10000;
	TA0CTL = TASSEL_2 + MC_2 + ID_2;

	// debug
	PJDIR |= BIT1;

}



// interrupt here, call to os_tick(); to pass clock to cocoOS
__attribute__((interrupt(TIMER0_A0_VECTOR)))
void TIMER0_A0_vect()
{
	//PJOUT ^= BIT1;
	TA0CCR0 += 10000;
	os_tick();
}

