#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>

#define MAX_LINE 80 /* The maximum length command */

int pipe();

int execute_command(char **args, size_t num, int amp){

    pid_t pid = fork();
    int concu = amp;
    int n = num;

    if(pid < 0){
        fprintf(stderr, "No se pudo hacer el fork!\n");
        return 0;
    
    }else if(pid == 0){ //hijo

        if(0){
            //pipe

        } else {
            int in, out;
            char *infile, *outfile;
            int contO, contI, posO, posI, j;
            int ioAux[] = {0, 0}; //pos 0 in pos 1 out

            contO = 0;
            contI = 0;
            for(size_t i = 0; i < num; i++){
                if(strcmp(args[i], ">") == 0){
            
                    if(i + 1 == (num)){
                        contO = 1;
                        posO = i;
                        printf("No se envio ningun archivo\n");
                        break;
                    } else {

                        ioAux[1] = 1;
                        outfile = args[i + 1];
                        contO = 2;
                        posO = i;
                    }
                
                }

                if(strcmp(args[i], "<") == 0){
            
                    if(i + 1 == (num)){
                        contI = 1;
                        posI = i;
                        printf("No se envio ningun archivo\n");
                        break;
                        
                    } else {
                        ioAux[0] = 1;
                        infile = args[i + 1];
                        contI = 2;
                        posI = i;
                    }
                
                }

            }
            
            if(ioAux[1] == 1){
                out = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 777);
                if(in < 0){
                    printf("No se pudo abrir el archivo %s\n", outfile);
                }else{
                    dup2(out, STDOUT_FILENO);
                }
            }

            if(ioAux[0] == 1){
                in = open(infile, O_RDONLY, 0644);
                if(in < 0){
                    printf("No se pudo abrir el archivo %s\n", infile);
                }else{
                    dup2(in, STDIN_FILENO);
                }
            }

            if(contO > 0){
                for(size_t i = posO, j = 0; j < contO; j++, i++){

                    free(args[i]);
                    args[i] = NULL;
                }
                n -= contO;
            }

            if(contI > 0){
                for(size_t i = posI, j = 0; j < contI; j++, i++){
                    free(args[i]);
                    args[i] = NULL;
                }
                n -= contI;
            }

            
        
            //redirection(args, &num, &ioAux, &infile, &outfile);

            if(execvp(args[0], args)){
                printf("comando invalido\n");
            }

        }

    }else{ //padre
        if(!concu)
            wait(NULL);
    }
    return 1;

}

int main(void){
    // variables
    char *args[MAX_LINE / 2 + 1], *token; 
    char command[MAX_LINE + 1], command_aux[MAX_LINE + 1], copy[MAX_LINE + 1];
    int ampersand;
    size_t n;

    strcpy(command, "");
    for(int i = 0; i != MAX_LINE / 2 + 1; i++){
        args[i] = NULL;
    }


    while(1){

        printf("osh>");
        fflush(stdout);
        fflush(stdin);

        //leer input
        if(fgets(command_aux, MAX_LINE + 1, stdin) == NULL) {
            fprintf(stderr, "Fallo en leer la entrada\n");
            continue;
        }

        //revisar el ultimo comando
        if(strncmp(command_aux, "!!", 2) == 0) {
            if(strlen(command) == 0) {  // no history yet
                fprintf(stderr, "No hay comandos en el historial\n");
                continue;
            }else{
                printf("%s", command);
            }

        }else{
            strcpy(command, command_aux);
        }

        //organizar el input
        n = 0;
        strcpy(copy, command);
        token = strtok(copy, " \n\t\v\r\f");
        while(token != NULL){
            args[n] = malloc(strlen(token) + 1);
            strcpy(args[n], token);
            n++;
            token = strtok(NULL, " \n\t\v\r\f");
        }

        // for(int i = 0; i < n; i++){
        //      printf("%s\n", args[i]);
        // }

        if(n == 0) { // empty input
            printf("escriba un comando!\n");
            continue;
        }

        if(strcmp(args[0], "exit") == 0) {
            break;
        }

        //comprobar si hay un ampersand 
        ampersand = 0; //0 si no hay 1 si si hay
        if(args[n - 1][strlen(args[n - 1]) - 1] == '&'){
            ampersand = 1;
            
            if(strlen(args[n - 1]) == 1){ //lo unico que hay en el argumento es el &
                free(args[n - 1]);
                args[n - 1] = NULL;
                n--;

            }else{
                args[n - 1][strlen(args[n - 1]) - 1] = '\0'; //no es unico que hay en el argumento es el &
            }

        }

        execute_command(args, n, ampersand);
    }
    
    return 0;
}


