#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define  MAXLINE 255
static void	sig_int(int);	

int main(void)
{
	char	buf[MAXLINE];	
	pid_t	pid;
	int		status;

	if (signal(SIGINT, sig_int) == SIG_ERR)
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

void sig_int(int signo){
	printf("interrupt%% \n");

}
