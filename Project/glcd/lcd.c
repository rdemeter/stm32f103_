#include "lcd.h"
#include "stdlib.h"
#include "delay.h"	 

_lcd_dev lcddev;

u16 POINT_COLOR = BLACK;//0xFFFF;
u16 BACK_COLOR = 0xFFFF;


/*****************************************************************/
void LCD_WR_REG(u8 data)
{       
    LCD_CS_SET;
    LCD_RS_SET;
    LCD_RD_SET;
    LCD_WR_SET;

    LCD_CS_CLR;
    LCD_RS_CLR;

    DATAOUT(0x0000);
    LCD_WR_CLR;
    LCD_WR_SET;

    DATAOUT(data << 8);
    LCD_WR_CLR;
    LCD_WR_SET;

    LCD_RS_SET;
}

/*****************************************************************/
void LCD_WR_DATA(u16 data)
{
    LCD_RS_SET;
    LCD_RD_SET;

    DATAOUT(data);
    LCD_WR_CLR;
    LCD_WR_SET;

    DATAOUT(data<<8);
    LCD_WR_CLR;
    LCD_WR_SET;

    LCD_CS_SET;
}

/*****************************************************************/
void LCD_DrawPoint_16Bit(u16 color)
{
    LCD_CS_CLR;
    LCD_RD_SET;
    LCD_RS_SET;	 
    DATAOUT(color);
    LCD_WR_CLR;
    LCD_WR_SET;	
    DATAOUT(color<<8);
    LCD_WR_CLR;
    LCD_WR_SET;	 
    LCD_CS_SET;
}

/*****************************************************************/
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
{
    LCD_WR_REG(LCD_Reg);
    LCD_WR_DATA(LCD_RegValue);
}	   
	 
/*****************************************************************/
void LCD_WriteRAM_Prepare(void)
{
    LCD_WR_REG(lcddev.wramcmd);
}	 

/*****************************************************************/
void LCD_DrawPoint(u16 x,u16 y)
{
    LCD_SetCursor(x,y);

    LCD_CS_CLR;
    LCD_RD_SET;
    LCD_RS_SET;
    DATAOUT(POINT_COLOR);
    LCD_WR_CLR;
    LCD_WR_SET;

    DATAOUT(POINT_COLOR<<8);
    LCD_WR_CLR;
    LCD_WR_SET;	 
    LCD_CS_SET;
}

/*****************************************************************/
void LCD_Clear(u16 Color)
{
    u32 index = 0;      
    LCD_SetWindows(0, 0, lcddev.width-1, lcddev.height-1);	

    LCD_RS_SET;
    LCD_CS_CLR;	   
    for(index=0; index<lcddev.width*lcddev.height; index++)
    {
        DATAOUT(Color);	
        LCD_WR_CLR;
        LCD_WR_SET;	
        
        DATAOUT(Color<<8);	
        LCD_WR_CLR;
        LCD_WR_SET;	   
    }
    LCD_CS_SET;	
} 

/*****************************************************************/
void LCD_GPIOInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE); 
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = 1<<LCD_BL_PIN|1<<LCD_CS_PIN|1<<LCD_RS_PIN|1<<LCD_WR_PIN|1<<LCD_RD_PIN|1<<LCD_RST_PIN;	   //GPIO_Pin_10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_SetBits(GPIOC,1<<LCD_CS_PIN|1<<LCD_RS_PIN|1<<LCD_WR_PIN|1<<LCD_RD_PIN|1<<LCD_RST_PIN);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB,GPIO_Pin_All);	 
}


/*************************************************************/

void LCD_DrawTriangle(u16 x1, u16 y1,u16 x2, u16 y2,u16 x3, u16 y3)
	
{
  LCD_DrawLine(x1, y1, x2, y2);
  LCD_DrawLine(x2, y2, x3, y3);
  LCD_DrawLine(x3, y3, x1, y1); 
}

/***************************************************************/
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             
	while(a<=b)
	{
		LCD_DrawPoint(x0+a,y0-b);             //5
 		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-a,y0+b);             //1       
 		LCD_DrawPoint(x0-b,y0+a);             
		LCD_DrawPoint(x0-a,y0-b);             //2             
  		LCD_DrawPoint(x0-b,y0-a);             //7     	         
		a++;
		    
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
}

/*****************************************************************/
void LCD_RESET(void)
{
    LCD_RST_CLR;
    delay_ms(100);	
    LCD_RST_SET;
    delay_ms(50);
}
 	
u16 LCD_ReadReg(u8 LCD_Reg)
{										   
    u16 t;
        
    LCD_WR_REG(LCD_Reg);
    
    LCD_RD_CLR;					   
    t=DATAIN;
    LCD_RD_SET;
    
    LCD_RD_CLR;					   
    t=t | (DATAIN>>8);
    LCD_RD_SET;
    LCD_CS_SET;   

    return t;  
} 

/*****************************************************************/
void LCD_Init(void)
{      
    LCD_GPIOInit();
    LCD_RESET();
    LCD_WriteReg(LCD_REG_0,  0x0001);
    delay_ms(50);
         
    lcddev.id = LCD_ReadReg(LCD_REG_0);

    //************* Start Initial Sequence **********//		
    LCD_WriteReg(0x00EC,0x108F);// internal timeing      
    LCD_WriteReg(0x00EF,0x1234);// ADD        
    //LCD_WriteReg(0x00e7,0x0010);      
    //LCD_WriteReg(0x0000,0x0001);//??????
    LCD_WriteReg(0x0001,0x0100);     
    LCD_WriteReg(0x0002,0x0700);//????                    
    //LCD_WriteReg(0x0003,(1<<3)|(1<<4) ); 	//65K  RGB
    //DRIVE TABLE(??? 03H)
    //BIT3=AM BIT4:5=ID0:1
    //AM ID0 ID1   FUNCATION
    // 0  0   0	   R->L D->U
    // 1  0   0	   D->U	R->L
    // 0  1   0	   L->R D->U
    // 1  1   0    D->U	L->R
    // 0  0   1	   R->L U->D
    // 1  0   1    U->D	R->L
    // 0  1   1    L->R U->D ??????.
    // 1  1   1	   U->D	L->R
    LCD_WriteReg(0x0003,(1<<12)|(3<<4)|(0<<3) );//65K    
    LCD_WriteReg(0x0004,0x0000);                                   
    LCD_WriteReg(0x0008,0x0202);	           
    LCD_WriteReg(0x0009,0x0000);         
    LCD_WriteReg(0x000a,0x0000);//display setting         
    LCD_WriteReg(0x000c,0x0001);//display setting          
    LCD_WriteReg(0x000d,0x0000);//0f3c          
    LCD_WriteReg(0x000f,0x0000);
            //????
    LCD_WriteReg(0x0010,0x0000);   
    LCD_WriteReg(0x0011,0x0007);
    LCD_WriteReg(0x0012,0x0000);                                                                 
    LCD_WriteReg(0x0013,0x0000);                 
    LCD_WriteReg(0x0007,0x0001);                 
    delay_ms(50); 
    LCD_WriteReg(0x0010,0x1490);   
    LCD_WriteReg(0x0011,0x0227);
    delay_ms(50); 
    LCD_WriteReg(0x0012,0x008A);                  
    delay_ms(50); 
    LCD_WriteReg(0x0013,0x1a00);   
    LCD_WriteReg(0x0029,0x0006);
    LCD_WriteReg(0x002b,0x000d);
    delay_ms(50); 
    LCD_WriteReg(0x0020,0x0000);                                                            
    LCD_WriteReg(0x0021,0x0000);           
    delay_ms(50); 
    //????
    LCD_WriteReg(0x0030,0x0000); 
    LCD_WriteReg(0x0031,0x0604);   
    LCD_WriteReg(0x0032,0x0305);
    LCD_WriteReg(0x0035,0x0000);
    LCD_WriteReg(0x0036,0x0C09); 
    LCD_WriteReg(0x0037,0x0204);
    LCD_WriteReg(0x0038,0x0301);        
    LCD_WriteReg(0x0039,0x0707);     
    LCD_WriteReg(0x003c,0x0000);
    LCD_WriteReg(0x003d,0x0a0a);
    delay_ms(50); 
    LCD_WriteReg(0x0050,0x0000); //??GRAM???? 
    LCD_WriteReg(0x0051,0x00ef); //??GRAM????                    
    LCD_WriteReg(0x0052,0x0000); //??GRAM????                    
    LCD_WriteReg(0x0053,0x013f); //??GRAM????  
 
    LCD_WriteReg(0x0060,0xa700);        
    LCD_WriteReg(0x0061,0x0001); 
    LCD_WriteReg(0x006a,0x0000);
    LCD_WriteReg(0x0080,0x0000);
    LCD_WriteReg(0x0081,0x0000);
    LCD_WriteReg(0x0082,0x0000);
    LCD_WriteReg(0x0083,0x0000);
    LCD_WriteReg(0x0084,0x0000);
    LCD_WriteReg(0x0085,0x0000);
  
    LCD_WriteReg(0x0090,0x0010);     
    LCD_WriteReg(0x0092,0x0600);  
    //??????    
    LCD_WriteReg(0x0007,0x0133);

    delay_ms(100);

    LCD_SetParam();
    //LCD_Display_Dir(0);
    //LCD_LED=1;
    //LCD_Clear(WHITE);
}
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
    u16 t; 
    int xerr=0,yerr=0,delta_x,delta_y,distance; 
    int incx,incy,uRow,uCol; 

    delta_x=x2-x1; 
    delta_y=y2-y1; 
    uRow=x1; 
    uCol=y1; 
    if(delta_x>0)incx=1; 
    else if(delta_x==0)incx=0;
    else {incx=-1;delta_x=-delta_x;} 
    if(delta_y>0)incy=1; 
    else if(delta_y==0)incy=0;
    else{incy=-1;delta_y=-delta_y;} 
    if( delta_x>delta_y)distance=delta_x;
    else distance=delta_y; 
    for(t=0;t<=distance+1;t++ )
    {  
        LCD_DrawPoint(uRow,uCol);
        xerr+=delta_x ; 
        yerr+=delta_y ; 
        if(xerr>distance) 
        { 
            xerr-=distance; 
            uRow+=incx; 
        }
        if(yerr>distance) 
        { 
            yerr-=distance; 
            uCol+=incy; 
        } 
    }  
} 


void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
    LCD_DrawLine(x1,y1,x2,y1);
    LCD_DrawLine(x1,y1,x1,y2);
    LCD_DrawLine(x1,y2,x2,y2);
    LCD_DrawLine(x2,y1,x2,y2);
}  


void LCD_DrawFillRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
    LCD_Fill(x1,y1,x2,y2,POINT_COLOR);
}


void LCD_DrawString(u16 x, u16 y, u16 FontColor, u16 BackColor, char *str, u8 mode, sFONT *font)
{
    while(*str) {
        x = LCD_Putchar(x,y,FontColor,BackColor,*str++,mode,font);
    }
}
void GUI_DrawPoint(u16 x,u16 y,u16 color)
{
    LCD_SetCursor(x,y);
    LCD_DrawPoint_16Bit(color); 
}
uint32_t LCD_Putchar(u16 x,u16 y,u16 FontColor, u16 BackColor, char c, u8 mode, sFONT *font)
{
    uint32_t i, j;
    unsigned short Data;
    sFONT *CurrentFont=font;
    uint32_t offset = (c-32) * CurrentFont->Height;
    uint16_t width = CurrentFont->Width;
         
    for (i = 0; i < CurrentFont->Height; i++)
    {
        Data = CurrentFont->table[offset+i];   
        for (j = 0; j < width; j++) 
        {
            if (CurrentFont!=&Font8x16)
            {
                if((((Data & ((0x80 << ((CurrentFont->Width / 12 ) * 8 ) ) >> j)) == 0x00) &&(CurrentFont->Width <= 12))||
                (((Data & (0x1 << j)) == 0x00)&&(CurrentFont->Width > 12 )))
                {
                    if (mode) GUI_DrawPoint(x + j, y + i,  BackColor);
                }
                else
                {
                    GUI_DrawPoint(x + j,y + i, FontColor);
                }
            }
            else
            {
                if ((Data >> j) & 0x01) 
                {
                    GUI_DrawPoint(x + j,y + i, FontColor);
                }
                else
                {
                    
                    if (mode) GUI_DrawPoint(x + j, y + i,  BackColor);
                }
            }
        }
    }
         
    return x+width;
}

void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{  	
    u16 i,j;			
    u16 width = ex-sx+1; 
    u16 height = ey-sy+1;
    LCD_SetWindows(sx, sy, ex-1, ey-1);

    LCD_RS_SET;
    LCD_CS_CLR;	   
    for(i=0;i<height;i++)
    {
        for(j=0;j<width;j++)
        {
            DATAOUT(color);	
            LCD_WR_CLR;
            LCD_WR_SET;	
            
            DATAOUT(color<<8);	
            LCD_WR_CLR;
            LCD_WR_SET;	
        }   
    }
    LCD_CS_SET;	

    LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);
}
/*****************************************************************/
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
{	
    /*LCD_WR_REG(lcddev.setxcmd);	
    LCD_WR_DATA(xStar>>8);
    LCD_WR_DATA(0x00FF&xStar);		
    LCD_WR_DATA(xEnd>>8);
    LCD_WR_DATA(0x00FF&xEnd);
    LCD_WR_REG(lcddev.setycmd);	
    LCD_WR_DATA(yStar>>8);
    LCD_WR_DATA(0x00FF&yStar);		
    LCD_WR_DATA(yEnd>>8);
    LCD_WR_DATA(0x00FF&yEnd);	
    LCD_WriteRAM_Prepare();		*/	
  
    LCD_WR_REG(lcddev.setxcmd);	
    LCD_WR_DATA(xStar);
    LCD_WR_DATA(xStar<<8);		
    LCD_WR_DATA(xEnd);
    LCD_WR_DATA(xEnd<<8);
    LCD_WR_REG(lcddev.setycmd);	
    LCD_WR_DATA(yStar);
    LCD_WR_DATA(yStar<<8);		
    LCD_WR_DATA(yEnd);
    LCD_WR_DATA(yEnd>>8);	
    LCD_WriteRAM_Prepare();	
}   

/*****************************************************************/
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	  	    			
    /*LCD_WR_REG(lcddev.setxcmd);	
    LCD_WR_DATA(Xpos>>8);
    LCD_WR_DATA(0x00FF&Xpos);		
    LCD_WR_REG(lcddev.setycmd);	
    LCD_WR_DATA(Ypos>>8);
    LCD_WR_DATA(0x00FF&Ypos);		
    LCD_WriteRAM_Prepare();*/

    LCD_WR_REG(lcddev.setxcmd);	
    LCD_WR_DATA(Xpos);
    LCD_WR_DATA(Xpos<<8);		
    LCD_WR_REG(lcddev.setycmd);	
    LCD_WR_DATA(Ypos);
    LCD_WR_DATA(Ypos<<8);		
    LCD_WriteRAM_Prepare();
} 

/*****************************************************************/
void LCD_SetParam(void)
{ 	
    lcddev.wramcmd=34;
#if USE_HORIZONTAL==1 
    lcddev.dir=0;
    lcddev.width=320;
    lcddev.height=240;
    lcddev.setxcmd=32;
    lcddev.setycmd=33;			
    LCD_WriteReg(0x36,0x0028);
#else
    lcddev.dir=0;		 	 		
    lcddev.width=240;          
    lcddev.height=320;
    lcddev.setxcmd=0x32;
    lcddev.setycmd=0x33;	
    LCD_WriteReg(0x36,0x0028);
#endif
}	


