
#ifndef _GLCD_H_
#define _GLCD_H_

#include "lcd.h"

static volatile unsigned short Color[2] = {WHITE, BLACK};
void GLCD_DisplayString (unsigned int ln, unsigned int col, unsigned char fi, unsigned char *s);
void GLCD_DrawRect (unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int m);
void GLCD_DrawCharW (unsigned int x, unsigned int y, unsigned int cw, unsigned int ch, unsigned char *c);
void GLCD_DrawCharB (unsigned int x, unsigned int y, unsigned int cw, unsigned int ch, unsigned char *c);

void GLCD_Clear (unsigned short color);
void GLCD_SetTextColor (unsigned short color);
void GLCD_SetBackColor (unsigned short color);
#endif