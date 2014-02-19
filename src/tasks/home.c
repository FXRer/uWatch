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
#include "home.h"
#include "buttons.h"
#include "tasks.h"
#include "../drivers/lcd.h"
#include "../xprint.h"

struct menu {
	char *image;
	char name[10];
	taskproctype task;
	
};


#define BUTTON_UP 0x80
#define BUTTON_DOWN 0x20
#define BUTTON_SELECT 0x40
#define BUTTON_MENU 0x08

struct menu mainMenu[3] = {
{0,"Analog",analog_task},
{0,"Taskman",manager_task},
{0,"Analog_A",analog_task},

};

#define N_MENU (sizeof(mainMenu)/sizeof(struct menu))

int current_menu_item, current_tid;


void home_task(void)
{

	task_open();
	current_menu_item = 1;
	current_tid = 0;
	while(1)
	{
		
		if( current_tid  == 0)
		{
		clearBuff();
		
		
		setxy(2,4);
		xprint("%s\n0x%04X",mainMenu[current_menu_item].name,mainMenu[current_menu_item].task ); 
		
		lcd_xmit();
		
		}
		event_wait( buttonEvent );
		
		char button_state = button_get_pressed();
		if(( button_state & BUTTON_DOWN )&&( current_menu_item > 0 ))
		{
			current_menu_item--; 
			// antmation?
		}
		else if(( button_state & BUTTON_UP )&&( current_menu_item < N_MENU -1 ))
		{
			current_menu_item++;
		}
		else if(( button_state & BUTTON_SELECT )&&( current_tid == 0 )) // no task running
		{
			// call up a new task
			
			task_create( mainMenu[current_menu_item].task, 10, 0, 0, 0 ); // should be a lower priority than this task
			
			// store tid
			current_tid = 1;//task_id_get( mainMenu[current_menu_item].task );
			
		}
		else if(( button_state & BUTTON_MENU )&&( current_tid != 0 ))
		{
			task_kill( mainMenu[current_menu_item].task );
			current_tid = 0;
		}
		task_wait(10);
			//P2OUT ^= BIT3;
	}
	
	task_close();

}

