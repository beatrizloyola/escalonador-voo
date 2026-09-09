#define _POSIX_C_SOURCE 200809L // pra poder usar strdup

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tarefa.h"

Tarefa *criarTarefa(char *nome, int periodo, int deadline, int burst){
    Tarefa *tarefa = (Tarefa*)malloc(sizeof(Tarefa));
    if (tarefa == NULL){
        fprintf(stderr, "Erro: falha na alocação de memória para tarefa\n");
        exit(1);
    }
    tarefa->nome = strdup(nome);
    tarefa->periodo = periodo;
    tarefa->deadline = deadline;
    tarefa->burst = burst;
    return tarefa;
}