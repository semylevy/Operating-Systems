#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int salida;
    printf("Ejemplo de exec \n");
    printf("aqui %s\n", getenv("USER"));
    salida = execl("/bin/ps", "/bin/ps", "-fu", getenv("USER"), NULL);
    printf("Salida del comando: %d\n", salida);
    exit(0);
}