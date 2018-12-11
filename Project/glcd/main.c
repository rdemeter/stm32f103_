#include <string.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <delay.h>
#include "lcd.h"
#include "touch.h"
#include "bounce_utils.h" 

#include "font.h"

int main(void)
{
    SystemInit();
    delay_init(24);

    LCD_Init();
    TP_Init();
    LCD_BL_SET;
    TouchPanel_Calibrate();

    /*
    LCD_DrawRectangle(50,50,100,100);
    LCD_DrawRectangle(30,30,120,120);
    LCD_DrawRectangle(10,10,140,140);
    LCD_Draw_Circle(190,70,15);
    LCD_Draw_Circle(190,70,30);
    LCD_Draw_Circle(190,70,45);
    LCD_DrawPoint(190,70);
    LCD_DrawPoint(191,70);
    LCD_DrawPoint(189,70);
    LCD_DrawTriangle(50,155,20,175,80,175);
    LCD_DrawTriangle(100,155,100,175,150,175);
    LCD_DrawTriangle(180,155,170,175,220,175);
    
    LCD_DrawLine(20, 200, 220, 200);
    LCD_DrawString(20,220,RED,YELLOW,"Functiile LCD-ului ILI9328",0,&Font8x8);
    LCD_DrawString(20,230,RED,YELLOW,"- punct;",0,&Font8x8);
    LCD_DrawString(20,240,RED,YELLOW,"- linie;",0,&Font8x8);
    LCD_DrawString(20,250,RED,YELLOW,"- cerc;",0,&Font8x8);
    LCD_DrawString(20,260,RED,YELLOW,"- dreptunghi;",0,&Font8x8);
    LCD_DrawString(20,270,RED,YELLOW,"- triunghi;",0,&Font8x8);
    LCD_DrawString(20,280,RED,YELLOW,"- caracter;",0,&Font8x8);
    LCD_DrawString(20,290,RED,YELLOW,"- string.",0,&Font8x8);
    
    LCD_Putchar(20,182, RED,YELLOW, 'C',0,&Font12x12);
    
    LCD_Fill(0,10,100,45,BROWN);
    LCD_Fill(0,18,100,50,BRED);
    LCD_Fill(0,30,100,70,GRED);
    LCD_Fill(0,45,100,80,GBLUE);
    LCD_Fill(0,60,100,95,RED);
    LCD_Fill(0,75,100,110,MAGENTA);
    LCD_Fill(0,90,100,125,GREEN);
    LCD_Fill(0,105,100,140,CYAN);
    LCD_Fill(0,120,100,155,YELLOW);
    LCD_Fill(0,135,100,165,BLUE);
    LCD_Fill(0,154,100,195,BRRED);
    LCD_Fill(0,170,100,205,GRAY);
    LCD_Fill(0,185,100,220,DARKBLUE);
    LCD_Fill(0,200,100,235,LIGHTBLUE);
    LCD_Fill(0,215,100,250,GRAYBLUE);
    LCD_Fill(0,230,100,265,ORANGE);
    LCD_Fill(0,245,100,280,OLIVE);
    LCD_Fill(0,260,100,295,MAROON);
    LCD_Fill(0,275,100,310,LIGHTGREEN);
    LCD_Fill(0,290,100,320,LIGHTGRAY);
    */
   
    game_Init();
    while(1)
    {  
      update_ball();

      int mode = getGameInfo();
      if(mode == SGL)
      {
        update_player1();
      }
      else if(mode == DUAL)
      {
        update_player1();
        update_player2();
      }
      else if(mode == AI)
      {
retry:
        if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2))
        {
           getDisplayPoint(&display, Read_Ads7846(), &matrix ) ;
           if(display.y > HEIGHT || display.x > WIDTH) goto retry;
           if(display.x < WIDTH / 2)
                 update_player1();
           else
             update_player2();
         }
      }
        
      check_collision();
        
      if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
        game_Init();

       /*if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2))
        {
          
            getDisplayPoint(&display, Read_Ads7846(), &matrix ) ;
            LCD_DrawString(display.x,display.y,BLACK,YELLOW,"*",0,&Font8x8);
        } */         
    }
}
