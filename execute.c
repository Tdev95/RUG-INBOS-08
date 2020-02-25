#include <unistd.h>     /* Symbolic Constants */
#include <sys/types.h>  /* Primitive System Data Types */ 
#include <errno.h>      /* Errors */
#include <stdio.h>      /* Input/Output */
#include <sys/wait.h>   /* Wait for Process Termination */
#include <stdlib.h>     /* General Utilities */
#include <string.h>     /* String utilities */ 

#define TRUE 1

char * read_line(void){
    /*read line stdin*/
    
    char *line = NULL;
    size_t len = 0;
    
    //printf("> ");
 
    getline(&line, &len, stdin);
    return line;
    
}

char ** parse_line(char *line){

    int i = 0;
    int len = 64;
    char  **array = malloc(len * sizeof(char*));
    char seps[]= " \t\r\n\a";

    // Extract the first token
   char * token = strtok(line, seps);
   // loop through the string to extract all other tokens
   while( token != NULL ) {
       array[i] = token;
       i++;

       if (i>len)
        {
        len+=64;
        array = realloc(array, len * sizeof(char*));
        }

      token = strtok(NULL,seps);
    }
    array[i] = NULL;
    return array;

}


int main() {
    pid_t p;
    int status; /* parent process: child's exit status */
    char *line;
    char **args;
    int cmd;


   //while(TRUE){ /*repeat forever*/
    
       line = read_line(); /* read line*/
        args = parse_line(line);

       p = fork();
       if (p <0){
           //printf("Unable to fork");
           //continue;
       }   
       if (p!=0) {
           /*parent code*/
           waitpid(-1, &status, 0);  /*wait for child to exit*/
       } else{
           /* child code*/

        char path[200] = "/bin/";
        strcat(path, args[0]);
        char *const envParms[2] = {getenv("PATH"), NULL};

        if (execve(path, args, envParms) == -1 ){
            printf("Command %s not found!\n", args[0]); 
        }
           
      }



   //}
   return 0;
}