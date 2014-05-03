#ifndef __WORMS__
#define __WORMS__

#include "worm.h"

#define LENGTH 7
#define RUNLEN 8

#define PLAYABLE_WIDTH      (WINDOW_WIDTH-2)
#define PLAYABLE_HEIGHT     (WINDOW_HEIGHT-2)

void  setup(void);
void  prize(void);
void  crash(Worm *);
void  newpos(WormBody *);
void  process(Worm *, int);

Worm  *add_player(char head, char body, int initial_size);

#endif
