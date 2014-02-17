#ifndef LCD_H__
#define LCD_H__

#include "hardware.h"

#define LCD_CS_H P2OUT |= LCD_CS
#define LCD_CS_L P2OUT &= ~LCD_CS

#define X 96
#define Y 96 // display size

extern char _buffer[96*14];

void lcd_xmit(void);

#endif
