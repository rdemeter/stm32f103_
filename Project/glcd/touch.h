/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               TouchPanel.h
** Descriptions:            The TouchPanel application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*********************************************************************************************************/

#ifndef _TOUCHPANEL_H_
#define _TOUCHPANEL_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Private typedef -----------------------------------------------------------*/
typedef	struct POINT 
{
   uint16_t x;
   uint16_t y;
}Coordinate;


typedef struct Matrix 
{						
long double An,  
            Bn,     
            Cn,   
            Dn,    
            En,    
            Fn,     
            Divider ;
} Matrix ;

/* Private variables ---------------------------------------------------------*/
extern Coordinate ScreenSample[7];
extern Coordinate DisplaySample[7];
extern Matrix matrix ;
extern Coordinate  display ;

/* Private define ------------------------------------------------------------*/
/* AD channel selection command and register */
#define	CHX 	0x90 	/* channel Y+ selection command */	
#define	CHY 	0xd0	/* channel X+ selection command*/

#define TP_CS(x)	x ? GPIO_SetBits(GPIOA,GPIO_Pin_4): GPIO_ResetBits(GPIOA,GPIO_Pin_4)

#define TP_INT_IN   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)

/* Private function prototypes -----------------------------------------------*/				
void TP_Init(void);	
Coordinate *Read_Ads7846(void);
void waitTouch(void);
void TouchPanel_Calibrate(void);
void DrawCross(uint16_t Xpos,uint16_t Ypos);
void TP_DrawPoint(uint16_t Xpos,uint16_t Ypos);
FunctionalState setCalibrationMatrix( Coordinate * displayPtr,Coordinate * screenPtr,Matrix * matrixPtr);
FunctionalState getDisplayPoint(Coordinate * displayPtr,Coordinate * screenPtr,Matrix * matrixPtr );

#endif

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/


