#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


#define MAX 50
#define TRUE 1

void safeRead(int fd, void *buf, size_t count){
	if(read(fd, buf, count) == -1){
		perror("write failed");
		exit(1);
	}
}

void safeWrite(int fd, void *buf, size_t count){
	if(write(fd, buf, count) == -1){
		perror("write failed");
		exit(1);
	}
}

int main(int argc, char* argv[]){
	int masterpid, pid;
	int num_processes;
	int fd[2];
	int first;
	int f, m;
	int n = 0;
	// initialize pids
	masterpid = getpid();

	// set stdout not to buffer
	setbuf(stdout, NULL);
	
	// read in number of processes	
	scanf("%d", &num_processes);
	assert(num_processes >= 1 && num_processes <= 16);
	
	// create pipes	
	pipe(fd);
	f = fd[0];
	m = fd[1];
	
	first = m; // entry point first message
	
	int i;
	for(i=1; i < num_processes; i++){
		pipe(fd);
		if((pid = fork()) == -1){
			perror("fork failed");
			exit(1);
		} else if(pid == 0) {
			f = fd[0];
			//first = m;
		} else {
			m = fd[1];
			break;
		}		
	}
	
	// send first message
	if(getpid() == masterpid){
		safeWrite(first, &n, 1);
	}
	
	while(TRUE){
		safeRead(f, &n, 1);		
		if(n <= 50){
			// themis complained about this, so replacing with my own "pid"
			// printf("relative pid=%d: %d\n", getpid() - masterpid, n);
			printf("relative pid=%d: %d\n", i-1, n);
		} else {
			break;
		}
		n++;
		safeWrite(m, &n, 1);
	}
	safeWrite(m, &n, 1);
	close(m); // fd cleanup
	
	// wait to avoid orphan processes
	int status;
	if(i != num_processes){
		wait(&status);
	}
	
	return 0;
}
