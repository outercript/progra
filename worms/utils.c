#include <stdlib.h>
#include <curses.h>
#include <unistd.h>
#include <err.h>

#include "utils.h"

const char*	const SERVER_FIFO = "/tmp/worm_server.pipe";
const char* const CLIENT_FIFO = "/tmp/worm_%ld.pipe";

static WINDOW *play_area;
static WINDOW *status_bar;
static char outbuf[BUFSIZ];

int rnd(int range)
{
    return abs((rand()>>5)+(rand()>>5)) % range;
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

void display(int x, int y, char chr)
{
    wmove(play_area, y, x);
    waddch(play_area, chr);
}

int checkpos(int x, int y)
{
    int result = 0;

    if(emptypos(x - 1, y))
        result |= LEFT_EMPTY;
    if (emptypos(x + 1, y))
        result |= RIGHT_EMPTY;
    if (emptypos(x, y + 1))
        result |= DOWN_EMPTY;
    if (emptypos(x, y - 1))
        result |= UP_EMPTY;

    return result;
}

int capture_keyboard()
{
    int key = 0;

    switch(getch()) {
    case KEY_LEFT:
    case KEYBOARD_LEFT:
        key = KEYBOARD_LEFT;
        break;

    case KEY_DOWN:
    case KEYBOARD_DOWN:
        key = KEYBOARD_DOWN;
        break;

    case KEY_UP:
    case KEYBOARD_UP:
        key = KEYBOARD_UP;
        break;

    case KEY_RIGHT:
    case KEYBOARD_RIGHT:
        key = KEYBOARD_RIGHT;
        break;

    case KEYBOARD_EXIT:
        key = KEYBOARD_EXIT;
        break;

    default:
        break;
    }

    return key;
}

void setup_window(void)
{
    setregid(getgid(), getgid());
    setbuf(stdout, outbuf);
    srand(getpid());

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    clear();
    if (COLS < MIN_WINDOW_WIDTH || LINES < MIN_WINDOW_HEIGHT) {
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
}

void terminate_window(void)
{
    endwin();
}

void update_score(int score)
{
    wmove(status_bar, 0, COLS - 12);
    wprintw(status_bar, "Score: %3d", score);
    wrefresh(status_bar);
}

void update_board(void)
{
    wrefresh(play_area);
}
