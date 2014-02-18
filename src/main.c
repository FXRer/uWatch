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
 
 #include "cocoOS/cocoos.h" // include cocoOS
 #include "drivers/clock.h"
 #include "drivers/lcd.h"
 #include "drivers/hardware.h"
 #include "tasks/tasks.h"
 #include "xprint.h"
 
 
 //#include "tasks/buttons.h"
 //#include "tasks/buzzer.h"

 static void simple_task(void)
 {
	 task_open();
	 while(1)
	 {
	 	PJOUT ^= BIT0;
	 	event_wait(buttonEvent);
	 	event_signal(buzzerEvent);
	 }
	 task_close();
 }
 

 
 int main(void)
 {	
 	// init the hardware
 	WDTCTL = WDTPW + WDTHOLD;
 	PJDIR |= BIT0 | BIT1;
 	
 	
 		// XT1 Setup   
	// Configure XT1
 PJSEL0 |= BIT4+BIT5;
//PJSEL0 = 0;

CSCTL0 = 0xA500;
  CSCTL1 = DCOFSEL0 + DCOFSEL1;             // Set max. DCO setting
  CSCTL2 = SELA_0 + SELS_3 + SELM_3;        // set ACLK = MCLK = DCO
  CSCTL3 = DIVA_0 + DIVS_0 + DIVM_0;        // set all dividers 

P2DIR = BIT7 | LCD_EXTC | LCD_CLK;
P1DIR = LCD_DISP | LCD_DAT;


	UCB0CTLW0 = UCSWRST;
	UCB0CTLW0 = UCSYNC + UCCKPH + UCMSB + UCMST + UCSSEL_2 + UCSWRST;
	//UCA0CTL1 = UCSWRST + UCSSEL_2;
	UCB0BRW = 8<<2; // 8Mhz SCLK, 1MHz SCLK for LCD as specified in datasheet. 
	//Tests have confirmed this runs at 2MHz, but we don't need that speed.


	P2SEL1 |= LCD_CLK;// + MISO + MOSI + BIT1;
	P1SEL1 |= LCD_DAT;

	P2SEL0 = 0;
	P1SEL0 = 0;//BIT1 | BUZZER;

	UCB0CTLW0 &= ~UCSWRST;

	P1OUT |= LCD_DISP;

	clearBuff();

	out_dev(&putc);

	setxy(2,2);
	xprint("Hello World");
	//putc('e');
	

 	os_init();
 	
 	// init events
 	buttonEvent = event_create();
 	buzzerEvent = event_create();
 	
 	
 	// init tasks
 task_create( home_task, 100, 0, 0, 0);
 	task_create( simple_task, 20 , 0, 0, 0);
 //	task_create( simple_task_A, 100 , 0, 0, 0);
 //	task_create( manager_task, 200, 0 , 0 ,0 );
 //	task_create( analog_task, 101,0,0,0);
 	task_create( buzzer_task, 5, 0, 0, 0);
 	task_create( button_task, 2, 0, 0, 0);
 	
 	// start timer
 	clock_init();
 	
 	// start cocoOS
 	os_start();
 	
 	
 	return 0;
 }
 
 

