#include <string.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include "delay.h"
#include "lcd.h"
#include "touch.h"

#include "font.h"

int main(void)
{
    SystemInit();
    delay_init(24);

    LCD_Init();
    TP_Init();
    LCD_BL_SET;
    TouchPanel_Calibrate();

    LCD_Clear(ORANGE);

    LCD_DrawRectangle(20,20,200,200);

    LCD_DrawString(50,100,WHITE,YELLOW,"STM32F103RCT6",0,&Font8x8);
    while(1)
    {
        if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2))
        {
            getDisplayPoint(&display, Read_Ads7846(), &matrix ) ;
            LCD_DrawString(display.x,display.y,WHITE,YELLOW,"*",0,&Font8x8);
        }
    }
}
