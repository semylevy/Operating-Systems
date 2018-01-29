#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include<sys/wait.h>
#include <string.h>

#define MAX_SIZE 100
#define MAX_ARGS 3

int getCmdNumber(char *str);
void printCmds();
void runCmd(char *input, pid_t p, int arg_count, char **args);
void tooManyArgs();

int main() {
    char *input [MAX_ARGS];
    for(int i = 0; i < MAX_ARGS; i++) input[i] = (char *)malloc(MAX_SIZE);
    char arg[MAX_SIZE];
    char exit[MAX_SIZE];
    strcpy(exit, "salir");

    do {
        int arg_count = 0;
        char tmp;
        printf("SHELL_SLB>$ ");
        do {
            if(arg_count >= MAX_ARGS) {
                tooManyArgs();
                arg_count--;
            }
            scanf("%s%c", input[arg_count], &tmp);
            arg_count++;
        } while (tmp != '\n');
        pid_t p;
        switch(getCmdNumber(input[0])) {
            case -1:
                printf("No existe una acción para %s. Comandos disponibles:\n", input[0]);
                printCmds();
                break;
            case 0:
                runCmd("ls", p, 0, input);
                break;
            case 1:
                runCmd("date", p, 0, input);
                break;
            case 2:
                runCmd("mv", p, 2, input);
                break;
            case 3:
                runCmd("rm", p, 1, input);
                break;
            case 4:
                runCmd("touch", p, 1, input);
            default:
                break;
        }
    } while(strcmp(exit, input[0]));
    return 1;
}

int getCmdNumber(char *str) {
    if(!strcmp(str, "listar")) {
        return 0;
    } else if(!strcmp(str, "fecha")) {
        return 1;
    } else if(!strcmp(str, "renombrar")) {
        return 2;
    } else if(!strcmp(str, "borrar")) {
        return 3;
    } else if(!strcmp(str, "crear")) {
        return 4;
    }
    else {
        return -1;
    }
    return -1;
}

void printCmds() {
    printf("listar\n");
    printf("fecha\n");
    printf("renombrar\n");
    printf("borrar\n");
    printf("crear\n");
}

void runCmd(char *input, pid_t p, int arg_count, char **args) {
    p = fork();
    char cmd[MAX_SIZE];
    strcpy(cmd, "/bin/");
    strcat(cmd, input);
    if(p == 0) {
        if(arg_count > 0) {
            args[arg_count+1] = NULL;
            execvp(cmd, args);
        }
        else {
            execl(cmd, cmd, NULL);
        }
    } else {
        p = wait(NULL);
        return;
    }
}

void tooManyArgs() {
    printf("--- Cuidado con esos argumentos, podrías romper algo. ---\n");
}