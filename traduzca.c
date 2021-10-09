#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main(int argc, const char *argv[]){
    if (argc != 7) {
        printf("Se necesitan 6 argumentos %d\n", argc);
        return -1;
    }
    int n, m, k, l;
    n = atoi(argv[3]);
    m = atoi(argv[4]);
    k = atoi(argv[5]);
    l = atoi(argv[6]);
    printf("n = %d, m = %d, k = %d, l = %d\n", n, m, k, l);
    int mascaraOffset = 0;
    for(int i =0; i<k; i++){
        mascaraOffset += pow(2,i);;
    }
    printf("a: %d\n", mascaraOffset);
    int mascaraDireccion = 0;
    for(int i =0; i<m; i++){
        mascaraDireccion += pow(2,i);
    }
    FILE* file = fopen(argv[1], "r");
    char line[256];
    int direccionLogica, direccionFisica, offset;
    while (fgets(line, sizeof(line), file)) {
        direccionLogica = atoi(line);
        offset = direccionLogica & mascaraOffset;
        direccionFisica = (direccionLogica >> k) & mascaraDireccion;
        printf("Direccion Logica: %d, Direccion Fisica: %d, Offset: %d\n", direccionLogica, direccionFisica, offset);
    }
}
