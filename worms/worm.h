#ifndef __WORM__
#define __WORM__

#define WORM_LENGTH     7

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


Worm *WormCreate(char head, char body, int length, WormBody pos);
void WormDestroy(Worm *w);
int WormMove(Worm *w, int position);

#endif // __WORM__
