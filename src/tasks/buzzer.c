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
 

#include "../drivers/hardware.h"
#include "buzzer.h"
 
Evt_t buzzerEvent;
 
// task desc: sit and wait for buzzer request.. 
// when req recv. timeout and stop buzzer 
void buzzer_task(void)
{
	task_open();
	// some initial init
	P2SEL0 |= BUZZER; //assign TB2.0 to BUZZER
	P2DIR |= BUZZER;
	
	while(1){
	// sit and wait
	event_wait(buzzerEvent);
	
	// configure timer B2
	TB2CCR0 = 350;
	TB2CTL = TASSEL_2 + MC_3 + TACLR;
	
	TB2CCTL0 = OUTMOD_4; 
	
	// timeout?
	task_wait(1);
	
	
	// clean up, disabling timer will save power
	// ensure NPN is off to reduce current through buzzer.
	TB2CCTL0 = 0; 	// this will clear OUT, setting BUZZER LOW
	TB2CTL = 0; 	// disable timer
	
	
		
	}
	// never ends
	task_close();	 	
}


