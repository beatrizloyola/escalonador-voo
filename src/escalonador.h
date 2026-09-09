#ifndef ESCALONADOR_H
#define ESCALONADOR_H

#include "tarefa.h"

Instancia *escolherProxima(Instancia **instancias, int qtdTarefas, char *modo);
Instancia *modoRate(Instancia **instancias, int qtdTarefas);
Instancia *modoEdf(Instancia **instancias, int qtdTarefas);

#endif