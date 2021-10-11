#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// Jose Pablo Fernández Cubillo
// Roberto Vidal Patiño

int cantidadDigitos (int n) {
    int r = 1;
    while (n > 9) {
        n /= 10;
        r++;
    }
    return r;
}

int main(int argc, const char *argv[]){
    if(argc != 2){
        printf("Se necesita el número como argumento\n");
        return -1;
    }
    int tamannoBytes = 24;
    int limite = 0;
    for(int i =0; i < tamannoBytes; i++){
        limite += pow(2, i);
    }
    printf("%d\n", limite);
    FILE *fp;
    fp = fopen ("dirs.bin", "w");
    int cantidad = atoi(argv[1]);
    srand(time(0));
    int n;
    int sumaTamanno = 0;
    int cantDigitos;
    int limiteDigitos = cantidadDigitos(limite);
    while(sumaTamanno < cantidad){
        if(cantidad - sumaTamanno <= limiteDigitos){
            for(int i = 0; i < cantidad - sumaTamanno; i++){
                n = rand() % 10;
                fprintf(fp, "%d", n);
            }
            sumaTamanno = cantidad;
        }
        else{
            n = rand() % (limite + 1);
            cantDigitos = cantidadDigitos(n);
            fprintf(fp, "%d\n", n);
            sumaTamanno += cantDigitos + 1;
        }
    }
    fclose(fp);
}
