#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>

#define MAX_LINE 80 /* The maximum length command */

void remove_newLine(char *st){
    size_t n = strlen(st);
    st[n-1] = '\0';
}

void ejecutar(const char *com, char **args){
    
    
}

int main(void){
    
    char args[MAX_LINE], command[MAX_LINE]; /* command line arguments */
    char *all_args[MAX_LINE];
    int should_run = 1; /* flag to determine when to exit program */
    int result, i;

    while (should_run) {
        printf("osh>");
        fflush(stdout);
        fflush(stdin);

        fgets(args, MAX_LINE, stdin);

        if(strncmp(args,"exit", 4) == 0){
            should_run = 0;
            break;
        }

        remove_newLine(args);
        char *aux = strtok(args, " ");
        int ampersand, ampersand_pos;

        i = 0;
        ampersand = 0;
        while(aux != NULL){
            if(i == 0){
                strcpy(command, aux);
            }

            printf("%d\n", strncmp(aux,"&", 1));
            if(strncmp(aux,"&", 1) == 0){
                ampersand = 1;
                ampersand_pos = i;
                aux = strtok(NULL, " ");

            } else {
                all_args[i] = (char*)malloc(MAX_LINE*sizeof(char*));
                strcpy(all_args[i], aux);
                aux = strtok(NULL, " ");
                i += 1;

            }
            
        }
        
        pid_t pid = fork();
        all_args[i] = NULL;

        if (pid < 0) { 
   	        fprintf(stderr, "Fork Failed");
   	        return 1;
        }

        else if (pid == 0) { 
            
            if (execvp(command , all_args) < 0)    
                printf("comando desconocido\n");
    
        }

        else {
            if(ampersand)
   	            wait(NULL);

        }
        
        
        
    }
    return 0;
}


