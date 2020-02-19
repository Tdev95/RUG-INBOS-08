#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>


#define MAX 50
#define TRUE 1

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
	first = m;
	//close(f);

	for(int i=1; i < num_processes; i++){
		pipe(fd);
		if((pid = fork()) == -1){
			perror("fork failed");
			exit(1);
		} else if(pid == 0) {
			m = fd[1];
			first = m;
		} else {
			f = fd[0];
			// close input pipe
			//close(f);
			break;
		}		
	}
	
	// send first message
	if(getpid() == masterpid){
		if(write(first, &n, 1) == -1){
			perror("write failed");
		}
	}
	
	while(TRUE){
		if(read(f, &n, 1) == -1){
			perror("read failed");
			break;
		}		
		if(n <= 50){
			printf("relative pid=%d: %d\n", getpid() - masterpid, n);
		} else {
			break;
		}
		n++;
		write(m, &n, 1);
	}
	write(m, &n, 1);
	close(m);
	
	return 0;
}
