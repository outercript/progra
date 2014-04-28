#ifndef __WORMS__
#define __WORMS__

#include "worm.h"

#define LENGTH 7
#define RUNLEN 8

#define MIN_WINDOW_WIDTH    18
#define MIN_WINDOW_HEIGHT   5

#define WINDOW_WIDTH        COLS-1
#define WINDOW_HEIGHT       LINES-1
#define PLAYABLE_WIDTH      COLS-3
#define PLAYABLE_HEIGHT     LINES-3

void  display(const WormBody *, char);
int   emptypos(int, int);

int   rnd(int);
void  setup(void);
void  prize(void);
void  crash(Worm *);
void  newpos(WormBody *);
int   checkpos(WormBody *);
void  process(Worm *,int);
void  update_score(int score);
Worm  *add_player(char head, char body, int initial_size);

#endif
