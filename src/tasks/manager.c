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


#include "manager.h"
#include "../xprint.h"

// desc, a simple task manager. is infact a task in itself. 

//extern tcb task_list[ N_TASKS ];
char* tasks;


void manager_task(void)
{
	int i,j;
	static int selected;
	task_open();
	selected = 0;
	clearBuff();
	while(1)
	{
		// get list of running tasks
		tasks =  task_get_list( 0 );
		
		// small preamble
		setxy(0,0);
		xprint(" id Prio time\n");
		
		
		char *t = &tasks[0]; // do something with this information
		
		
		// display the list
		for(i = 0; i < 7; i++)
		{
			
			t = &tasks[20*i];
			
			
		//	for(j = 0; j < 8; j++)
			//{
				xprint("%c%02X  %03d %04X %04X",selected == i ? '>' : '~', *t & 0xFF, t[1], t[14], t[18]);
				t++;
			//}  
			putc('\n');
			//xprint("\n");
			//xprint("%c%u %u %04X\n",selected == i ? '>' : '-',t[0],0,0);
			
		}
		
		
		// display this result.
		lcd_xmit();
		
		
		
		// take input of some kind
		event_wait_timeout( buttonEvent , 10 );
		
		switch( button_get_pressed() )
		{
			case 0x20 :
				selected++;
			break;
		}
		//task_wait(10);
		//selected++;
	}
	
	task_close();
}
