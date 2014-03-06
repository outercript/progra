#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define  MAXLINE 255
static void	sig_usr2(int);	
static void sig_usr1(int);

int main(void)
{
	char	buf[MAXLINE];	
	pid_t	pid;
	int		status;

	if (signal(SIGUSR2, sig_usr2) == SIG_ERR)
		printf("signal error");
	if (signal(SIGUSR1, sig_usr1) == SIG_ERR)
		printf("signal error");


	printf("%% ");	
	while (fgets(buf, MAXLINE, stdin) != NULL) {
		if (buf[strlen(buf) - 1] == '\n')
			buf[strlen(buf) - 1] = 0; 
		pid = fork();
		switch(pid) {
			case -1 : /* error */
				printf("Error\n");
			break;
			case 0 : /* child */
				execlp(buf, buf, (char *)0);
				printf("couldn't execute: %s", buf);
				exit(127);
			break;
			default : /* parent */
				if ((pid = waitpid(pid, &status, 0)) < 0)
					printf("waitpid error");
				printf("%% ");
			break;
		}		
	}
	exit(0);
}

void sig_usr2(int signo){
	printf("SIGUSR2%% \n");
	printf("Sleeping for 10 seconds \n");
	sleep(10);
	printf("Out of SIGUSR2%% \n");
}

void sig_usr1(int signo){
	printf("SIGUSR1%% \n");
	printf("Sleeping for 5 seconds \n");
	sleep(5);
	printf("Out of SIGUSR1%% \n");
}
