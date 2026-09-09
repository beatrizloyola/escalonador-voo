# Ordem de Implementação — Escalonador de Tarefas Críticas de Voo

Backlog incremental. Cada fase constrói sobre a anterior. Marcar `[X]` ao concluir.

## 🧱 Fase 1 — Esqueleto e parsing de argumentos
- [X] Estrutura do projeto: `src/`, `Makefile`, `.gitignore`, `README`
- [X] Parsing de `argc/argv`: `./scheduler [rate|edf] [arquivo.txt]`
- [X] Validação: número correto de argumentos (senão erro + saída)
- [X] Validação: primeiro argumento é exatamente `rate` ou `edf`
- [X] Validação: arquivo de entrada existe e é legível
- [X] Mensagens de erro coerentes via stderr, código de saída ≠ 0, sem criar arquivo de saída

## 📂 Fase 2 — Leitura e validação do arquivo de entrada
- [X] Ler a primeira linha como `TEMPO_TOTAL` (inteiro positivo)
- [X] Ler cada linha seguinte no formato `[NOME] [PERÍODO] [DEADLINE] [BURST]`
- [X] Validar: todos os campos presentes, numéricos e positivos
- [X] Validar: restrição `C ≤ D ≤ P` para cada tarefa (erro se violada)
- [X] Armazenar tarefas em estrutura interna (preservando ordem de chegada no arquivo)

## 🧮 Fase 3 — Núcleo da simulação (loop de tempo)
- [X] Implementar o loop de simulação tick a tick (t = 0 até TEMPO_TOTAL − 1)
- [X] Gerenciar instâncias: a cada tick, verificar quais tarefas têm nova instância chegando (`t % P == 0`)
- [X] Gerenciar deadline absoluto de cada instância (`chegada + D`)
- [X] Implementar detecção de perda de deadline: instância não terminou até seu deadline absoluto → descartar rajada restante, registrar lost deadline, aguardar próxima instância
- [X] Implementar idle: nenhuma tarefa pronta → registrar idle
- [X] Estrutura de dados para fila de prontos (suporte a preempção)

## ✈️ Fase 4 — Algoritmo Rate-Monotonic (rate)
- [ ] Prioridade fixa: menor período → maior prioridade
- [ ] Preempção: nova tarefa de maior prioridade interrompe a atual imediatamente
- [ ] Desempate: tarefa que aparece primeiro no arquivo de entrada vence
- [ ] Gerar saída `rate_<login>.out` com o formato correto (blocos de execução contíguos)
- [ ] Seções do arquivo de saída: `EXECUTION BY RATE`, `LOST DEADLINES`, `COMPLETE EXECUTION`, `KILLED`
- [ ] Conferir saída manualmente com o exemplo `voo.txt` do enunciado

## 🎯 Fase 5 — Algoritmo Earliest-Deadline-First (edf)
- [ ] Prioridade dinâmica: menor deadline absoluto → maior prioridade (recalculada a cada tick)
- [ ] Preempção: nova tarefa com deadline mais próximo interrompe a atual imediatamente
- [ ] Desempate: mesma regra (ordem no arquivo de entrada)
- [ ] Gerar saída `edf_<login>.out` com o mesmo formato de seções
- [ ] Conferir saída manualmente com o exemplo `voo.txt`

## 📄 Fase 6 — Formato de saída e contadores
- [ ] Garantir que blocos contíguos de execução da mesma tarefa são agrupados (não um tick por linha)
- [ ] Sufixos corretos por linha de execução: `F` (finished), `H` (hit deadline, i.e., preempted/interrupted mas terminou a tempo), `L` (lost deadline)
- [ ] Contador `LOST DEADLINES` por tarefa (perdas dentro do tempo de simulação)
- [ ] Contador `COMPLETE EXECUTION` por tarefa (instâncias que terminaram a tempo)
- [ ] Contador `KILLED` por tarefa (instâncias ativas quando a simulação encerra)
- [ ] Nada impresso em stdout durante execução normal

## 🛡️ Fase 7 — Robustez e tratamento de erros
- [ ] Erro: número incorreto de argumentos
- [ ] Erro: primeiro argumento diferente de `rate` ou `edf`
- [ ] Erro: arquivo inexistente ou ilegível
- [ ] Erro: arquivo malformado (campo faltando, valor não numérico, valor não positivo)
- [ ] Erro: tarefa com `D > P` ou `C > D`
- [ ] Em qualquer erro: stderr + exit ≠ 0 + nenhum arquivo `.out` criado
- [ ] Testar casos extremos: 1 tarefa, TEMPO_TOTAL muito curto, tarefa nunca termina

## 🔬 Fase 8 — Testes e análise comparativa
- [ ] Reproduzir exatamente o exemplo do enunciado (`voo.txt`) para `rate` e conferir linha a linha
- [ ] Criar caso de teste onde `rate` perde deadlines e `edf` não (obrigatório para o relatório)
- [ ] Testar com múltiplas tarefas, períodos variados, deadline justo
- [ ] Testar todos os casos de erro listados na Fase 7
- [ ] Registrar evidências (saídas, diffs) para o relatório

## 📝 Fase 9 — README e Makefile
- [ ] Makefile: `make` sem alvo gera executável `scheduler`; `make clean` remove compilados
- [ ] README: lista todos os arquivos `.c`, como compilar, como executar, como testar, SO utilizado
- [ ] Confirmar que `make` funciona em ambiente limpo

## 📦 Fase 10 — Empacotamento e entrega
- [ ] Commits atômicos no repositório
- [ ] Relatório em PDF seguindo o Guia de Relatórios da disciplina (dificuldades, limitações, evidências)
- [ ] Análise comparativa obrigatória no relatório: caso em que rate perde e edf não, com explicação
- [ ] Diretório nomeado com o login (iniciais do e-mail em minúsculas)
- [ ] Conferir nomes finais: `login.pdf`, `login.tar`, diretório `login/`