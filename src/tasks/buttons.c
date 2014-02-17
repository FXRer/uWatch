/*
 * Copyright (c) 2014 Greg Davill
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted (subject to the limitations in the
 * disclaimer below) provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
 * GRANTED BY THIS LICENSE.  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
 * HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Author: Greg Davill <greg.davill@gmail.com>
 */
 
#include <msp430.h>
#include "../drivers/hardware.h"
#include "buttons.h"
 
Evt_t buttonEvent;

char buttonMask;
int buttonHoldDuration;
 
void button_task(void)
{
	task_open();
	// init the Interrupt
	P3IES = 0xF8;
	P3OUT = 0xF8;
	P3REN = 0xF8;
	P3IFG = 0;
	P3IE = 0xF8;
	
	while(1)
	{
		event_wait(buttonEvent); // wait till button pressed
		buttonHoldDuration = 0;
		while(buttonMask != 0xF8)
		{
			// count the number of ticks that have elapsed.
			task_wait(10);
			buttonHoldDuration++;
			
			buttonMask = (P3IN & 0xF8);
		}
		//buttonMask = 0;
	}
	task_close();	 	
}

char button_get_hold_duration(void)
{
	return buttonHoldDuration;
}

char button_get_pressed(void)
{
	return buttonMask ^ 0xF8;
}

// hardware interrup support
__attribute__((interrupt(PORT3_VECTOR)))
void PORT3_vect(void)
{
	buttonMask = P3IN;
	P3IFG = 0;
	// signal an event
	event_ISR_signal(buttonEvent);
	
	// should we wake up? probably...
	
}

