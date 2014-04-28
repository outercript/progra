#include <stdlib.h>
#include <ctype.h>
#include "worm.h"

Worm *WormCreate(char head, char body, int initial_size)
{
    Worm* w = calloc(1, sizeof(Worm));
    if (w == NULL)
        err(1, NULL);

    w->HEAD = head;
    w->BODY = body;
    w->start_len = initial_size;

    return w;
}

void WormDestroy(Worm *w)
{
    WormBody *current, *next;
    for(current = w->tail; current != NULL; current=next){
        next = current->next;
        display(current, ' ');
        free(current);
    }
    free(w);
}

void WormInit(Worm *w, WormBody pos)
{
    int i;
    WormGrowHead(w, pos.x, pos.y);

    for (i = 0, w->tail = w->head; i < w->start_len; i++) {
        pos.x = w->tail->x;
        pos.y = w->tail->y;

        // Left, Right, Down, Up
        if (emptypos(pos.x - 1, pos.y)) {
            pos.x--;
        } else if (emptypos(pos.x + 1, pos.y)) {
            pos.x++;
        } else if (emptypos(pos.x, pos.y + 1)) {
            pos.y++;
        } else if (emptypos(pos.x, pos.y - 1)) {
            pos.y--;
        } else {
            break;
        }

        WormGrowTail(w, pos.x, pos.y);
    }
}

int WormMove(Worm *w, int x, int y)
{
    int ch, score = 0;
    if (w->growing == 0) {
        WormStripTail(w);
    } else {
        w->growing--;
    }

    display(w->head, w->BODY);

    if (isdigit(ch = chatpos(x, y))) {
        w->growing += ch - '0';
        w->score += w->growing;
        score = w->growing;
    } else if(ch != ' '){
        return -1;
    }

    WormGrowHead(w, x, y);
    return score;
}

void WormGrowHead(Worm *w, int x, int y)
{
    WormBody *new_head = malloc(sizeof(WormBody));
    if (new_head == NULL)
        err(1, NULL);

    new_head->y = y;
    new_head->x = x;
    new_head->next = NULL;

    if(w->head != NULL){
        new_head->prev = w->head;
        w->head->next = new_head;
    }

    w->head = new_head;
    display(w->head, w->HEAD);
    w->visible_len++;
}

void WormGrowTail(Worm *w, int x, int y)
{
    WormBody *new_tail = malloc(sizeof (WormBody));
    if (new_tail == NULL)
        err(1, NULL);

    new_tail->x = x;
    new_tail->y = y;
    new_tail->prev = NULL;
    new_tail->next = w->tail;
    w->tail->prev = new_tail;

    w->tail = new_tail;
    display(w->tail, w->BODY);
    w->visible_len++;
}

void WormStripTail(Worm* w)
{
    WormBody *new_tail;
    if (w->tail == NULL)
        err(1, NULL);

    new_tail = w->tail->next;
    new_tail->prev = NULL;
    display(w->tail, ' ');
    free(w->tail);

    w->tail = new_tail;
    w->visible_len--;
}
