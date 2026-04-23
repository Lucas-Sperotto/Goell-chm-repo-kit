# TODO

Pendencias tecnicas priorizadas depois do fechamento inicial da pasta `docs/`.

## Alta prioridade

- Revisar o bloco `H^{TA}` em `src/goell_q_solver.cpp` e conferir a forma escalada contra [docs/referencias/02_matriz_global_e_normalizacao.md](docs/referencias/02_matriz_global_e_normalizacao.md).
- Separar a protecao numerica de `safe_positive()` da avaliacao fisica dos argumentos `hr` e `pr`.
- Reexecutar a Tabela I apos esses dois ajustes e comparar o resultado com [docs/referencias/04_notas_sobre_a_tabela_1.md](docs/referencias/04_notas_sobre_a_tabela_1.md).

## Validacao modal

- Estabilizar a identificacao do ramo fisico correto nas Figs. 16-19.
- Revisar o setor `even`, que continua mais sensivel a selecao de pontos e de raiz.
- Consolidar quando usar `n_r = 1.01` e quando usar `n_r = 1.0001`, evitando comparacoes cruzadas entre regimes diferentes.

## Depois de estabilizar o nucleo

- Voltar as Figs. 20-22.
- Reavaliar a nota de reescalonamento da p. 2144 do paper.
- Verificar se a reproducao da Tabela I pode deixar de escolher a raiz "mais proxima do paper" e passar a servir como validacao mais independente.
