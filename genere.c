#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, const char *argv[]){
    if(argc != 2){
        printf("Se necesita el numero de direcciones\n");
        return -1;
    }
    int limite = 16777215;
    FILE *fp;
    fp = fopen ("dirs.bin", "w");
    int cantidad = atoi(argv[1]);
    srand(time(0));
    int n = rand() % (limite + 1);
    fprintf(fp, "%d", n);
    for(int i = 0; i< cantidad-1;i++){
        n = rand() % (limite + 1);
        fprintf(fp, "\n%d", n);
    }
    fclose(fp);
}
