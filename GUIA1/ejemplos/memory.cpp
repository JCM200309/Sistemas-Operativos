#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <cstring>

int main(int argc, char *argv[]) {
    char str[128];
    strcpy(str, "Gisela");
    printf("La direccion de str es %p\n", str);
    pid_t pidOrZero = fork();
    if (pidOrZero == 0) { // The child should modify str
        printf("Soy el hijo, la direccion de str es %p\n", str);
        strcpy(str, "Hernan");
        printf("Soy el hijo y cambie el valor de str a %s. La direccion de str es todavia %p\n", str, str);
    } else { // The parent should sleep and print out str
        printf("Soy el padre. La direccion de str es %p\n", str);
        printf("Soy el padre y voy a dormir por 2 segundos.\n");
        sleep(2);
        printf("Soy el padre, recien me desperte, al direccion es %p, y su valor es %s\n", str, str);
    }
    return 0;
}