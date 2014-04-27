#ifndef __WORM__
#define __WORM__

typedef struct {
    struct body *head;
    struct body *tail;
    char HEAD;
    char BODY;
    int growing;
    int slow;
    int score;
    int start_len;
    int visible_len;
    int lastch;
} Worm;

struct body {
    int x;
    int y;
    struct body *prev;
    struct body *next;
} goody;

extern int baudrate(void);
extern void err(int, const char*, ...);

Worm *WormCreate(char head, char body, int initial_size);
void WormStripTail(Worm* w);
void WormGrowTail(Worm *w, int x, int y);
void WormGrowHead(Worm *w, int x, int y);

#endif // __WORM__
