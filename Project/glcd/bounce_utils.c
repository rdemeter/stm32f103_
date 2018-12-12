#include "bounce_utils.h"

GameInfo thisGame;
BallInfo initPstn;

void menu1(void);
void menu2(void);

int getGameInfo()
{
  return thisGame.mode;
}

/******************************************************************************
*  LCDupdateBall(int x, int y)
*  Draw ball(at position x,y) on the LCD
*******************************************************************************/
void LCDupdateBall(int x, int y)
{
  static int last_x = 0;
  static int last_y = 0;;

  //Font_16x24_h[0 * 24]
  /* First erase the last ball drawn */
  GLCD_DrawCharW((unsigned int) last_x, (unsigned int) last_y, (unsigned int) 16, (unsigned int) 24, "O"); 
  /* Then draw the new ball */	
  GLCD_DrawCharB((unsigned int) x,(unsigned int) y, (unsigned int) 16, (unsigned int) 24, "O");
  /* Save the new position */
  last_x = x; last_y = y;
}

/******************************************************************************
*  LCDupdatePaddle(PaddleInfo p)
*  Draw paddle (p) on the LCD
*******************************************************************************/
void LCDupdatePaddle(PaddleInfo p)
{
  static unsigned int yyP1 = 0; 
  static unsigned int yyP2 = 0;
  
  if (p.x==0) {
    if (yyP1 != p.y) {
      GLCD_DrawRect((unsigned int) p.x, yyP1, (unsigned int) BAR_W, (unsigned int) BAR_H, (unsigned int) BAR_OFF);
      GLCD_DrawRect((unsigned int) p.x, p.y, (unsigned int) BAR_W, (unsigned int) BAR_H, (unsigned int) BAR_ON);
      yyP1 = p.y;
    }
  }
  else {
    if (yyP2 != p.y) {
      if(thisGame.mode!=SGL) {
        GLCD_DrawRect((unsigned int) p.x, yyP2, (unsigned int) BAR_W, (unsigned int) BAR_H, (unsigned int) BAR_OFF);
	GLCD_DrawRect((unsigned int) p.x, p.y, (unsigned int) BAR_W, (unsigned int) BAR_H, (unsigned int) BAR_ON);
      }
      yyP2 = p.y;
    }
  }
}

/******************************************************************************
* update_player(unsigned int *)
* Read the ADC and draw the player 1's bat				
*******************************************************************************/
void update_player1()
{
  static int lastValue = 0;
  if(thisGame.mode !=AI && !GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2)) {
    getDisplayPoint(&display, Read_Ads7846(), &matrix );
  }

  if(display.x >=0 && display.x <= WIDTH && display.y >=0 && display.y <=HEIGHT) {
    thisGame.p1.y = (unsigned int) display.y;
    LCDupdatePaddle(thisGame.p1);
    lastValue = display.y;
  }
}

void update_player2()
{
  static int lastValue = 0;
  if(thisGame.mode !=AI && !GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2))
  {
     getDisplayPoint(&display, Read_Ads7846(), &matrix ) ;
  }
  
  if(display.x >=0 && display.x <= WIDTH && display.y >=0 && display.y <=HEIGHT){
    thisGame.p2.y = (unsigned int) display.y;
    LCDupdatePaddle(thisGame.p2);
    lastValue = display.y;
  }
}

/******************************************************************************
*  undate_ball()
*  update the ball position - depending on direction of travel
*******************************************************************************/
void update_ball()
{
  delay_us(thisGame.delay);
  switch (thisGame.ball.dirn)
  {
    case 0: thisGame.ball.x++;
            break;
    case 1: thisGame.ball.x++;
            thisGame.ball.y--;
            break;
    case 2: thisGame.ball.y--;
            break;
    case 3: thisGame.ball.x--;
            thisGame.ball.y--;
            break;
    case 4: thisGame.ball.x--;
            break;
    case 5: thisGame.ball.x--;
            thisGame.ball.y++;
            break;
    case 6: thisGame.ball.y++;
            break;
    case 7: thisGame.ball.x++;
            thisGame.ball.y++;
            break;			
  }

  if ((thisGame.ball.x<(-CHAR_W)) || (thisGame.ball.x>(WIDTH-CHAR_W))) {		 /* reset position */
    //if (thisGame.ball.x<(-CHAR_W)) {
    delay_us(T_LONG);
    thisGame.ball = initPstn;
  }
  LCDupdateBall(thisGame.ball.x, thisGame.ball.y);
}

/******************************************************************************
*  check_collision(void)
*  check for contact between ball and screen edges/bat and 
*  change direction accordingly
*******************************************************************************/
void check_collision()
{
  /* collision detection */
  if ((thisGame.ball.x<=BAR_W) || thisGame.ball.x>=(WIDTH-CHAR_W-BAR_W)) {
    switch (thisGame.ball.dirn)
    {
      case 0: if (thisGame.mode==SGL)
	       	thisGame.ball.dirn = (thisGame.ball.dirn+4)%8;
	      else {	
                if ( (thisGame.ball.y>=thisGame.p2.y) && (thisGame.ball.y<=(thisGame.p2.y+BAR_H)) )
		    thisGame.ball.dirn = (thisGame.ball.dirn+4)%8;
		else	  ;
		  //thisGame.ball.x = WIDTH-CHAR_W;
	      }
	      break;
      case 1: if (thisGame.mode==SGL)
                thisGame.ball.dirn = (thisGame.ball.dirn+2)%8;
	      else {	
                if ( ((thisGame.ball.y+CHAR_W)>=thisGame.p2.y) && (thisGame.ball.y<=(thisGame.p2.y+BAR_H)) )
		  thisGame.ball.dirn = (thisGame.ball.dirn+2)%8;
		else 
		  thisGame.ball.x = WIDTH-CHAR_W;
	      }
	      break;
      case 3: if ( ((thisGame.ball.y+CHAR_W)>=thisGame.p1.y) && (thisGame.ball.y<=(thisGame.p1.y+BAR_H)) )
                thisGame.ball.dirn = (thisGame.ball.dirn+6)%8;
              else  
                thisGame.ball.x = -CHAR_W;	
              break;
      case 4: if ( ((thisGame.ball.y+CHAR_W)>=thisGame.p1.y) && (thisGame.ball.y<=(thisGame.p1.y+BAR_H)) )
                thisGame.ball.dirn = (thisGame.ball.dirn+4)%8;
              break;
      case 5: if ( ((thisGame.ball.y+CHAR_W)>=thisGame.p1.y) && (thisGame.ball.y<=(thisGame.p1.y+BAR_H)) ) 
                thisGame.ball.dirn = (thisGame.ball.dirn+2)%8;
              else 
                thisGame.ball.x = -CHAR_W;
              break;
                          // single              
                                 // 6 nu e       
                          // mirror              
                      /* case 6: if ( ((thisGame.ball.y+CHAR_W)>=thisGame.p2.y) && (thisGame.ball.y<=(thisGame.p2.y+BAR_H)) )
						thisGame.ball.dirn = (thisGame.ball.dirn+4)%8;
					break;
			case 7: if ( ((thisGame.ball.y+CHAR_W)>=thisGame.p2.y) && (thisGame.ball.y<=(thisGame.p2.y+BAR_H)) ) 
						thisGame.ball.dirn = (thisGame.ball.dirn+6)%8;
					else 
						thisGame.ball.x = WIDTH-CHAR_W;
					break;*/
      case 7: if (thisGame.mode==SGL)
		thisGame.ball.dirn = (thisGame.ball.dirn+6)%8;
	      else {
		if ( ((thisGame.ball.y+CHAR_W)>=thisGame.p2.y) && (thisGame.ball.y<=(thisGame.p2.y+BAR_H)) )
		  thisGame.ball.dirn = (thisGame.ball.dirn+6)%8;
		else 
		  thisGame.ball.x = WIDTH-CHAR_W;
		}
		break;
    }
  }
        
        
        
                       /* if ((thisGame.ball.x<=BAR_W) || thisGame.ball.x>=(WIDTH-CHAR_W-BAR_W)) {
	        	switch (thisGame.ball.dirn)
			{
			case 7: if (thisGame.mode==DUAL)
						thisGame.ball.dirn = (thisGame.ball.dirn+4)%8;
					else {	
						if ( (thisGame.ball.y>=thisGame.p2.y) && (thisGame.ball.y<=(thisGame.p2.y+BAR_H)) )
				          thisGame.ball.dirn = (thisGame.ball.dirn+4)%8;
						else	  ;
						  //thisGame.ball.x = WIDTH-CHAR_W;
					}
				    break;
			case 8: if (thisGame.mode==DUAL)
						thisGame.ball.dirn = (thisGame.ball.dirn+2)%8;
					else {	
						if ( ((thisGame.ball.y+CHAR_W)>=thisGame.p2.y) && (thisGame.ball.y<=(thisGame.p2.y+BAR_H)) )
				          thisGame.ball.dirn = (thisGame.ball.dirn+2)%8;
						else 
						  thisGame.ball.x = WIDTH-CHAR_W;
					}
				    break;
			case 9: if ( ((thisGame.ball.y+CHAR_W)>=thisGame.p1.y) && (thisGame.ball.y<=(thisGame.p1.y+BAR_H)) )
					  thisGame.ball.dirn = (thisGame.ball.dirn+6)%8;
					else  
					  thisGame.ball.x = -CHAR_W;	
					break;
			case 10: if ( ((thisGame.ball.y+CHAR_W)>=thisGame.p1.y) && (thisGame.ball.y<=(thisGame.p1.y+BAR_H)) )
						thisGame.ball.dirn = (thisGame.ball.dirn+4)%8;
					break;
			case 11: if ( ((thisGame.ball.y+CHAR_W)>=thisGame.p1.y) && (thisGame.ball.y<=(thisGame.p1.y+BAR_H)) ) 
						thisGame.ball.dirn = (thisGame.ball.dirn+2)%8;
					else 
						thisGame.ball.x = -CHAR_W;
					break;
                                        
                         case 12: if ( ((thisGame.ball.y+CHAR_W)>=thisGame.p2.y) && (thisGame.ball.y<=(thisGame.p2.y+BAR_H)) )
						thisGame.ball.dirn = (thisGame.ball.dirn+4)%8;
					break;
			case 13: if ( ((thisGame.ball.y+CHAR_W)>=thisGame.p2.y) && (thisGame.ball.y<=(thisGame.p2.y+BAR_H)) ) 
						thisGame.ball.dirn = (thisGame.ball.dirn+6)%8;
					else 
						thisGame.ball.x = WIDTH-CHAR_W;
					break; 
        
                        }
        }
        */
        
  /* collision detection for y*/
  if ((thisGame.ball.y<=0) || thisGame.ball.y>=(HEIGHT-CHAR_H)) {
    switch (thisGame.ball.dirn)
    {
      case 1: thisGame.ball.dirn = (thisGame.ball.dirn+6)%8;
	      break;
      case 2: thisGame.ball.dirn = (thisGame.ball.dirn+4)%8;
	      break;
      case 3: thisGame.ball.dirn = (thisGame.ball.dirn+2)%8;
	      break;
      case 5: thisGame.ball.dirn = (thisGame.ball.dirn+6)%8;
	      break;
      case 6: thisGame.ball.dirn = (thisGame.ball.dirn+4)%8;
	      break;
      case 7: thisGame.ball.dirn = (thisGame.ball.dirn+2)%8;
	      break;
    }
  }
}


/*
 * Start screen
 */
void startScreen(void)
{
  GLCD_Clear(WHITE);
  GLCD_SetTextColor(WHITE);
  GLCD_DisplayString(0, 20, 1, "   STM32 PROJECT      ");
  GLCD_DisplayString(0, 50, 1, "                      ");
  GLCD_DisplayString(0, 80, 1, "     WELOCOME         ");
  GLCD_DisplayString(0, 110, 1,"        TO            ");
  GLCD_DisplayString(0, 140, 1,"     PONG-GAME        ");

  GLCD_SetTextColor(BLACK);

  GLCD_SetBackColor(WHITE);
  GLCD_DisplayString(0, 200, 1, "  Press To Start     ");

  waitTouch();     

  menu1();
  menu2();
  GLCD_Clear(RED);   
  GLCD_SetBackColor(BLACK);
  GLCD_SetTextColor(RED); 
}

/******************************************************************************
*  game_Init()
*  Initialize some game parameters.
*******************************************************************************/
void game_Init()
{
  thisGame.delay = T_FAST;
  LCD_Clear(WHITE);                              /* Clear the GLCD              */
  //GLCD_SetBackColor(WHITE);                    /* Set the Back Color          */
  //GLCD_SetTextColor(BLUE);                     /* Set the Text Color          */

  thisGame.p1.x = 0;
  thisGame.p1.y = 0;
  thisGame.p2.x = (WIDTH-BAR_W);
  thisGame.p2.y = 0;

  initPstn.dirn = 7;
  initPstn.x = (WIDTH-CHAR_W)/2;
  initPstn.y = (HEIGHT-CHAR_H)/2;
  thisGame.ball = initPstn;

  startScreen();
}

/*
 * Menu Screen 1
 */
void menu1(void)
{
  int pos = 2;

  GLCD_Clear(WHITE);
  GLCD_SetBackColor(RED);  
  GLCD_SetTextColor(WHITE);

  while(1) {
    GLCD_DisplayString(0, 20, 1, "        PONG        ");
    /* Prints option with *'s around selected option*/
    if(pos == 1) {
      GLCD_DisplayString(0, 85, 1, "       *Easy*       ");
      GLCD_DrawRect (10, 60, 210, 50, 0);
    }
    else{
      GLCD_DisplayString(0, 85, 1, "        Easy        ");
      GLCD_DrawRect (10, 60, 210, 50,0);
    }
    if(pos == 2) {
      GLCD_DisplayString(0, 145, 1, "      *Medium*      ");
      GLCD_DrawRect (10, 120, 210, 50,0);
    }
    else {
      GLCD_DisplayString(0, 145, 1, "       Medium       ");
      GLCD_DrawRect (10, 120, 210, 50,0);
    }
    if(pos == 3) {
      GLCD_DisplayString(0, 205, 1, "       *Hard*       ");
      GLCD_DrawRect (10, 180, 210, 50,0);
    }
    else {
      GLCD_DisplayString(0, 205, 1, "        Hard        ");
      GLCD_DrawRect (10, 180, 210, 50,0);
    }
    if(pos == 4) {
      GLCD_DisplayString(0, 265, 1, "   *Instructions*   ");
      GLCD_DrawRect (10, 240, 210, 50,0);
    }
    else {
      GLCD_DisplayString(0, 265, 1, "    Instructions    ");
      GLCD_DrawRect (10, 240, 210, 50,0);
    }
    //if (GPIOD->IDR & (1 << 12)){ pos++;};  /* If Joystick up move selected option up */
    //if (GPIOD->IDR & (1 << 14)){pos--;};   /* If Joystick down move selected option down */
    //xy = xyTouch(); /* Get coordinated from touch screen */ qwqw
    // Coordinate  disp ;
  while(1) {
    if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2)) {
      getDisplayPoint(&display, Read_Ads7846(), &matrix ) ;
      
      if(display.y >= 60 && display.y <=290)
	break;
    } 
  }
  /* delay_ms(100);   
  GLCD_DisplayString(display.x,display.y,0,"******");       
  waitTouch();*/
  /* Easy Option */
  if ((display.y > 60) && (display.y <= 110) ) {
    pos = 1;
  }
  /* Medium Option */
  else if ((display.y > 120) && (display.y <= 170) ) {
    pos = 2;
  }
  /* Hard Option */
  else if ((display.y > 180) && (display.y <= 230)  ) {
    pos = 3;
  }
  /* Instructions Option */
  else if ((display.y > 240) && (display.y <= 290) ) {
    pos = 4;
  }

  /* If option seleced and clicked preform relevent action */
  //if (!(GPIOD->IDR & (1 << 11))||s==0) 
  {
    /* Start game on easy*/
    if(pos == 1) {
      thisGame.delay = T_SLOW;
      break;
      }
      /* Start game on medim*/
      if(pos == 2) {
        thisGame.delay = T_MEDIUM;
        break;
      }																	  
      /* Start game on hard */
      if(pos == 3) {
	thisGame.delay = T_FAST;
	break;
      }
      /* Show instructions */
      if(pos == 4) {
	GLCD_Clear(WHITE);
	GLCD_SetBackColor(BLACK);
	GLCD_SetTextColor(WHITE);
				
	GLCD_DisplayString(0, 10, 1, "      PONG         ");
	GLCD_DisplayString(0, 40, 1, "  PONG - a tennis  ");
	GLCD_DisplayString(0, 70, 1, " sports game using ");
	GLCD_DisplayString(0, 100, 1, "  2D graphics was  ");
	GLCD_DisplayString(0, 130, 1, " the first game to ");
	GLCD_DisplayString(0, 160, 1, " reach mainstream  ");
	GLCD_DisplayString(0, 190, 1, "    popularity.    ");	 
	GLCD_DisplayString(0, 220, 1, " First released by ");
	GLCD_DisplayString(0, 250, 1, "   Atari in 1972.  ");
	waitTouch();

	GLCD_Clear(WHITE);
	GLCD_SetBackColor(BLACK);
        GLCD_SetTextColor(WHITE);
      }
    }
  }		
}

/*
 * Menu Screen 2
 */
void menu2(void)
{
  int pos = 1;

  GLCD_Clear(WHITE);
  GLCD_SetBackColor(BLACK);
  GLCD_SetTextColor(WHITE);

  while(1) {
    GLCD_DisplayString(0, 10, 1, "        PONG        ");
    /* Prints option with *'s around selected option*/
    
    if(pos == 1) {
      GLCD_DisplayString(0, 85, 1, "   *Single Player*  ");
      GLCD_DrawRect(10, 60, 210, 50, 0);
    }
    else {
      GLCD_DisplayString(0, 85, 1, "    Single Player   ");
      GLCD_DrawRect (10, 60, 210, 50, 0);
    }

    if(pos == 2) {
      GLCD_DisplayString(0, 145, 1, "  *Mirror Opponent* ");
      GLCD_DrawRect (10, 120, 210, 50, 0);  
    }
    else {
      GLCD_DisplayString(0, 145, 1, "   Mirror Opponent  ");
      GLCD_DrawRect (10, 120, 210, 50, 0);
    }

    if(pos == 3) {
      GLCD_DisplayString(0, 205, 1, "   *AI Opponent*    ");
      GLCD_DrawRect (10, 180, 210, 50, 0);
    }
    else {
      GLCD_DisplayString(0, 205, 1, "    AI Opponent     ");
      GLCD_DrawRect (10, 180, 210, 50, 0);
    }

    if(pos == 4) {
      GLCD_DisplayString(0, 265, 1, "   *Instructions*   ");
      GLCD_DrawRect (10, 240, 210, 50, 0);
    }
    else {
      GLCD_DisplayString(0, 265, 1, "    Instructions    ");
      GLCD_DrawRect (10, 240, 210, 50, 0);
    }
    //if (GPIOD->IDR & (1 << 12)){ pos++;};  /* If Joystick up move selected option up */
    //if (GPIOD->IDR & (1 << 14)){pos--;};   /* If Joystick down move selected option down */
    //xy = xyTouch(); /* Get coordinated from touch screen */qwqw
    //Coordinate  disp ;
    while(1) {
      if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2)) {
        getDisplayPoint(&display, Read_Ads7846(), &matrix ) ;
        
        if(display.y >= 60 && display.y <=290)
          break;
      } 
    }
    
    /*delay_ms(100);
    GLCD_DisplayString(disp.x,disp.y,0,"*******");
    waitTouch();*\
    
    /* Single Player */
    if ((display.y >= 60) && (display.y <= 110) ) {
      pos = 1;
    }
    /* Mirror Opponent */
    else if ((display.y > 120) && (display.y <= 170) ) {
      pos = 2;
    }
    /* AI Opponent */
    else if ((display.y > 180) && (display.y <= 230)  ) {
      pos = 3;
    }
    /* Instructions */
    else if ((display.y > 240) && (display.y <= 290) ) {
      pos = 4;
    }
	    
    /* If option seleced and clicked preform relevent action */
    //if (!(GPIOD->IDR & (1 << 11))||s==0) 
    {
      /* Start game on single*/
      if(pos == 1) {
        thisGame.mode = SGL;
        break;
      }
      /* Start game on dual*/
      if(pos == 2) {
        thisGame.mode = DUAL;
	break;
      }																	  
      /* Start game on ai */
      if(pos == 3) {
        thisGame.mode = AI;
        break;
      }
      /* Show instructions */
      if(pos == 4) {
	GLCD_Clear(WHITE);
	GLCD_SetBackColor(BLACK);
	GLCD_SetTextColor(WHITE);
				
	GLCD_DisplayString(0, 10, 1, "      PONG         ");
	GLCD_DisplayString(0, 40, 1, "  Choose play mode  ");
	GLCD_DisplayString(0, 70, 1, "                   ");
	GLCD_DisplayString(0, 100, 1, "                  ");
	GLCD_DisplayString(0, 130, 1, "                  ");
	GLCD_DisplayString(0, 160, 1, "  Touch Screen to  ");
	GLCD_DisplayString(0, 190, 1, "       Exit       ");	 
	GLCD_DisplayString(0, 230, 1, "                  ");
	GLCD_DisplayString(0, 260, 1, "                  ");
	waitTouch();

	GLCD_Clear(WHITE);
	GLCD_SetBackColor(BLACK);
        GLCD_SetTextColor(WHITE);
      }
    }	
  }		
}

