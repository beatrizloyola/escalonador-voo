#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tarefa.h"

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

    int TEMPO_TOTAL;

    if (fscanf(arquivo, "%d", &TEMPO_TOTAL) != 1){
        fprintf(stderr, "Erro: tempo total inválido ou ausente\n");
        exit(1);
    }

    if (TEMPO_TOTAL <= 0){
        fprintf(stderr, "Erro: tempo total não é inteiro positivo\n");
        exit(1);
    }

    int qtdTarefas = 0;
    int maxTarefas = 100;
    Tarefa *tarefas[maxTarefas];

    while (1){
        if (qtdTarefas >= maxTarefas){
            fprintf(stderr, "Erro: número de tarefas excede o limite suportado\n");
            exit(1);
        }

        char nomeBuffer[64];
        int periodoBuffer, deadlineBuffer, burstBuffer;

        int retornoScanner= (fscanf(arquivo, "%s %d %d %d", nomeBuffer, &periodoBuffer, &deadlineBuffer, &burstBuffer));

        if (retornoScanner == EOF){
            break;
        }

        if (retornoScanner != 4){
            fprintf(stderr, "Erro: linha mal formada\n");
            exit(1);
        }

        if (deadlineBuffer <= 0){
            fprintf(stderr, "Erro: deadline não é inteiro positivo\n");
            exit(1);
        }

        if (burstBuffer <= 0){
            fprintf(stderr, "Erro: burst não é inteiro positivo\n");
            exit(1);
        }

        if (periodoBuffer <= 0){
            fprintf(stderr, "Erro: período não é inteiro positivo\n");
            exit(1);
        }

        if (burstBuffer > deadlineBuffer || deadlineBuffer > periodoBuffer){
            fprintf(stderr, "Erro: é esperado que burst <= deadline <= periodo\n");
            exit(1);
        }

        tarefas[qtdTarefas] = criarTarefa(nomeBuffer, periodoBuffer, deadlineBuffer, burstBuffer);
        qtdTarefas = qtdTarefas + 1;
    }


    fclose(arquivo);
    return 0;
}
