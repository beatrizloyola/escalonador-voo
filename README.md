# Escalonador de Tarefas Críticas de Voo

Simulador de escalonamento de tarefas periódicas críticas (computador de bordo de aeronave),
comparando os algoritmos **rate-monotonic** (`rate`) e **earliest-deadline-first** (`edf`).

## Arquivos `.c`

- `src/main.c` — parsing de argumentos, leitura/validação do arquivo de entrada, loop de
  simulação tick a tick, geração do arquivo de saída.
- `src/tarefa.c` — construtores de `Tarefa` (definição estática de uma tarefa) e `Instancia`
  (instância concreta de uma tarefa em execução, com deadline absoluto e rajada restante).
- `src/escalonador.c` — lógica de escolha da próxima tarefa a rodar: `modoRate` (prioridade por
  menor período) e `modoEdf` (prioridade por menor deadline absoluto).

Headers correspondentes: `src/tarefa.h`, `src/escalonador.h`.

## Como compilar

```
make
```

Gera o executável `scheduler` na raiz do projeto.

Para limpar os binários compilados:

```
make clean
```

## Como executar

```
./scheduler rate arquivo.txt
./scheduler edf arquivo.txt
```

O arquivo de entrada segue o formato:

```
TEMPO_TOTAL
NOME PERIODO DEADLINE BURST
NOME PERIODO DEADLINE BURST
...
```

O resultado é gravado em `rate_blgv.out` ou `edf_blgv.out` (conforme o modo escolhido). Nada é
impresso em stdout durante a execução normal; erros de entrada são reportados em stderr com
código de saída diferente de zero, e nenhum arquivo de saída é criado nesse caso.

## Como testar

Um caso de exemplo (`voo.txt`, do enunciado) valida a saída linha a linha:

```
100
ATT 20 12 8
NAV 50 30 15
```

```
./scheduler rate voo.txt
cat rate_blgv.out
```

Casos de erro a testar manualmente: número incorreto de argumentos, modo diferente de
`rate`/`edf`, arquivo inexistente, arquivo malformado (campo faltando, valor não numérico ou
não positivo), tarefa com `D > P` ou `C > D`. Em todos, o programa deve escrever em stderr,
encerrar com código ≠ 0 e não criar arquivo `.out`.

## Sistema operacional

Desenvolvido e testado em Linux (Ubuntu 24.04, kernel 7.0.0-28-generic), compilado com `gcc`
(`-std=c11 -Wall -Wextra`).
