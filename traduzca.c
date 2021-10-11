#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct estructuraTLB{
    int logica;
    int fisica;
};

struct estructuraTLB *tlb;
int iTlb = 0;
int tamTlb;

int *tablaPaginas;
int tamTablaPaginas;

int tamPagina;

int *memoria;

signed char  *memoriaFisica;

void falloDePagina(const char *nArchivo, int paginaFisica, int paginaLogica, int offset){
    FILE *archivo = fopen(nArchivo,"rb");

    if (archivo == NULL){
        printf("No se pudo abrir %s\n", nArchivo); 
        return;
    }

    if (fseek(archivo, tamPagina*paginaLogica, SEEK_SET) != 0){
        printf("No se encontró la página\n");
        return;
    }

    unsigned char dato[256];
    fread(dato,sizeof(dato),1,archivo);

    for(int i = 0; i < 256; i++){
        memoriaFisica[paginaFisica*tamPagina+i] = dato[i];
    }
    tablaPaginas[paginaLogica] = paginaFisica;

    fclose(archivo);
}

void meteEnTlb(int logica, int fisica) {
    struct estructuraTLB *t = &tlb[iTlb % tamTlb];
    t->logica = logica;
    t->fisica = fisica;
    iTlb++;
}

int buscaEnTlb(int paginaLogica){
    int i = 0;
    if(iTlb - tamTlb > 0){
        i = iTlb - tamTlb;
    }
    for(; i < iTlb; i++){
        struct estructuraTLB *t = &tlb[i % tamTlb];
        if(t->logica == paginaLogica){
            return t->fisica;
        }
    }
    return -1;
}

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
    if(n % 8 != 0){
        printf("El argumento 3 (n) debe ser múltiplo de 8\n");
        return -1;
    }
    if(m + k > n){
        printf("El argumento 4 (m) + argumento 5 (k) debe ser menor o igual que el argumento 3 (n)\n");
        return -1;
    }
    if(n <= 0 || m <= 0 || k <= 0 || l <= 0){
        printf("Todos los argumentos números deben ser mayores que 0\n");
        return -1;
    }

    int totalDirecciones = 0;
    int direccionLogica = 0;
    int paginaLogica = 0;
    int direccionFisica = 0;
    int offset = 0;
    signed char valor = 0;
    int paginaFisica = 0;
    int tlbHits = 0;
    int fallosDePagina = 0;
    int paginaLibre = 0;

    int mascaraOffset = 0;
    int mascaraDireccion = 0;

    tamPagina = (int)pow(2,k);
    tamTablaPaginas = pow(2,m);
    tamTlb = pow(2,l);
    tablaPaginas = malloc(tamTablaPaginas * sizeof(int));
    tlb = malloc(tamTlb * sizeof(struct estructuraTLB));
    memoriaFisica = malloc(tamTablaPaginas * tamPagina * sizeof(int));

    for (int i = 0; i < tamTablaPaginas; i++) {
        tablaPaginas[i] = -1;
    }

    for(int i = 0; i < k; i++){
        mascaraOffset |= 1 << i;
    }

    for(int i = 0; i < m; i++){
        mascaraDireccion |= 1 << i;
    }

    FILE* direcciones = fopen(argv[1], "rb");

    char dir[256];
    while (fgets(dir, 256, direcciones)) {
        totalDirecciones++;
        direccionLogica = atoi(dir);
        offset = direccionLogica & mascaraOffset;
        paginaLogica = (direccionLogica >> k) & mascaraDireccion;
        
        paginaFisica = buscaEnTlb(paginaLogica);

        if(paginaFisica == -1){
            paginaFisica = tablaPaginas[paginaLogica];
            
            if (paginaFisica == -1) {
                fallosDePagina++;
                
                paginaFisica = paginaLibre;
                paginaLibre++;
                falloDePagina(argv[2], paginaFisica, paginaLogica, offset);
            }
            
            meteEnTlb(paginaLogica, paginaFisica);
        } else {
            tlbHits++;
        }
        direccionFisica = (paginaFisica << k) | offset;
        valor = memoriaFisica[paginaFisica * tamPagina + offset];
        printf("Direccion Logica: %d, Direccion Fisica: %d, Valor: %d\n", direccionLogica, direccionFisica, valor);
    }

    printf("Número de direcciones traducidas: %d\n", totalDirecciones);
    printf("Fallos de página: %d\n", fallosDePagina);
    printf("Índice de fallos de página: %f\n", fallosDePagina / (1. * totalDirecciones));
    printf("TLB hits: %d\n", tlbHits);
    printf("Índice de TLB hit: %f\n", tlbHits / (1. * totalDirecciones));
    
    free(tablaPaginas);
    free(tlb);

    fclose(direcciones);
}