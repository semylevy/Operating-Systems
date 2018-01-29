#include <stdio.h>
#include <stdlib.h>

int main() {
    int salida;
    char command[100];
    printf("Ejemplo de la llamada system \n");
    sprintf(command, "/bin/ps -fu %s", getenv("USER"));
    salida = system(command);
    printf("Salida del comando: %d\n", salida);
    exit(0);
}