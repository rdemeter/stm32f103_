#ifndef _BOUNCEUTILS_H_
#define _BOUNCEUTILS_H_

#include "touch.h" 
#include "glcd.h"
#include "delay.h"

#define WIDTH		240
#define HEIGHT		320
#define CHAR_H   	8           /* Character Height (in pixels)       */
#define CHAR_W   	8           /* Character Width (in pixels)        */
#define BAR_W           10	    /* Bar Width (in pixels) */
#define BAR_H		48	    /* Bar Height (in pixels) */
#define BAR_ON		1
#define BAR_OFF		0
#define T_LONG		10000000
#define T_SLOW		100000
#define T_MEDIUM	10000
#define T_FAST		1000

enum  pMode {SGL, DUAL, AI};

typedef struct {
  int x;
  int y;
} PaddleInfo;

typedef struct {
  int dirn;
  int x; 
  int y; 
} BallInfo;

typedef struct {
  unsigned long delay;
  enum pMode mode;
  PaddleInfo p1;
  PaddleInfo p2;
  BallInfo ball;
} GameInfo;

#endif
