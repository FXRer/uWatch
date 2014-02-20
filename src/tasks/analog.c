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
signed char sine_table[] = {
0x1E,0x1D,0x1D,0x1C,0x1B,0x19,0x18,0x16,
0x14,0x11,0x0F,0x0C,0x09,0x06,0x03,0x00,
0xFD,0xFA,0xF7,0xF4,0xF2,0xEF,0xEC,0xEA,
0xE8,0xE7,0xE5,0xE4,0xE3,0xE3,0xE3,0xE3,
0xE3,0xE4,0xE5,0xE7,0xE8,0xEA,0xEC,0xEF,
0xF1,0xF4,0xF7,0xFA,0xFD,0x00,0x03,0x06,
0x09,0x0C,0x0E,0x11,0x14,0x16,0x18,0x19,
0x1B,0x1C,0x1D,0x1D,};

signed char tiny_sine[] = {
0x05,0x04,0x04,0x04,0x04,0x04,0x04,0x03,
0x03,0x02,0x02,0x02,0x01,0x01,0x00,0x00,
0x00,0xFF,0xFF,0xFE,0xFE,0xFE,0xFD,0xFD,
0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,
0xFC,0xFC,0xFC,0xFC,0xFC,0xFD,0xFD,0xFE,
0xFE,0xFE,0xFF,0xFF,0x00,0x00,0x00,0x01,
0x01,0x02,0x02,0x02,0x03,0x03,0x04,0x04,
0x04,0x04,0x04,0x04,};


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
		xprint("   %02u:%02u",hour+1,minute); 
		
		plotCircle(48,48,44);
		
		
		
		char x = 48 + tiny_sine[(minute+15)%60];
		char y = 48 + tiny_sine[(minute)%60];
		
		plotLine(x, y , 48-sine_table[minute], 48 + sine_table[(15+minute)%60]);
		
		char x1 = 48 - tiny_sine[(minute+15)%60];
		char y1 = 48 - tiny_sine[(minute)%60];
		
		plotLine(x,y,x1,y1);
		
		plotLine(x1, y1 , 48-sine_table[minute], 48 + sine_table[(15+minute)%60]);
		
		
		
		
		
		
		int h = 5*hour + minute/12;
		
		x = 48 + tiny_sine[(h+15)%60];
		y = 48 + tiny_sine[(h)%60];
		
		
		plotLine(x, y, 48-sine_table[h]*2/3, 48 + sine_table[(15+h)%60]*2/3);
		
		
		
		x1 = 48 - tiny_sine[(h+15)%60];
		y1 = 48 - tiny_sine[(h)%60];
		
		plotLine(x,y,x1,y1);
		
		
		plotLine(x1, y1, 48-((sine_table[h]*2)/3), 48 + ((sine_table[(15+h)%60]*2)/3));
		
		
		//plotLine(48, 48, 48-(sine_table[h]>>1), 48 + (sine_table[(15+hour*5 + minute/12)%60]>>1));
		
		
		lcd_xmit();
		//PJOUT ^= BIT0;
		
		
		task_wait(4);
		
		
		if(++minute > 59)
		{
			minute = 0;
			if(++hour > 11)
			{
				hour  = 0; 
			}
		}
		
			//P2OUT ^= BIT3;
	}
	
	task_close();

}

