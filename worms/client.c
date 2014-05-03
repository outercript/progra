#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#include "utils.h"

#define	MAXLINE	    255
#define FIFO_PERMS  (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

int main(int argc, char **argv)
{
    int is_running = 1;
    int input_char, score, res, n;
    int server_fd, client_fd;
    char client_filename[MAXLINE], buffer[MAXLINE];
    pid_t pid;

    // Verify server FIFO exists
    if (access(SERVER_FIFO, F_OK)) {
        printf("Error: Server is not running\n");
        exit(EXIT_FAILURE);
    }

    // Create client FIFO
    pid = getpid();
    snprintf(client_filename, sizeof(client_filename), CLIENT_FIFO, (long)pid);
    if ((mkfifo(client_filename, FIFO_PERMS) < 0) && (errno != EEXIST)) {
        printf("can't create %s", client_filename);
        exit(EXIT_FAILURE);
    }

    // Connected, process input
    setup_window();
    server_fd = open(SERVER_FIFO, O_WRONLY);

    while (is_running) {
        input_char = capture_keyboard();
        if (input_char == KEYBOARD_EXIT) {
            break;
        }

        // Send request to the server
        sprintf(buffer, "%s %d", client_filename, input_char);
        write(server_fd, buffer, sizeof(buffer));

        // Block until we receive servers response
        client_fd = open(client_filename, O_RDONLY);
        do {
            n = read(client_fd, &res, sizeof(res));
        } while (n <= 0);
        close(client_fd);

        // Check response to know if we still alive
        if (res >= 0) {
            score = res;
        } else {
            break;
        }

        update_score(score);
    }

    // Terminate client gracefully
    terminate_window();
    printf("\nWell, you ran into something and the game is over.\n");
    printf("Your final score was %d\n\n", score);

    close(server_fd);
    unlink(client_filename);
    exit(0);
}
