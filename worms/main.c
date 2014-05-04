#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#include "main.h"
#include "worm.h"
#include "utils.h"

#define	MAXLINE	    255
#define FIFO_PERMS  (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)


int main()
{
    int server_fifo, length;
    char buffer[MAXLINE], filename[MAXLINE];
    char head, body;

    setup_window();
    prize();
    update_board();
    fflush(stdout);

    // Create server FIFO
    if ((mkfifo(SERVER_FIFO, FIFO_PERMS) < 0) && (errno != EEXIST)){
		printf("can't create %s", SERVER_FIFO);
		exit(EXIT_FAILURE);
	}

    server_fifo = open(SERVER_FIFO, O_RDONLY);

    while(1){
        // Block until server FIFO has new data
        while((read(server_fifo, buffer, MAXLINE)) <= 0);

        sscanf(buffer, "%s %c %c %d", filename, &head, &body, &length);
        test(filename, head, body, length);
    }

    close(server_fifo);
	unlink(SERVER_FIFO);

    return 0;
}

void test(char *filename, char head, char body, int length)
{
    int key, res, client_fd;
    Worm *player = add_player(head, body, length);
    update_board();

    // Notify client init result. 0:Success, else:Failure
    res = (player != NULL) ? 0 : -1;
    client_fd = open(filename, O_WRONLY);
    write(client_fd, &res, sizeof(res));
    close(client_fd);

    while(1){
        // Block until server FIFO has new data
        client_fd = open(filename, O_RDONLY);
        while((read(client_fd, &key, sizeof(key))) <= 0);
        close(client_fd);

        // Process client input
        res = process(player, key);
        fflush(stdout);

        // Send response to the client
        client_fd = open(filename, O_WRONLY);
        write(client_fd, &res, sizeof(res));
        close(client_fd);

        // Player died, no need to continue here
        if (res < 0){
            break;
        }
    }
}

Worm *add_player(char head, char body, int initial_size)
{
    WormBody pos;
    Worm *p = WormCreate(head, body, initial_size);
    if(p == NULL)
        return NULL;

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

int process(Worm *W, int ch)
{
    int res;
    ch = WormMove(W, ch);
    res = W->score;
    if (ch > 0) {
        prize();
    } else if (ch < 0) {
        WormDestroy(W);
        res = -1;
    }

    update_board();
    return res;
}

void crash(Worm *W)
{
    terminate_window();
    printf("\nWell, you ran into something and the game is over.\n");
    printf("Your final score was %d\n\n", W->score);
    exit(0);
}
