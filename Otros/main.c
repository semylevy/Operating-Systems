#include <stdio.h>
#include <stdlib.h>

int main() {
    int *var;
    int entero;
    var = (int*)malloc(sizeof(*var)*2);
    *var = 1;
    *(var+1) = 2;
    printf("%p", var);
    free(var);
    return 1;
}
