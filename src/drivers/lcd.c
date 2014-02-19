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
#include "lcd.h"




const unsigned int font[] = {
//__attribute__((section(".infomem"))) = { 
0x03A0,0x6018,0x7D5F,0x2BEA,0x688B,0x5EAB,0x0300,0x022E,
0x3A20,0x288A,0x11C4,0x0060,0x1084,0x0020,0x6083,0x7E3F,
0x03E0,0x76B7,0x7EB1,0x7C9C,0x5EBD,0x5EBF,0x7E10,0x7EBF,
0x7EBD,0x0140,0x0160,0x4544,0x294A,0x1151,0x62B0,0x3AAE,
0x3E8F,0x2ABF,0x462E,0x3A3F,0x46BF,0x429F,0x5E2E,0x7C9F,
0x47F1,0x7E33,0x6C9F,0x043F,0x7D9F,0x7E0F,0x7E3F,0x729F,
0x7E5E,0x6E9F,0x5EBD,0x43F0,0x7C3F,0x783E,0x7CDF,0x6C9B,
0x70FC,0x66B3,0x023F,0x0C98,0x7E20,0x2208,0x0421,0x2200,
0x3EA2,0x08BF,0x2526,0x7CA2,0x36AE,0x51E0,0x7EA8,0x1C9F,
0x02E0,0x02E1,0x144F,0x01E0,0x3C8F,0x1D07,0x1926,0x114F,
0x3D44,0x208F,0x29E5,0x15E4,0x3C2F,0x382E,0x3C4F,0x24C9,
0x3CAC,0x3569,0x02AA,0x03E0,0x2AA0,0x3108,

}; 



void lcd_xmit(void)
{

	__bic_SR_register(GIE); // disable interrupts

	LCD_CS_H;
	
	DMA0CTL = 0;
	DMACTL0 = DMA0TSEL__UCB0TXIFG0;
	UCB0IFG = 0;
	__asm__("mov.w	%1,%0": "=m" (DMA0SA) :"i" (_buffer));
	__asm__("mov.w	%1,%0": "=m" (DMA0DA) :"i" (&UCB0TXBUF));
	
	DMA0SZ = (96*14)+1;
	DMA0CTL = DMADT_4|DMASRCINCR_3|DMADSTBYTE|DMASRCBYTE|DMAEN;
	
	UCB0TXBUF = 0x80; // start DMA xfer
	
	while((DMAIFG & DMA0CTL) == 0);
	
	
	DMACTL0 = 0;
	
	LCD_CS_L;
	
	__bis_SR_register(GIE); // We're done, enable interrupts


}

void clearBuff()
{
	char *ptr = (char*)_buffer;

	int m,n;

	for(m = 0 ; m < 96; m++)
	{
		ptr++;
		for(n = 0; n < 13; n++)
			*ptr++ = 0xFF;
		//ptr++;
	}	
	
}

void clear(int _x, int _y, int _w, int _h)
{
	unsigned char *buf = (unsigned char*)_buffer + 20*168;// END_OF_SCREEN_BUFFER;
	
	int m,n;

	buf -= 14*_x;
	buf -= 13-_y;

	for(m = _w ; m > 0; m--)
	{
		char *ptr = buf;
		for(n = _h; n > 0; n--)
			*ptr++ = 0xFF;
		buf -= 14;
	}	
}

void plotCircle(int xm, int ym, int r)
{
   int x = -r, y = 0, err = 2-2*r;                /* bottom left to top right */
   do {                                          
      setPixel(xm-x, ym+y, 1);                            /*   I. Quadrant +x +y */
      setPixel(xm-y, ym-x, 1);                            /*  II. Quadrant -x +y */
      setPixel(xm+x, ym-y, 1);                            /* III. Quadrant -x -y */
      setPixel(xm+y, ym+x, 1);                            /*  IV. Quadrant +x -y */
      r = err;                                   
      if (r <= y) err += ++y*2+1;                             /* e_xy+e_y < 0 */
      if (r > x || err > y)                  /* e_xy+e_x > 0 or no 2nd y-step */
         err += ++x*2+1;                                     /* -> x-step now */
   } while (x < 0);
}


int x,y;

// setxy, sets x and y values used to print character to the display
void setxy(int _x, int _y)
{
	x = _x;
	y = _y;
}


void plotLine(int x0, int y0, int x1, int y1)
{
   int dx =  abs(x1-x0), sx = x0<x1 ? 1 : -1;
   int dy = -abs(y1-y0), sy = y0<y1 ? 1 : -1;
   int err = dx+dy, e2;                                   /* error value e_xy */
                                                    
   for (;;){                                                          /* loop */
      setPixel(x0,y0, 1);                              
      e2 = 2*err;                                   
      if (e2 >= dy) {                                         /* e_xy+e_x > 0 */
         if (x0 == x1) break;                       
         err += dy; x0 += sx;                       
      }                                             
      if (e2 <= dx) {                                         /* e_xy+e_y < 0 */
         if (y0 == y1) break;
         err += dx; y0 += sy;
      }
   }
}



void setPixel(int _x, int _y, char b)
{
	if(_y > Y || _x > X || _x < 0 || _y < 0)
		return;

	int xloc = ((_y*(X/8+2)))+_x/8+1;
	int shift = (_x % 8);

	char *ptr = (char*)_buffer;

	if (b)
		ptr[xloc] &= ~(0x80 >> shift);
	else
		ptr[xloc] |= (0x80 >> shift);
}



//TODO: make this function smaller/more efficient
// putc, simple printing function, used as a debug console.
void putc(char d)
{
	int xx,yy;

	if(d == ' ')
	{
		y+=1;
		return;
	}
	else
		if(d == '\n')
		{
			y= 0;
			x+= 1;
			return;
		}

	d = d - '!';


	char *ptr = (char*)_buffer  + (14*96) - (1+X/8);


	ptr += x+1;


	ptr -= (y*((X/2)+8));

	int q = font[d];

	unsigned int mask = 0x001F;
	for(xx = 0 ; xx < 3; xx++)
	{
		*ptr = ~(q & mask);
		q >>= 5;
		ptr -= (X/8) + 2 ;
	}

	y += 1;
}



