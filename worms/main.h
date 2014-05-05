#ifndef __WORMS__
#define __WORMS__

#include "worm.h"

#define MAXLINE	    255
#define LENGTH 7
#define RUNLEN 8

#define PLAYABLE_WIDTH      (WINDOW_WIDTH-2)
#define PLAYABLE_HEIGHT     (WINDOW_HEIGHT-2)

typedef struct {
    char filename[MAXLINE];
    Worm *worm;
} Player;

void  prize(void);
void  newpos(WormBody *);

Player *PlayerCreate(char *filename, char head, char body, int length);
void PlayerDestroy(Player *);
void *PlayerMonitor(void *);

#endif
