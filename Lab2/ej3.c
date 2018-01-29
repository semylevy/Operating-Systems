#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    printf("PID of mi programa fork.c = %d\n", getpid());
    pid_t p;
    p = fork();
    if(p == -1) {
        printf("Error al crear un proceso hijo\n");
    } else if(p == 0) {
        printf("Estamoes en el proceso hijo y ejecutamos hello.c\n");
        execl("./hello", "./hello", (char *)0);
    } else {
        printf("Estamos en el proceso padre, con id: %d\n", p);
    }
    return 0;
}