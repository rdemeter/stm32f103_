#include "glcd.h"

void GLCD_DisplayString (unsigned int ln, unsigned int col, unsigned char fi, unsigned char *s)
{
  LCD_DrawString(ln,col,BLACK,YELLOW,(char*)s,0,&Font12x12);
}

/*******************************************************************************
* Draw rectangle on given position                                             *
*   Parameter:      x:        horizontal position                              *
*                   y:        vertical position                                *
*                   w:        width  					                       *
*                   h:        height										   *
*		    s:        OFF/ON (OFF = 0)                                 *
*   Return:                                                                    *
*******************************************************************************/

void GLCD_DrawRect (unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int m)
{
  if(m) POINT_COLOR=WHITE;
  else POINT_COLOR=RED;
  LCD_DrawRectangle(x,y,x+w,y+h);
}


/*******************************************************************************
* Draw character on given position                                             *
*   Parameter:      x:        horizontal position                              *
*                   y:        vertical position                                *
*                   cw:       character width in pixel                         *
*                   ch:       character height in pixels                       *
*                   c:        pointer to character bitmap                      *
*   Return:                                                                    *
*******************************************************************************/

void GLCD_DrawCharW (unsigned int x, unsigned int y, unsigned int cw, unsigned int ch, unsigned char *c)
{
  LCD_Putchar( x, y,RED, WHITE, *c, 0,&Font8x8);
}

void GLCD_DrawCharB (unsigned int x, unsigned int y, unsigned int cw, unsigned int ch, unsigned char *c)
{
  LCD_Putchar( x, y,WHITE, RED, *c, 0,&Font8x8);
}
/*******************************************************************************
* Clear display                                                                *
*   Parameter:      color:    display clearing color                           *
*   Return:                                                                    *
*******************************************************************************/

void GLCD_Clear (unsigned short color) {
  LCD_Clear(color);
}


/*******************************************************************************
* Set foreground color                                                         *
*   Parameter:      color:    foreground color                                 *
*   Return:                                                                    *
*******************************************************************************/

void GLCD_SetTextColor (unsigned short color) {

  //YELLOW
  //Color[TXT_COLOR] = color;
}

/*******************************************************************************
* Set background color                                                         *
*   Parameter:      color:    background color                                 *
*   Return:                                                                    *
*******************************************************************************/

void GLCD_SetBackColor (unsigned short color) {

  //Color[BG_COLOR] = color;
}
