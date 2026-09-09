#ifndef TAREFA_H
#define TAREFA_H

typedef struct Tarefa{
    char* nome;
    int periodo;
    int deadline;
    int burst;

    int deadlinesPerdidos;
    int completos;
    int killed;
} Tarefa;

typedef struct Instancia{
    Tarefa *tarefa;
    int chegada; // t em que nasceu
    int deadlineAbsoluta; // chegada + deadline
    int burstRestante;
    int foiPreemptada;
} Instancia;

Tarefa *criarTarefa(char *nome, int periodo, int deadline, int burst);
Instancia *criarInstancia(Tarefa *tarefa, int chegada);

#endif