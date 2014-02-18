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
#include "analog.h"
#include "../drivers/lcd.h"
#include "../xprint.h"


// x = [0:60], y = sin(x*2pi/60) * 40;
const signed char sine_table[] = {
0x28,0x27,0x27,0x26,0x24,0x22,0x20,0x1D,
0x1A,0x17,0x14,0x10,0x0C,0x08,0x04,0x00,
0xFC,0xF8,0xF4,0xF0,0xED,0xE9,0xE6,0xE3,
0xE0,0xDE,0xDC,0xDA,0xD9,0xD9,0xD9,0xD9,
0xD9,0xDA,0xDC,0xDE,0xE0,0xE3,0xE6,0xE9,
0xEC,0xF0,0xF4,0xF8,0xFC,0x00,0x04,0x08,
0x0C,0x10,0x13,0x17,0x1A,0x1D,0x20,0x22,
0x24,0x26,0x27,0x27};

static int hour,minute;

void analog_task(void)
{

	task_open();
	//c = 0;
	while(1)
	{
		clearBuff();
		setxy(1,4);
		xprint(" Analog Watch\n"); 
		xprint("   %02u:%02u",hour,minute); 
		
		plotCircle(48,48,44);
		
		
		plotLine(48, 48, 48-sine_table[minute], 48 + sine_table[(15+minute)%60]);
		
		
		plotLine(48, 48, 48-(sine_table[hour*5+ minute/12]>>1), 48 + (sine_table[(15+hour*5 + minute/12)%60]>>1));
		
		
		lcd_xmit();
		//PJOUT ^= BIT0;
		
		
		task_wait(100);
		
		
		if(++minute > 59)
		{
			minute = 0;
			if(++hour > 12)
			{
				hour  = 0; 
			}
		}
		
			//P2OUT ^= BIT3;
	}
	
	task_close();

}

