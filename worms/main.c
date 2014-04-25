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

WINDOW *play_area;
WINDOW *status_bar;
char outbuf[BUFSIZ];

int main(int argc, char **argv)
{
    Worm *player_1;
    setup();

    /*Create worm*/
    player_1 = add_player('@', '#', 80);
    add_player('D', '=', 5);
    /*
    if (argc == 2)
        player_1->start_len = atoi(argv[1]);
    if ((player_1->start_len <= 0) || (player_1->start_len > ((PLAYABLE_HEIGHT) * (PLAYABLE_WIDTH)) / 3))
        player_1->start_len = LENGTH;
    life(player_1, 2);                 /* Create the worm */
    prize();          /* Put up a goal */


    while(1) {
        if (player_1->running) {
            player_1->running--;
            process(player_1, player_1->lastch);
        } else {
            fflush(stdout);
            process(player_1, getch());
        }
    }
}

Worm *add_player(char head, char body, int initial_size)
{
    int pos = rnd(4) + 1;
    Worm *p = WormCreate(head, body, initial_size);
    if(p == NULL)
        err(1, NULL);

    life(p, pos);
    return p;
}

void life(Worm *W, int ypos)
{
    int x, y, i, j = 1;

    x = W->start_len % (PLAYABLE_WIDTH-2) + 2;
    y = LINES / ypos;
    WormGrowHead(W, x, y);
    display(W->head, W->HEAD);

    for (i = 0, W->tail = W->head; i < W->start_len; i++) {
        x = W->tail->x;
        y = W->tail->y;
        if (((x <= 2) && (j == 1)) || ((x >= PLAYABLE_WIDTH-1) && (j == -1))) {
            j *= -1;
            y++;
        } else {
            x -= j;
        }
        WormGrowTail(W, x, y);
        display(W->tail, W->BODY);
    }
}

void display(const struct body *pos, char chr)
{
    wmove(play_area, pos->y, pos->x);
    waddch(play_area, chr);
}


int rnd(int range)
{
    return abs((rand()>>5)+(rand()>>5)) % range;
}

void newpos(Worm *W, struct body * bp)
{
    if (W != NULL && W->visible_len >= (20)) {
        endwin();

        printf("\nYou won!\n");
        printf("Your final score was %d\n\n", W->score);
        exit(0);
    }

    do {
        bp->y = rnd(PLAYABLE_HEIGHT) + 1;
        bp->x = rnd(PLAYABLE_WIDTH) + 1;
        wmove(play_area, bp->y, bp->x);
    } while(winch(play_area) != ' ');
}

void prize(void)
{
    int value;
    value = rnd(9) + 1;
    newpos(NULL, &goody);
    waddch(play_area, value+'0');
    wrefresh(play_area);
}

void process(Worm *W, int ch)
{
    int x,y; // Head position
    struct body *new_head;

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
        WormStripTail(W);
    } else {
        W->growing--;
    }

    display(W->head, W->BODY); // Transform head into a body part
    wmove(play_area, y, x); // Moves the cursor to the head position

    if (isdigit(ch = winch(play_area))) { // Obtain the caracter at screen cursor
        W->growing += ch-'0';
        W->score += W->growing;
        W->running = 0;
        update_score(W->score);
        prize();

    } else if(ch != ' ')
        crash(W);

    WormGrowHead(W, x, y);
    display(W->head, W->HEAD);

    if (!(W->slow && W->running)) {
        wmove(play_area, W->head->y, W->head->x);
        wrefresh(play_area);
    }

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
