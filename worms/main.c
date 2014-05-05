#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include "main.h"
#include "worm.h"
#include "utils.h"


#define FIFO_PERMS  (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)


int main()
{
    int server_fifo, length;
    char buffer[MAXLINE], filename[MAXLINE];
    char head, body;
    pthread_t thread;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    // Create server FIFO
    if ((mkfifo(SERVER_FIFO, FIFO_PERMS) < 0) && (errno != EEXIST)) {
        printf("Unable to create: %s", SERVER_FIFO);
        exit(EXIT_FAILURE);
    }

    // Create the game board
    setup_window();
    prize();
    update_board();

    server_fifo = open(SERVER_FIFO, O_RDONLY);
    while(1) {
        // Block until server FIFO has new data
        while((read(server_fifo, buffer, MAXLINE)) <= 0);

        // Create a new thread to monitor a new player
        sscanf(buffer, "%s %c %c %d", filename, &head, &body, &length);
        Player *p = PlayerCreate(filename, head, body, length);
        pthread_create(&thread, &attr, PlayerMonitor, p);
    }

    // Cleanup
    terminate_window();
    close(server_fifo);
    unlink(SERVER_FIFO);

    return 0;
}


void *PlayerMonitor(void *player)
{
    Player *p = (Player*) player;
    int key, res, score, client_fd;

    // Notify client init result. 0:Success, else:Failure
    res = (p != NULL) ? 0 : -1;
    client_fd = open(p->filename, O_WRONLY);
    write(client_fd, &res, sizeof(res));
    close(client_fd);

    if (p == NULL)
        pthread_exit(NULL);

    while(1) {
        // Block until server FIFO has new data
        client_fd = open(p->filename, O_RDONLY);
        while((read(client_fd, &key, sizeof(key))) <= 0);
        close(client_fd);

        // Process client input (critical section)
        if ((res = WormMove(p->worm, key)) > 0)
            prize();
        score = (res >= 0) ? p->worm->score: -1;
        update_board();

        // Send response to the client
        client_fd = open(p->filename, O_WRONLY);
        write(client_fd, &score, sizeof(score));
        close(client_fd);

        // Player died, nothing to do here. GG
        if (res < 0)
            break;
    }

    PlayerDestroy(p);
    pthread_exit(NULL);
}


Player *PlayerCreate(char *filename, char head, char body, int length)
{
    WormBody pos;
    Player *p = malloc(sizeof(Player));
    if (p == NULL)
        return  NULL;

    // Copy the buffer to the player (assume no overflow)
    strncpy(p->filename, filename, sizeof(p->filename));

    // Find an empty location (infinite loop if no empty places)
    do {
        newpos(&pos);
    } while (!(checkpos(pos.x, pos.y) == ALL_EMPTY));

    // Create a worm for the player
    p->worm = WormCreate(head, body, length, pos);
    if (p->worm == NULL) {
        free(p->filename);
        free(p);
        return NULL;
    }

    update_board();
    return p;
}

void PlayerDestroy(Player *p)
{
    WormDestroy(p->worm);
    free(p->filename);
    update_board();
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
