#include <sys/cdefs.h>
#include <ctype.h>
#include <curses.h>
#include <err.h>
#include <signal.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include "main.h"
#include "worm.h"
#include "defines.h"

WINDOW *play_area;
WINDOW *status_bar;
char outbuf[BUFSIZ];
int current_player = 0;

int main()
{
    Worm *p[5];
    setup();

    /*Create worm*/
    p[0] = add_player('@', '#', 80);
    p[1] = add_player('D', '=', 125);
    p[2] = add_player('X', '+', 135);
    p[3] = add_player('O', '-', 75);
    p[4] = add_player('K', 'o', 145);

    prize();          /* Put up a goal */
    wrefresh(play_area);
    fflush(stdout);

    while(current_player < 5) {
        process(p[current_player], getch());
        fflush(stdout);
    }

    crash(p[0]);
}

Worm *add_player(char head, char body, int initial_size)
{
    WormBody pos;
    Worm *p = WormCreate(head, body, initial_size);
    if(p == NULL)
        err(1, NULL);

    while(1){
        newpos(&pos);
        if ((checkpos(&pos) && ALL_EMPTY)){
            break;
        }
    }
    WormInit(p, pos);
    return p;
}

void display(const WormBody *pos, char chr)
{
    wmove(play_area, pos->y, pos->x);
    waddch(play_area, chr);
}

int rnd(int range)
{
    return abs((rand()>>5)+(rand()>>5)) % range;
}

int checkpos(WormBody *pos)
{
    int result = 0;

    if(emptypos(pos->x - 1, pos->y))
        result |= LEFT_EMPTY;
    if (emptypos(pos->x + 1, pos->y))
        result |= RIGHT_EMPTY;
    if (emptypos(pos->x, pos->y + 1))
        result |= DOWN_EMPTY;
    if (emptypos(pos->x, pos->y - 1))
        result |= UP_EMPTY;

    return result;
}

void newpos(WormBody *pos)
{
    do {
        pos->y = rnd(PLAYABLE_HEIGHT) + 1;
        pos->x = rnd(PLAYABLE_WIDTH) + 1;
    } while(!(emptypos(pos->x, pos->y)));
}

int emptypos(int x, int y)
{
    wmove(play_area, y, x);
    return winch(play_area) == ' ';
}

int chatpos(int x, int y)
{
    wmove(play_area, y, x);
    return winch(play_area);
}

void prize(void)
{
    WormBody goody;
    int value = rnd(9) + 1;
    newpos(&goody);
    waddch(play_area, value + '0');
}

void process(Worm *W, int ch)
{
    int x, y; // Head position
    x = W->head->x;
    y = W->head->y;

    switch(ch) {
    case KEY_LEFT:
    case KEYBOARD_LEFT:
        x--;
        break;

    case KEY_DOWN:
    case KEYBOARD_DOWN:
        y++;
        break;

    case KEY_UP:
    case KEYBOARD_UP:
        y--;
        break;

    case KEY_RIGHT:
    case KEYBOARD_RIGHT:
        x++;
        break;

    default:
        return;
    }

    ch = WormMove(W, x, y);
    if (ch > 0) {
        update_score(W->score);
        prize();
    } else if (ch < 0) {
        WormDestroy(W);
        current_player++;
    }

    wrefresh(play_area);
}

void crash(Worm *W)
{
    endwin();
    printf("\nWell, you ran into something and the game is over.\n");
    printf("Your final score was %d\n\n", W->score);
    exit(0);
}

void update_score(int score)
{
    wmove(status_bar, 0, COLS - 12);
    wprintw(status_bar, "Score: %3d", score);
    wrefresh(status_bar);
}

void setup(void)
{
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
    clear();
    if (COLS < MIN_WINDOW_WIDTH || LINES < MIN_WINDOW_HEIGHT) {
        /*
         * Insufficient room for the line with " Worm" and the
         * score if fewer than 18 columns; insufficient room for
         * anything much if fewer than 5 lines.
         */
        endwin();
        errx(1, "screen too small");
    }

    // Hide cursor
    curs_set(0);

    // Create the game windows
    status_bar = newwin(1, WINDOW_WIDTH, 0, 0);
    scrollok(status_bar, FALSE);
    wmove(status_bar, 0, 0);
    wprintw(status_bar, " Worm");

    play_area = newwin(WINDOW_HEIGHT, WINDOW_WIDTH, 1, 0);
    scrollok(play_area, FALSE);
    box(play_area, '*', '*');

    refresh();
    wrefresh(status_bar);
    wrefresh(play_area);

    update_score(0);
}
