#include <sys/cdefs.h>
#include <ctype.h>
#include <curses.h>
#include <err.h>
#include <signal.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include "main.h"

#define newlink() (struct body *) malloc(sizeof (struct body));

WINDOW *tv;
WINDOW *stw;
char outbuf[BUFSIZ];


int main(int argc, char **argv)
{
    /*Create worm*/
    Worm player_1;
    player_1.HEAD = '@';
    player_1.BODY = '#';
    player_1.growing = 0;
    player_1.running = 0;
    player_1.score = 0;

    /* Revoke setgid privileges */
    setregid(getgid(), getgid());

    setbuf(stdout, outbuf);
    srand(getpid());
    initscr();
    cbreak();
    noecho();

#ifdef KEY_LEFT
    keypad(stdscr, TRUE);
#endif
    player_1.slow = (baudrate() <= 1200);
    clear();
    if (COLS < 18 || LINES < 5) {
        /*
         * Insufficient room for the line with " Worm" and the
         * score if fewer than 18 columns; insufficient room for
         * anything much if fewer than 5 lines.
         */
        endwin();
        errx(1, "screen too small");
    }


    if (argc == 2)
        player_1.start_len = atoi(argv[1]);
    if ((player_1.start_len <= 0) || (player_1.start_len > ((LINES-3) * (COLS-2)) / 3))
        player_1.start_len = LENGTH;


    // Create the window
    stw = newwin(1, COLS-1, 0, 0);
    tv = newwin(LINES-1, COLS-1, 1, 0);
    box(tv, '*', '*');
    scrollok(tv, FALSE);
    scrollok(stw, FALSE);
    wmove(stw, 0, 0);
    wprintw(stw, " Worm");
    refresh();
    wrefresh(stw);
    wrefresh(tv);


    life(&player_1,2);                 /* Create the worm */
    prize();          /* Put up a goal */


    while(1) {
        if (player_1.running) {
            player_1.running--;
            process(&player_1, player_1.lastch);
        } else {
            fflush(stdout);
            process(&player_1, getch());
        }
    }
}

void life(Worm *W, int ypos)
{
    struct body *bp, *np;
    int i, j = 1;

    np = NULL;
    W->head = newlink();
    if (W->head == NULL)
        err(1, NULL);

    W->head->x = W->start_len % (COLS-5) + 2;
    W->head->y = LINES / ypos;
    W->head->next = NULL;
    display(W->head, W->HEAD);

    for (i = 0, bp = W->head; i < W->start_len; i++, bp = np) {
        np = newlink();

        if (np == NULL)
            err(1, NULL);
        np->next = bp;
        bp->prev = np;

        if (((bp->x <= 2) && (j == 1)) || ((bp->x >= COLS-4) && (j == -1))) {
            j *= -1;
            np->x = bp->x;
            np->y = bp->y + 1;
        } else {
            np->x = bp->x - j;
            np->y = bp->y;
        }
        display(np, W->BODY);
    }

    W->tail = np;
    W->tail->prev = NULL;
    W->visible_len = W->start_len + 1;
}

void display(const struct body *pos, char chr)
{
    wmove(tv, pos->y, pos->x);
    waddch(tv, chr);
}


int rnd(int range)
{
    return abs((rand()>>5)+(rand()>>5)) % range;
}

void newpos(Worm *W, struct body * bp)
{
    if (W != NULL && W->visible_len == (LINES-3) * (COLS-3) - 1) {
        endwin();

        printf("\nYou won!\n");
        printf("Your final score was %d\n\n", W->score);
        exit(0);
    }

    do {
        bp->y = rnd(LINES-3)+ 1;
        bp->x = rnd(COLS-3) + 1;
        wmove(tv, bp->y, bp->x);
    } while(winch(tv) != ' ');
}

void prize(void)
{
    int value;
    value = rnd(9) + 1;
    newpos(NULL, &goody);
    waddch(tv, value+'0');
    wrefresh(tv);
}

void process(Worm *W, int ch)
{
    int x,y; // Head position
    struct body *nh;

    x = W->head->x;
    y = W->head->y;

    switch(ch) {
#ifdef KEY_LEFT
    case KEY_LEFT:
#endif
    case 'h':
        x--;
        break;

#ifdef KEY_DOWN
    case KEY_DOWN:
#endif
    case 'j':
        y++;
        break;

#ifdef KEY_UP
    case KEY_UP:
#endif
    case 'k':
        y--;
        break;

#ifdef KEY_RIGHT
    case KEY_RIGHT:
#endif
    case 'l':
        x++;
        break;

    default:
        return;
    }

    W->lastch = ch;

    if (W->growing == 0) { // Update the tail of the worm
        display(W->tail, ' ');
        W->tail->next->prev = NULL;
        nh = W->tail->next;
        free(W->tail);
        W->tail = nh;
        W->visible_len--;
    } else {
        W->growing--;
    }

    display(W->head, W->BODY); // Transform head into a body part
    wmove(tv, y, x); // Moves the cursor to the head position

    if (isdigit(ch = winch(tv))) { // Obtain the caracter at screen cursor
        W->growing += ch-'0';
        prize();
        W->score += W->growing;
        W->running = 0;
        wmove(stw, 0, COLS - 12);
        wprintw(stw, "Score: %3d", W->score);
        wrefresh(stw);
    } else if(ch != ' ')
        crash(W);

    nh = newlink();

    if (nh == NULL)
        err(1, NULL);

    nh->next = NULL;
    nh->prev = W->head;
    W->head->next = nh;
    nh->y = y;
    nh->x = x;
    display(nh, W->HEAD);
    W->head = nh;
    W->visible_len++;

    if (!(W->slow && W->running)) {
        wmove(tv, W->head->y, W->head->x);
        wrefresh(tv);
    }

}

void crash(Worm *W)
{
    endwin();
    printf("\nWell, you ran into something and the game is over.\n");
    printf("Your final score was %d\n\n", W->score);
    exit(0);
}

