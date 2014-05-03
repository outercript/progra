#include <sys/cdefs.h>
#include <sys/stat.h>
#include <ctype.h>
#include <curses.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <stdlib.h>
#include <termios.h>
#include <stdio.h>
#include <unistd.h>

#include "main.h"
#include "worm.h"
#include "utils.h"

#define	MAXLINE	    255
#define FIFO_PERMS  (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

int current_player = 0;

int main()
{
    int server_fifo, fd;
    char buffer[MAXLINE], client_filename[MAXLINE];
    int action;
    Worm *p[5];

    setup();
    fflush(stdout);

    // Create worm
    p[0] = add_player('@', '#', 80);
    p[1] = add_player('D', '=', 125);
    p[2] = add_player('X', '+', 135);
    p[3] = add_player('O', '-', 75);
    p[4] = add_player('K', 'o', 145);

    prize();          // Put up a goal
    update_board();
    fflush(stdout);

    if ((mkfifo(SERVER_FIFO, FIFO_PERMS) < 0) && (errno != EEXIST)){
		printf("can't create %s", SERVER_FIFO);
		exit(EXIT_FAILURE);
	}

    server_fifo = open(SERVER_FIFO, O_RDONLY);

    while(current_player < 2){
        while((read(server_fifo, buffer, MAXLINE)) > 0){
            sscanf(buffer, "%s %d", client_filename, &action);
            process(p[current_player], action);
            fflush(stdout);

            fd = open(client_filename, O_WRONLY);
            write(fd, &action, sizeof(action));
            close(fd);
        }
    }

    crash(p[0]);
    close(server_fifo);
	unlink(SERVER_FIFO);

    return 0;
}

Worm *add_player(char head, char body, int initial_size)
{
    WormBody pos;
    Worm *p = WormCreate(head, body, initial_size);
    if(p == NULL)
        err(1, NULL);

    do {
        newpos(&pos);
    } while (!(checkpos(pos.x, pos.y) && ALL_EMPTY));

    WormInit(p, pos);
    return p;
}

void newpos(WormBody *pos)
{
    do {
        pos->y = rnd(PLAYABLE_HEIGHT) + 1;
        pos->x = rnd(PLAYABLE_WIDTH) + 1;
    } while(!(emptypos(pos->x, pos->y)));
}

void prize(void)
{
    WormBody goody;
    int value = rnd(9) + 1;
    newpos(&goody);
    display(goody.x, goody.y, value + '0');
}

void process(Worm *W, int ch)
{
    int x, y; // Head position
    x = W->head->x;
    y = W->head->y;

    switch(ch) {
    case KEYBOARD_LEFT:
        x--;
        break;
    case KEYBOARD_DOWN:
        y++;
        break;
    case KEYBOARD_UP:
        y--;
        break;
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

    update_board();
}

void crash(Worm *W)
{
    terminate_window();
    printf("\nWell, you ran into something and the game is over.\n");
    printf("Your final score was %d\n\n", W->score);
    exit(0);
}

void setup(void)
{
    /* Revoke setgid privileges */
    setup_window();
    update_score(0);
}
