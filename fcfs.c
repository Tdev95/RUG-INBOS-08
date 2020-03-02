#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>

#define CPUIO_MAX 20
#define P_MAX 10

struct Process {
	int atime;
	int priority;
	int cpuio[CPUIO_MAX];
};

int readProcess(struct Process *pc){
	int minus = false;
	int n = 0;
	int i = 0;
	char c;
	while(scanf("%c", &c) != EOF){
		if(minus){
			if(c == '1'){
				pc->cpuio[i-2] = -1;
				return 0;
			} else {
				printf("invalid input");
				return -1;
			}
		} else if(isdigit(c)){
			n *= 10;
			n += (int)(c-'0');
		} else if(c == ' '){
			if(i == 0){
				pc->atime = n;
			} else if(i == 1){
				pc->priority = n;
			} else {
				pc->cpuio[i-2] = n;
			}
			n = 0;
			i++;
		} else if(c == '-'){
			minus = true;
		}
	}
	return EOF;
}

int main(int argc, char *argv[]){
	struct Process pcss[P_MAX];
	int n_pcss;

	// read input
	for(n_pcss=0; readProcess(&pcss[n_pcss]) != EOF; n_pcss++){
		;
	}
	

	// print input
	printf("number of processes: %d\n", n_pcss);
	int k;	
	for(int i=0; i<n_pcss; i++){
		for(int j=0; j<CPUIO_MAX; j++){
			k = (&pcss[i])->cpuio[j];
			if(k == -1){
				break;
			}
			printf("%d\n", k);
		}
		printf("\n");
	}
	return 0;
}
