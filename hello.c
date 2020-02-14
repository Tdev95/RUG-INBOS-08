#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include <string.h>
#include <errno.h>

#define TRUE 1

int main(int argc, char* arvg[])			{
	char str[100];
	pid_t pid;
	int status;
	//char pathenv[200] = getenv("PATH");
	char paths[10][100];	

	// print current working directory
	printf("%s\n", getenv("PWD"));
	// print path
	printf("%s\n", getenv("PATH"));

	char buffer[200];
	char path[] = "/usr/bin";
	printf("%s\n", path);
	printf("%d\n", execv(path, (char *[]){"mkdir", "test"}));
	printf("%s\n", strerror(errno));
	exit(1);
	/*
	while(TRUE){
		printf(">");
		scanf("%s", str);
		
		pid = fork();
		
		//execute command if child
		if(pid==0){
			strcpy(buffer, getenv("PWD"));
			strcat(buffer, "/mkdir");
			printf("%s", buffer);
			execv(path, (char *[]){"mkdir", "test", NULL});
			// for all paths:
			//		execv(getenv("PATH"), (char *[]){"echo", "test"});
		} else if (pid > 0) {
			wait(NULL);
			// waitpid(pid, status, 0);
		} else {
			//fork failed
			//TODO: give proper error
			;
		}
	}
	*/
	return 0;		}
