#ifndef TAREFA_H
#define TAREFA_H

typedef struct Tarefa{
    char* nome;
    int periodo;
    int deadline;
    int burst;
} Tarefa;

Tarefa *criarTarefa(char *nome, int periodo, int deadline, int burst);

#endif