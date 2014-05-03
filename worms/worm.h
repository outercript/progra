#ifndef __WORM__
#define __WORM__

typedef struct body {
    int x;
    int y;
    struct body *prev;
    struct body *next;
} WormBody;

typedef struct {
    WormBody *head;
    WormBody *tail;
    char HEAD;
    char BODY;
    int growing;
    int score;
    int start_len;
    int visible_len;
} Worm;


extern int emptypos(int, int);
extern void err(int, const char*, ...);
extern void display(int, int, char);
extern int chatpos(int, int);


void WormInit(Worm *w, WormBody pos);
Worm *WormCreate(char head, char body, int initial_size);
void WormDestroy(Worm *w);
int WormMove(Worm *w, int position);

#endif // __WORM__
