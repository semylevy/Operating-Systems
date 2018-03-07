#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    pid_t cf;
    int var = 10;
    cf = fork();

    switch(cf) {
        case -1:
            printf("No he podido crear el proceso hijo\n");
            break;
        case 0:
            for(int i = 0; i < 10; i++) var++;
            printf("Soy el hijo, mi PID es %d y mi PPID %d\n", getpid(), getppid());
            sleep(20);
            break;
        default:
            for(int i = 0; i < 10; i++) var+=10;
            printf("Soy el padre, mi PID es %d y el PID de mi hijo es %d\n", getpid(), cf);
            sleep(30);
            break;
    }
    printf("Final de ejecución de %d, var vale: %d\n", getpid(), var);
    exit(0);
}