#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


#define MAX_LENGTH 255
#define SHELL_PROMPT "%% "

int main(void)
{
    char command[MAX_LENGTH];
    pid_t pid;
    int status;

    printf(SHELL_PROMPT);
    while (fgets(command, MAX_LENGTH, stdin) != NULL) {

        // Strip newline char at the end of the command
        if (command[strlen(command) - 1] == '\n')
            command[strlen(command) - 1] = 0;

        pid = fork();
        switch(pid) {

        case -1 : // Fork failed
            printf("Error\n");
            break;

        case 0 : // Code executed by child
            // Use sh to execute external commands, otherwise we would need to
            // parse the buffer and create a *char[] with all the passed args
            execlp("sh", "sh", "-c", command, (char *) NULL);

            // This code is executed only when execlp fails
            printf("Unable to execute: %s", command);
            exit(EXIT_FAILURE);
            break;

        default : // Code executed by parent
            // Wait for the subprocess to be terminated
            if ((pid = waitpid(pid, &status, 0)) < 0)
                printf("waitpid error");

            // Print the return code of the child
            if(WIFEXITED(status))
                printf("Process's exit code: %d\n", WEXITSTATUS(status));

            // Show the prompt again, so it appears before the user types again
            printf(SHELL_PROMPT);
            break;
        }
    }
    exit(EXIT_SUCCESS);
}
