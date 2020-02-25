#include <unistd.h>     /* Symbolic Constants */
#include <sys/types.h>  /* Primitive System Data Types */ 
#include <errno.h>      /* Errors */
#include <stdio.h>      /* Input/Output */
#include <sys/wait.h>   /* Wait for Process Termination */
#include <stdlib.h>     /* General Utilities */
#include <string.h>     /* String utilities */ 

#define TRUE 1

char instruction;


char * read_line(void){
    /*read line stdin*/
    
    char *line = NULL;
    size_t len = 0;
    
    //printf("> ");
 
    getline(&line, &len, stdin);
    //printf("line = %s", line);
    //printf("line length = %zu\n", read);

    //free(line);
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

        //printf( "%s\n", token ); //printing each token
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

char ** get_paths(void){

    int i = 0;
    int len = 64;
    char  **path = malloc(len * sizeof(char*));
    char * PATH = getenv("PATH");

    // Extract the first token
   char * token = strtok(PATH, ":");
   // loop through the string to extract all other tokens
   while( token != NULL ) {
       path[i] = token;
       i++;

       if (i>len)
        {
        len+=64;
        path= realloc(path, len * sizeof(char*));
        }

      token = strtok(NULL,":");
    }
    path[i] = NULL;
    return path;

}

int main() {
    pid_t p;
    int status; /* parent process: child's exit status */
    char *line;
    char **args;
 

  // while(TRUE){ /*repeat forever*/
    
       line = read_line(); /* read line*/
       //printf("line: %s", line);

       
        args = parse_line(line);

        

        // print current working directory
        //printf("pwd: %s\n", getenv("PWD"));
        // print path
        //printf("path: %s\n", getenv("PATH"));

       p = fork();
       if (p <0){
           //printf("Unable to fork");
           //continue;
       }   
       if (p!=0) {
           /*parent code*/
           //printf("PARENT:\n");
           wait(NULL);
           waitpid(-1, &status, 0);  /*wait for child to exit*/
       } else{
           /* child code*/
           //printf("CHILD: \n");
           /* NO execvp*/

        //printf("execve: \n");

        char **paths;
        paths = get_paths();
        char buffer[1000];
        int loop;
        for(loop = 0; loop < 1000; loop++){
            if (paths[loop]!= NULL) {
                strcpy(buffer, paths[loop]);
                strcat(buffer,"/");
                strcat(buffer, args[0]);
                //printf("buffer: %s\n",buffer);
                char *const envParms[2] = {getenv("PATH"), NULL};
                if (access(buffer,X_OK)==0){
                    //printf("existe!!");
                    printf("%d", execve(buffer, args, envParms));
            //         if (execve(buffer, args, envParms) == -1) {
            //         perror("lsh");
            //         }
            //         if (execve(buffer, args, envParms) == -1 ){
            //    printf("Command %s not found!\n", args[0]); 
            // }
        
                }
            }
        } 
        printf("Command %s not found!\n", args[0]);

        // char path[200] = "/bin/";
        // strcat(path, args[0]);

        // char *const envParms[2] = {getenv("PATH"), NULL};
        // printf("%d\n", execve(path, args, envParms));

        
          
        // char path2[] = "/bin/mkdir";
        // char *const parmList[] = {"mkdir", "holaaa", NULL};
        // char *const envParms[2] = {getenv("PATH"), NULL};
	    // printf("intento 2: ");
        // printf("%d\n", execve(path2, parmList, envParms));
           
      }



   //}
   return 0;
}