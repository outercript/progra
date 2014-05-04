#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#include "utils.h"

#define	MAXLINE	    255
#define WORM_LENGHT   7
#define FIFO_PERMS  (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

int main(int argc, char **argv)
{
    int is_running = 1;
    int input_char, score, res, n;
    int server_fd, client_fd;
    char client_filename[MAXLINE], buffer[MAXLINE];
    pid_t pid;

    if (argc == 1){
        n = WORM_LENGHT;
    } else if (argc == 2){
        n = atoi(argv[1]);
    } else {
        printf("Bad usage: %s [initial_length]", argv[0]);
        exit(EXIT_FAILURE);
    }

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

    // Connect with the server <fifoname lenght>
    sprintf(buffer, "%s @ - %d", client_filename, n);
    server_fd = open(SERVER_FIFO, O_WRONLY);
    write(server_fd, buffer, sizeof(buffer));
    close(server_fd);

    // Wait for server response
    client_fd = open(client_filename, O_RDONLY);
    while((read(client_fd, &res, sizeof(res))) <= 0);
    close(client_fd);
    if (res != 0){
        printf("Server error: %d\n", res);
        exit(EXIT_FAILURE);
    }

    // Connected, process input
    setup_window();

    while (is_running) {
        input_char = capture_keyboard();
        if (input_char == KEYBOARD_EXIT) {
            is_running = 0;
        }

        // Send request to the server
        client_fd = open(client_filename, O_WRONLY);
        write(client_fd, &input_char, sizeof(input_char));
        close(client_fd);

        usleep(10*1000);

        // Block until we receive servers response
        client_fd = open(client_filename, O_RDONLY);
        while((read(client_fd, &res, sizeof(res))) <= 0);
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

    unlink(client_filename);
    exit(0);
}
