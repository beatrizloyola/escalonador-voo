#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tarefa.h"
#include "escalonador.h"

Instancia *escolherProxima(Instancia **instancias, int qtdTarefas, char *modo){
    if (strcmp(modo, "rate") == 0){
        return modoRate(instancias, qtdTarefas);
    } else {
        return modoEdf(instancias, qtdTarefas);
    }
}

Instancia *modoRate(Instancia **instancias, int qtdTarefas){
    Instancia *menorPeriodo = NULL;

    for (int i = 0; i < qtdTarefas; i++){
        if (instancias[i] == NULL){
            continue;
        }
        if (menorPeriodo == NULL || instancias[i]->tarefa->periodo < menorPeriodo->tarefa->periodo){
            menorPeriodo = instancias[i];
        }
    }

    return menorPeriodo;
}

Instancia *modoEdf(Instancia **instancias, int qtdTarefas){
    // Placeholder só pra compilar msm
    return NULL;
}