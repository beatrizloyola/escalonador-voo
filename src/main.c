#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tarefa.h"
#include "escalonador.h"

void fecharBloco(FILE *saida, Tarefa *blocoTarefa, int blocoDuracao, char sufixo){
    if (blocoDuracao == 0){
        return;
    }
    if (blocoTarefa == NULL){
        fprintf(saida, "idle for %d units\n", blocoDuracao);
    } else if (sufixo == '\0'){
        fprintf(saida, "[%s] for %d units\n", blocoTarefa->nome, blocoDuracao);
    } else {
        fprintf(saida, "[%s] for %d units - %c\n", blocoTarefa->nome, blocoDuracao, sufixo);
    }
}


int main(int argc, char *argv[]){
    if (argc != 3){
        fprintf(stderr, "Erro: Quantidade de argumentos inválida\n");
        exit(1);
    }

    if (strcmp(argv[1], "rate") != 0 && strcmp(argv[1], "edf") != 0){
        fprintf(stderr, "Modos aceitos: rate e edf\n");
        exit(1);
    }

    FILE *entrada = fopen(argv[2], "r");
    if (entrada == NULL){
        fprintf(stderr, "Erro: falha ao ler entrada de entrada\n");
        exit(1);
    }

    int TEMPO_TOTAL;

    if (fscanf(entrada, "%d", &TEMPO_TOTAL) != 1){
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

    char nomeentrada[32];
    sprintf(nomeentrada, "%s_blgv.out", argv[1]); // "rate_blgv.out" ou "edf_blgv.out"
    FILE *saida = fopen(nomeentrada, "w");
    if (saida == NULL){
        fprintf(stderr, "Erro: falha ao criar entrada de saída\n");
        exit(1);
    }

    while (1){
        if (qtdTarefas >= maxTarefas){
            fprintf(stderr, "Erro: número de tarefas excede o limite suportado\n");
            exit(1);
        }

        char nomeBuffer[64];
        int periodoBuffer, deadlineBuffer, burstBuffer;

        int retornoScanner= (fscanf(entrada, "%s %d %d %d", nomeBuffer, &periodoBuffer, &deadlineBuffer, &burstBuffer));

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

    Instancia *instancias[qtdTarefas];
    for (int i = 0; i < qtdTarefas; i++){
        instancias[i] = NULL;
    }

    int idle = 0;
    Tarefa *blocoTarefa = NULL;
    int blocoDuracao = 0;
    int blocoAberto = 0;

    fprintf(saida, "EXECUTION BY %s\n", strcmp(argv[1],"rate")==0 ? "RATE" : "EDF");

    for(int t = 0; t < TEMPO_TOTAL; t++){
        // Checar deadline perdido
        for (int i = 0; i < qtdTarefas; i++){
            if (instancias[i] != NULL && t >= instancias[i]->deadlineAbsoluta && instancias[i]->burstRestante > 0){
                tarefas[i]->deadlinesPerdidos++;
                if (blocoAberto && blocoTarefa == tarefas[i]){
                    fecharBloco(saida, blocoTarefa, blocoDuracao, 'L');
                    blocoAberto = 0;
                }
                free(instancias[i]);
                instancias[i] = NULL;
            }
        }


        // Checar chegadas
        for (int i = 0; i < qtdTarefas; i++){
            if (t % tarefas[i]->periodo == 0){
                instancias[i] = criarInstancia(tarefas[i], t);
            }
        }

        // Escolher quem roda
        Instancia *escolhida = escolherProxima(instancias, qtdTarefas, argv[1]);
        Tarefa *entidadeAtual = (escolhida != NULL) ? escolhida->tarefa : NULL;
        if (!blocoAberto){
            blocoTarefa = entidadeAtual;
            blocoDuracao = 0;
            blocoAberto = 1;
        } else if (entidadeAtual != blocoTarefa){
            fecharBloco(saida, blocoTarefa, blocoDuracao, 'H');
            blocoTarefa = entidadeAtual;
            blocoDuracao = 0;
        }
        
        // Executar 1 tick
        blocoDuracao++;
        if (escolhida == NULL){
            idle++;
        } else {
            escolhida->burstRestante--;
            if (escolhida->burstRestante == 0){
                for (int i = 0; i<qtdTarefas; i++){
                    if (instancias[i] == escolhida){
                        tarefas[i]->completos++;
                        instancias[i] = NULL;
                        free(escolhida);
                        break;
                    }
                }
                fecharBloco(saida, blocoTarefa, blocoDuracao, 'F');
                blocoAberto = 0;
            }
        }

    }

    // Encerrar bloco
    if (blocoAberto){
        fecharBloco(saida, blocoTarefa, blocoDuracao, '\0');
    }

    // Checar killed
    for (int i = 0; i < qtdTarefas; i++){
        if (instancias[i] != NULL && instancias[i]->burstRestante > 0){
            free(instancias[i]);
            tarefas[i]->killed++;
            instancias[i] = NULL;
        }
    }

    fprintf(saida, "\nLOST DEADLINES\n");
    for (int i = 0; i < qtdTarefas; i++){
        fprintf(saida, "[%s] %d\n", tarefas[i]->nome, tarefas[i]->deadlinesPerdidos);
    }

    fprintf(saida, "\nCOMPLETE EXECUTION\n");
    for (int i = 0; i < qtdTarefas; i++){
        fprintf(saida, "[%s] %d\n", tarefas[i]->nome, tarefas[i]->completos);
    }

    fprintf(saida, "\nKILLED\n");
    for (int i = 0; i < qtdTarefas; i++){
        fprintf(saida, "[%s] %d\n", tarefas[i]->nome, tarefas[i]->killed);
    }

    fclose(entrada);
    fclose(saida);
    return 0;
}
