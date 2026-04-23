# Guia de desenvolvimento

Este documento reúne as convenções do repositório e o roteiro de evolução planejado. Destina-se a quem quer contribuir com código, scripts ou documentação.

## Estado atual do repositorio

- O solver principal esta em `src/goell_q_solver.cpp`.
- Os utilitarios de reproducao, validacao e plotagem estao em `src/`.
- A documentacao principal esta em `docs/`, com indice em `docs/README.md`.
- As notas permanentes de apoio ficam em `docs/referencias/`.
- `build/`, `out/` e `figures/` sao artefatos gerados localmente.

## Convencoes do projeto

### Nucleo numerico

- O nucleo numerico continua em **C++17** com dependencia principal de Eigen.
- Mudancas no solver devem citar, sempre que possivel, quais equacoes, figuras ou tabelas do paper estao sendo implementadas ou revisadas.
- O foco atual e o metodo de Goell para `det(Q) = 0`, nao modelos de outros artigos nem uma arquitetura de multiplos solvers inexistente neste snapshot.

### Entradas e saidas

- O fluxo principal gera CSVs em `out/` e figuras em `figures/`.
- Os graficos devem nascer dos artefatos numericos produzidos pelo solver e pelos scripts de apoio em `src/`.
- Quando um resultado for importante para a memoria tecnica do projeto, ele deve virar nota em `docs/` ou `docs/referencias/`, em vez de ficar preso apenas a um artefato temporario.

### Documentacao e rastreabilidade

- Ambiguidades de OCR no artigo original devem ser marcadas com `TODO`.
- Divergencias entre reproducao e scan devem ser classificadas como cientificas ou editoriais.
- Se um arquivo em `docs/` for renomeado, movido ou promovido de categoria, atualizar tambem `docs/README.md`, o `README.md` da raiz e os links internos relevantes.

### Verificacao minima

- Recompilar o solver quando houver mudanca em `src/goell_q_solver.cpp`.
- Executar `python3 -m py_compile` nos scripts Python alterados.
- Se a mudanca for numerica, rerodar ao menos o fluxo mais proximo do trecho afetado, como `src/presets.sh`, `src/reproduce_table1.py` ou `src/validate_goell.py`.

## Mapa artigo -> codigo

| Parte do artigo | Arquivos principais |
| --- | --- |
| Secao II - formulacao teorica | `src/goell_q_solver.cpp`, `docs/02_derivacao_das_equacoes.md`, `docs/02.1_efeitos_da_simetria.md` a `docs/02.7_metodo_de_computacao.md` |
| Secao III - resultados | `src/presets.sh`, `src/plot_compare.py`, `docs/03_resultados_do_calculo.md`, `docs/03.1_precisao.md`, `docs/03.2_configuracoes_modais.md`, `docs/03.3_curvas_de_propagacao.md` |
| Tabela I | `src/reproduce_table1.py`, `src/analyze_table1_variation.py`, `docs/referencias/04_notas_sobre_a_tabela_1.md` |
| Conferencia de notacao e OCR | `docs/referencias/03_checklist_de_conferencia.md` |

## Comandos uteis

```bash
mkdir -p build
g++ -O3 -std=c++17 src/goell_q_solver.cpp -I /usr/include/eigen3 -o build/goell_q_solver
bash src/presets.sh fig16 --rebuild
python3 src/reproduce_table1.py
python3 src/analyze_table1_variation.py
python3 src/validate_goell.py --stability-figures fig16 fig17 --det-search sign
```

## Roteiro de evolucao

A sequencia recomendada de proximas contribuicoes, em ordem de prioridade, continua sendo a que esta consolidada em [TODO.md](TODO.md):

1. Revisar `H^{TA}` e o uso de `safe_positive()` nos argumentos fisicos.
2. Rerodar a Tabela I apos esses ajustes.
3. Estabilizar a identificacao modal das Figs. 16-19.
4. So depois atacar as Figs. 20-22 e a nota de reescalonamento da p. 2144.

## O que não fazer cedo demais

- Refatorar a arquitetura do projeto para diretórios que ainda nao existem so por imitacao de outros repositorios.
- Introduzir uma camada nova de build ou de automacao sem necessidade concreta.
- Perseguir fac-simile visual completo antes de fechar ambiguidades cientificas e modais.
- Documentar o repositório com referencias a comandos, scripts ou estruturas inexistentes no snapshot atual.
