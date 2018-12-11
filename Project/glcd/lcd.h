#ifndef __LCD_H
#define __LCD_H		
#include "sys.h"	 
#include "stdlib.h"
#include "font.h"
/**************************************************************************************************
§µ§á§â§Ñ§Ó§Ý§Ö§ß§Ú§Öfggfàïîî
LCD_RST - PC5
LCD_RD - PC6
LCD_WR - PC7
LCD_RS - PC8
LCD_CS - PC9

§¥§Ñ§ß§ß§í§Ö
LCD_D0  -  PB8
...
LCD_D8 - PB15
**************************************************************************************************/

typedef struct  
{										    
	u16 width;
	u16 height;
	u16 id;
	u8  dir;		
	u16	 wramcmd;	
	u16  setxcmd;	
	u16  setycmd;	
}_lcd_dev; 	

extern _lcd_dev lcddev;

#define USE_HORIZONTAL          1
#define LCD_USE8BIT_MODEL       1

#if USE_HORIZONTAL==1
#define LCD_W 320
#define LCD_H 240
#else
#define LCD_W 240
#define LCD_H 320
#endif

extern u16  POINT_COLOR;
extern u16  BACK_COLOR;

/*      Ïîðò äàííûõ             */
#define DATAOUT(x) GPIOB->ODR=x
#define DATAIN     GPIOB->IDR 

/*      Ñèãíàëû óïðàâëåíèÿ      */
#define LCD_CS_PIN      9
#define LCD_RS_PIN      8
#define LCD_WR_PIN      7
#define LCD_RD_PIN      6
#define LCD_RST_PIN     5
#define LCD_BL_PIN      4

#define	LCD_CS_SET      GPIOC->BSRR=1<<LCD_CS_PIN    //PC9
#define	LCD_RS_SET	GPIOC->BSRR=1<<LCD_RS_PIN    //PC8	   
#define	LCD_WR_SET	GPIOC->BSRR=1<<LCD_WR_PIN    //PC7
#define	LCD_RD_SET	GPIOC->BSRR=1<<LCD_RD_PIN    //PC6
#define	LCD_RST_SET	GPIOC->BSRR=1<<LCD_RST_PIN    //PC5
#define	LCD_BL_SET	GPIOC->BSRR=1<<LCD_BL_PIN    //PC5
								    
#define	LCD_CS_CLR      GPIOC->BRR=1<<LCD_CS_PIN     //PC9
#define	LCD_RS_CLR	GPIOC->BRR=1<<LCD_RS_PIN     //PC8	   
#define	LCD_WR_CLR	GPIOC->BRR=1<<LCD_WR_PIN     //PC7
#define	LCD_RD_CLR	GPIOC->BRR=1<<LCD_RD_PIN     //PC6
#define	LCD_RST_CLR	GPIOC->BRR=1<<LCD_RST_PIN    //PC5								    

#define	LCD_LED PCout(10)

#define L2R_U2D  0
#define L2R_D2U  1
#define R2L_U2D  2
#define R2L_D2U  3

#define U2D_L2R  4
#define U2D_R2L  5
#define D2U_L2R  6
#define D2U_R2L  7

#define DFT_SCAN_DIR  L2R_U2D

// §¬§à§Õ§í §è§Ó§Ö§ä§à§Ó
#define WHITE       0xFFFF
#define BLACK       0x0000	  
#define BLUE        0x001F  
#define BRED        0XF81F
#define GRED 	    0XFFE0
#define GBLUE	    0X07FF
#define RED         0xF800
#define MAGENTA     0xF81F
#define GREEN       0x07E0
#define CYAN        0x7FFF
#define YELLOW      0xFFE0
#define BROWN 	    0XBC40 
#define BRRED 	    0XFC07 
#define GRAY  	    0X8430 
#define DARKBLUE    0X01CF
#define LIGHTBLUE   0X7D7C
#define GRAYBLUE    0X5458 
#define ORANGE      0XFD20
#define OLIVE       0x7BE0
#define MAROON      0x7800
#define LIGHTGREEN  0X841F 
#define LIGHTGRAY   0XEF5B 
#define LGRAY 	    0XC618 

#define LGRAYBLUE   0XA651
#define LBBLUE      0X2B12
	    															  
extern u16 BACK_COLOR, POINT_COLOR ;  

void LCD_Init(void);
void LCD_DisplayOn(void);
void LCD_DisplayOff(void);
//void LCD_Display_Dir(u8);
void LCD_Clear(u16 Color);	 
void LCD_SetCursor(u16 Xpos, u16 Ypos);
void LCD_DrawPoint(u16 x,u16 y);
u16  LCD_ReadPoint(u16 x,u16 y);
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd);
void LCD_DrawPoint_16Bit(u16 color);
u16 LCD_RD_DATA(void);									    
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue);
void LCD_WR_DATA(u16 data);
u16 LCD_ReadReg(u8 LCD_Reg);
void LCD_WriteRAM_Prepare(void);
void LCD_WriteRAM(u16 RGB_Code);
u16 LCD_ReadRAM(void);		   
u16 LCD_BGR2RGB(u16 c);
void LCD_SetParam(void);
uint32_t LCD_Putchar(u16 x,u16 y,u16 FontColor, u16 BackColor, char c, u8 mode, sFONT *font);
void LCD_DrawString(u16 x, u16 y, u16 FontColor, u16 BackColor, char *str, u8 mode, sFONT *font);
#define LCD_REG_0             0x00
#define LCD_REG_1             0x01
#define LCD_REG_2             0x02
#define LCD_REG_3             0x03
#define LCD_REG_4             0x04
#define LCD_REG_5             0x05
#define LCD_REG_6             0x06
#define LCD_REG_7             0x07
#define LCD_REG_8             0x08
#define LCD_REG_9             0x09
#define LCD_REG_10            0x0A
#define LCD_REG_12            0x0C
#define LCD_REG_13            0x0D
#define LCD_REG_14            0x0E
#define LCD_REG_15            0x0F
#define LCD_REG_16            0x10
#define LCD_REG_17            0x11
#define LCD_REG_18            0x12
#define LCD_REG_19            0x13
#define LCD_REG_20            0x14
#define LCD_REG_21            0x15
#define LCD_REG_22            0x16
#define LCD_REG_23            0x17
#define LCD_REG_24            0x18
#define LCD_REG_25            0x19
#define LCD_REG_26            0x1A
#define LCD_REG_27            0x1B
#define LCD_REG_28            0x1C
#define LCD_REG_29            0x1D
#define LCD_REG_30            0x1E
#define LCD_REG_31            0x1F
#define LCD_REG_32            0x20
#define LCD_REG_33            0x21
#define LCD_REG_34            0x22
#define LCD_REG_36            0x24
#define LCD_REG_37            0x25
#define LCD_REG_40            0x28
#define LCD_REG_41            0x29
#define LCD_REG_43            0x2B
#define LCD_REG_45            0x2D
#define LCD_REG_48            0x30
#define LCD_REG_49            0x31
#define LCD_REG_50            0x32
#define LCD_REG_51            0x33
#define LCD_REG_52            0x34
#define LCD_REG_53            0x35
#define LCD_REG_54            0x36
#define LCD_REG_55            0x37
#define LCD_REG_56            0x38
#define LCD_REG_57            0x39
#define LCD_REG_59            0x3B
#define LCD_REG_60            0x3C
#define LCD_REG_61            0x3D
#define LCD_REG_62            0x3E
#define LCD_REG_63            0x3F
#define LCD_REG_64            0x40
#define LCD_REG_65            0x41
#define LCD_REG_66            0x42
#define LCD_REG_67            0x43
#define LCD_REG_68            0x44
#define LCD_REG_69            0x45
#define LCD_REG_70            0x46
#define LCD_REG_71            0x47
#define LCD_REG_72            0x48
#define LCD_REG_73            0x49
#define LCD_REG_74            0x4A
#define LCD_REG_75            0x4B
#define LCD_REG_76            0x4C
#define LCD_REG_77            0x4D
#define LCD_REG_78            0x4E
#define LCD_REG_79            0x4F
#define LCD_REG_80            0x50
#define LCD_REG_81            0x51
#define LCD_REG_82            0x52
#define LCD_REG_83            0x53
#define LCD_REG_96            0x60
#define LCD_REG_97            0x61
#define LCD_REG_106           0x6A
#define LCD_REG_118           0x76
#define LCD_REG_128           0x80
#define LCD_REG_129           0x81
#define LCD_REG_130           0x82
#define LCD_REG_131           0x83
#define LCD_REG_132           0x84
#define LCD_REG_133           0x85
#define LCD_REG_134           0x86
#define LCD_REG_135           0x87
#define LCD_REG_136           0x88
#define LCD_REG_137           0x89
#define LCD_REG_139           0x8B
#define LCD_REG_140           0x8C
#define LCD_REG_141           0x8D
#define LCD_REG_143           0x8F
#define LCD_REG_144           0x90
#define LCD_REG_145           0x91
#define LCD_REG_146           0x92
#define LCD_REG_147           0x93
#define LCD_REG_148           0x94
#define LCD_REG_149           0x95
#define LCD_REG_150           0x96
#define LCD_REG_151           0x97
#define LCD_REG_152           0x98
#define LCD_REG_153           0x99
#define LCD_REG_154           0x9A
#define LCD_REG_157           0x9D
#define LCD_REG_192           0xC0
#define LCD_REG_193           0xC1
#define LCD_REG_227           0xE3
#define LCD_REG_229           0xE5
#define LCD_REG_231           0xE7
#define LCD_REG_239           0xEF
					  		 
#endif  
	 
	 



