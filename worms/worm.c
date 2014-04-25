#include <stdlib.h>
#include "worm.h"

Worm *WormCreate(char head, char body, int initial_size)
{
    Worm* w = calloc(1, sizeof(Worm));
    w->HEAD = head;
    w->BODY = body;
    w->start_len = initial_size;
    w->slow = (baudrate() <= 1200);

    return w;
}

void WormGrowHead(Worm *w, int x, int y)
{
    struct body *new_head = malloc(sizeof (struct body));
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
    w->visible_len++;
}

void WormGrowTail(Worm *w, int x, int y)
{
    struct body *new_tail = malloc(sizeof (struct body));
    if (new_tail == NULL)
        err(1, NULL);

    new_tail->x = x;
    new_tail->y = y;
    new_tail->prev = NULL;
    new_tail->next = w->tail;
    w->tail->prev = new_tail;

    w->tail = new_tail;
    w->visible_len++;
}

void WormStripTail(Worm* w)
{
    struct body *new_tail;
    w->tail->next->prev = NULL;
    new_tail = w->tail->next;
    free(w->tail);
    w->tail = new_tail;
    w->visible_len--;
}
