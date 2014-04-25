#ifndef __WORMS__
#define __WORMS__

#define LENGTH 7
#define RUNLEN 8

struct body {
    int x;
    int y;
    struct body *prev;
    struct body *next;
} goody;

typedef struct {
    struct body *head;
    struct body *tail;
    char HEAD;
    char BODY;
    int growing;
    int running;
    int slow;
    int score;
    int start_len;
    int visible_len;
    int lastch;
} Worm;


void  crash(Worm *);
void  display(const struct body *, char);
void  life(Worm *, int);
void  newpos(Worm *, struct body *);
void  process(Worm *,int);
void  prize(void);
int   rnd(int);
void  setup(void);

#endif
