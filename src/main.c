#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
    if (argc != 3){
        fprintf(stderr, "Erro: Quantidade de argumentos inválida\n");
        exit(1);
    }

    if (strcmp(argv[1], "rate") != 0 && strcmp(argv[1], "edf") != 0){
        fprintf(stderr, "Modos aceitos: rate e edf\n");
        exit(1);
    }

    FILE *arquivo = fopen(argv[2], "r");
    if (arquivo == NULL){
        fprintf(stderr, "Erro: falha ao ler arquivo de entrada\n");
        exit(1);
    }

    fclose(arquivo);
    return 0;
}
