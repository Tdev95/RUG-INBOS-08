#include <dirent.h>
#include <linux/limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define FMAX 100
#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

char BASE_PATH[PATH_MAX];
int fcnt = 0;
char **flist;
int cmp_names = true;

int filecmp(FILE * fpa, FILE * fpb){
	char ca, cb;
	ca = cb = '\n';
	while((ca != EOF) && (cb != EOF) && ca == cb){
		ca = fgetc(fpa);
		cb = fgetc(fpb);
	}
	return (int) ca - (int) cb;
}

/* compares two named files */
int cmp(const void * a, const void * b){
	char pa[PATH_MAX], pb[PATH_MAX];
	FILE *fpa, *fpb;
	int result = 0;

	int seq = false;
	int beq = false;
	int neq = false;

	int fsz;

	// build file paths
	strcpy(pa, BASE_PATH);
	strcat(pa, "/");
	strcpy(pb, pa);
	strcat(pa, *(char **) a);
	strcat(pb, *(char **) b);
	

	fpa = fopen(pa, "rb");
	fpb = fopen(pb, "rb");
	
	fseek(fpa, 0, SEEK_END);
	fseek(fpb, 0, SEEK_END);

	// compare bytes is size is equal
	if((result=ftell(fpa) - ftell(fpb))==0){
		fsz = ftell(fpa);
		//printf("sz: %d\n", fsz);

		fseek(fpa, 0, SEEK_SET);
		fseek(fpb, 0, SEEK_SET);
		
		seq = true;
		//compare file path if bytes are equal and flag is set
		//if(((result = memcmp(fpa, fpb, fsz))==0)){
		if(((result = filecmp(fpa, fpb))==0)){
			beq = true;
			//printf("R: result %d", result);
			if(cmp_names){
				result = strcmp(pa, pb);
				if(result == 0){
					neq = true;				
				}
			}
		}
	}

	fclose(fpa);
	fclose(fpb);

	// printf("cmp(%s, %s) == %d\n%d %d %d\n", pa, pb, result, seq, beq, neq);
	return result;
}

void bar(){
	for(int i=0; i<fcnt; i++){
		printf("%s\n", flist[i]);	
	}
}

void bar2(){
	if(fcnt){
		printf("\"./%s\"", flist[0]);
		for(int i=0; i<fcnt-1; i++){
			if(cmp(&flist[i], &flist[i+1])){
				printf("\n\"./%s\"", flist[i+1]);
			} else {
				printf(" \"./%s\"", flist[i+1]);
			}
		}
	}
	return;
}


void bar3(){
	int i;
	int flag = false;

	if(fcnt){
		for(int i=0; i<fcnt-1; i++){
			if(cmp(&flist[i], &flist[i+1])){
				if(flag){
					printf(" \"./%s\"\n", flist[i]);
					flag = false;	
				}
			} else {
				if(flag){
					printf(" \"./%s\"", flist[i]);				
				} else {
					printf("\"./%s\"", flist[i]);
					flag = true;
				}
			}
		}
		if(flag){
			printf(" \"./%s\"\n", flist[i]);
		}
	}
	return;
}

// locate files, store in flist
int locate(char *folder){
	DIR *dir;
	struct dirent *dp;
	char path[PATH_MAX], entry[PATH_MAX];
	struct stat fileinfo;	

	// string manipulation to open directory
	strcpy(path, "");
	strcat(path, BASE_PATH);
	if(strcmp(folder, "")){
		strcat(path, "/");
	}
	strcat(path, folder);
	
	if((dir=opendir(path)) == NULL){
		printf("failed to open: %s", path);
		return -1;	
	}

	while((dp = readdir(dir))){
		strcpy(entry, path);
		strcat(entry, "/");
		strcat(entry, dp->d_name);
		// ignore . and .. as entries
		if(!stat(entry, &fileinfo) && strcmp(dp->d_name, ".") && strcmp(dp->d_name, "..")){
			// DIRECTORY: recurse through it
			if(S_ISDIR(fileinfo.st_mode)){
				// exit if name too long
				if(strlen(folder) + strlen("/") + strlen(dp->d_name) >= PATH_MAX){
					printf("name too long, exiting");	
					exit(EXIT_FAILURE);
				}
				
				// string manipulation to create folder argument
				strcpy(path, folder);
				if(strcmp(folder, "")){
					strcat(path, "/");
				}
				strcat(path, dp->d_name);
				locate(path);
			} else
			// FILE: add entry to flist
			if(S_ISREG(fileinfo.st_mode)){

				if(fcnt < FMAX){
					strcpy(flist[fcnt], folder);
					if(strcmp(folder, "")){
						strcat(flist[fcnt], "/");			
					}
					strcat(flist[fcnt], dp->d_name);
					fcnt++;
				}
			}
		}
	}
	closedir(dir);
	return 0;
}

int main (int argc, char **argv) {
	
	// init list of files
	flist = malloc(FMAX * sizeof(char *));
	for(int i=0; i<FMAX; i++){
		flist[i] = malloc(PATH_MAX * sizeof(char));	
	}
	// read folder name
	// printf("target folder: ");
	if(scanf("%s", BASE_PATH) != 1){
		perror("failed to read input");	
	}
	
	// identify files
	if(locate("") == -1){
		perror("invalid directory");	
	}

	// sort files
	qsort(flist, fcnt, sizeof(char *), cmp);
	
	// print result
	cmp_names = false;
	bar3();

	// free memory
	for(int i=0; i<FMAX; i++){
		free(flist[i]);	
	}
	return(0);
}
