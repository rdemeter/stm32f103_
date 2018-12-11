#include <string.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include "delay.h"
#include "lcd.h"
#include "font.h"

int main(void)
{
    SystemInit();
    delay_init(24);

    LCD_Init();

    //LCD_BL_SET;
    LCD_Clear(ORANGE); // BLACK

    //LCD_Rect(0,0,320,240,RGB565(255,255,255));
    //LCD_Fill(0,0,50,100,ORANGE);
    //LCD_DrawFillRectangle(0,10,100,10);
    //LCD_DrawPoint(12,10);
    //LCD_DrawPoint(13,10);
    //LCD_DrawRectangle(20,20,200,200);

    //delay_ms(1000);
    //LCD_DrawRectangle(40,20,220,220);
    //LCD_DrawString(50,100,WHITE,YELLOW,"STM32F103RCT6",0,&Font8x8);
    while(1)
    {
        
    }
}
