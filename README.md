# Goell 1969 - Reproducao, Estudo E Implementacao

Este repositorio existe para estudar e reproduzir numericamente o artigo:

J. E. Goell, *A Circular-Harmonic Computer Analysis of Rectangular Dielectric Waveguides*, *Bell System Technical Journal*, setembro de 1969.

Ele foi organizado para cumprir tres papeis ao mesmo tempo:

- servir como guia de estudo em portugues;
- concentrar uma implementacao executavel do metodo;
- registrar as tentativas de validacao contra tabelas e figuras do artigo.

## Visao Geral

O metodo de Goell parte de uma ideia elegante: em vez de descrever o guia retangular por solucoes cartesianas em varias regioes, ele expande os campos em harmonicos circulares e faz o casamento dos campos tangenciais ao longo da fronteira do nucleo.

Em termos de fluxo de trabalho, isso significa:

1. expandir `E_z` e `H_z` em series com funcoes de Bessel no interior e funcoes de Bessel modificadas no exterior;
2. reconstruir os campos transversais a partir das equacoes de Maxwell;
3. escolher classes modais usando simetria;
4. impor o casamento em pontos da fronteira;
5. montar a matriz global `Q`;
6. procurar os parametros para os quais `det(Q) = 0`.

Este repositorio foi montado justamente para tornar esse caminho legivel, verificavel e reproduzivel.

## Estrutura Do Repositorio

### Codigo

- [src/goell_q_solver.cpp](src/goell_q_solver.cpp): solver principal, com montagem da matriz `Q`, busca de raizes e exportacao das curvas.
- [src/presets.sh](src/presets.sh): execucoes padrao para figuras e rodadas de validacao.
- [src/plot_compare.py](src/plot_compare.py): plotador dos CSVs produzidos pelo solver.
- [src/reproduce_table1.py](src/reproduce_table1.py): reproducao automatizada da Tabela I.
- [src/analyze_table1_variation.py](src/analyze_table1_variation.py): diagnostico das fontes de variacao residual da Tabela I.
- [src/validate_goell.py](src/validate_goell.py): utilitarios de validacao por estabilidade em `N`.
- [src/track_roots.py](src/track_roots.py): rastreamento de ramos por continuidade.

### Documentacao

Os arquivos em `docs/` formam uma trilha de estudo do artigo.

- [docs/00_resumo.md](docs/00_resumo.md): mapa geral do problema, do metodo e do papel do repositorio.
- [docs/01_intro.md](docs/01_intro.md): leitura comentada da introducao.
- [docs/02_deriv.md](docs/02_deriv.md): derivacao guiada da teoria, com comentarios fisicos e numericos.
- [docs/03_result.md](docs/03_result.md): leitura da secao de resultados, com destaque para Tabela I e Figs. 16-22.
- [docs/04_conclusions.md](docs/04_conclusions.md): conclusoes do artigo com comentario critico.

Como referencia rapida:

- [docs/goell_01_field_expansions.md](docs/goell_01_field_expansions.md)
- [docs/goell_02_matrix_and_normalization.md](docs/goell_02_matrix_and_normalization.md)
- [docs/goell_03_review_checklist.md](docs/goell_03_review_checklist.md)
- [docs/table1_variation_notes.md](docs/table1_variation_notes.md)

### PDF De Referencia

- [docs/j.1538-7305.1969.tb01168.x.pdf](docs/j.1538-7305.1969.tb01168.x.pdf)

## Estado Atual Da Implementacao

O solver foi reorganizado para refletir a classificacao correta do artigo:

- paridade dos harmonicos: `odd` e `even`;
- familia de fase: `phi0` e `phi90`.

Isso gera as quatro classes modais tratadas explicitamente no codigo:

- `odd / phi0`
- `odd / phi90`
- `even / phi0`
- `even / phi90`

Tambem ja existe suporte a duas variantes importantes de busca:

- busca por minimos de uma metrica escalar;
- busca por mudanca de sinal de `det(Q)`, mais proxima do procedimento descrito no paper.

## O Que Ja Esta Bem Encaminhado

- a estrutura em quatro classes modais;
- a montagem geral da matriz `Q`;
- a interpretacao da fronteira por intersecao geometrica;
- a reproducao automatizada da Tabela I;
- o fluxo de geracao das figuras principais;
- a documentacao das equacoes e da narrativa fisica do artigo.

## O Que Ainda Esta Em Aberto

- identificacao robusta do ramo fisico correto quando ha varias raizes proximas;
- tratamento fino do setor `even`, que continua mais sensivel;
- alinhamento visual e modal das Figs. 16-22 com o artigo;
- interpretacao final da nota de reescalonamento da p. 2144.

Em outras palavras: o repositorio ja esta num ponto em que a dificuldade principal nao e mais "montar alguma versao do metodo", e sim "fazer a reproducao numerica convergir para os mesmos ramos do artigo".

## Como Compilar

Dependencias principais:

- `g++` com suporte a C++17
- Eigen
- Python 3
- `matplotlib`

Compilacao manual:

```bash
mkdir -p build
g++ -O3 -std=c++17 src/goell_q_solver.cpp -I /usr/include/eigen3 -o build/goell_q_solver
```

## Como Rodar

Exemplos com os presets principais:

```bash
bash src/presets.sh fig16
bash src/presets.sh fig17
bash src/presets.sh fig18
bash src/presets.sh fig19
```

Os CSVs sao escritos em `out/` e as figuras em `figures/`.

Tambem e possivel sobrescrever parametros por variaveis de ambiente:

```bash
N=7 NB=80 PSCAN=320 bash src/presets.sh fig17
OUT_PREFIX=fig16_det METRIC=det DET_SEARCH=sign bash src/presets.sh fig16
GEOMETRY=intersection NR=1.0001 bash src/presets.sh fig17
```

## Colunas Exportadas Pelo Solver

Os CSVs produzidos pelo solver usam, em geral:

- `branch_id`
- `B`
- `Pprime`
- `merit`
- `parity`
- `phase`
- `geometry`

Observacao importante: no estado atual do projeto, `branch_id` ainda nao equivale automaticamente a uma identificacao modal fisica final como a do artigo. Ele deve ser lido como identificador local da raiz ou do candidato exportado.

## Validacao

Hoje, os testes de validacao mais importantes do repositorio sao:

- reproducao da Tabela I;
- comparacao qualitativa e quantitativa das Figs. 16-19;
- exploracao dos efeitos de `\Delta n_r` e da razao de aspecto para as Figs. 20-22.

A Tabela I tem sido especialmente util porque ela testa a convergencia em funcao do numero de harmonicos, isto e, testa o proprio coracao numerico do metodo.

## Como Ler O Projeto

Se voce estiver chegando agora, a ordem recomendada e:

1. [docs/00_resumo.md](docs/00_resumo.md)
2. [docs/01_intro.md](docs/01_intro.md)
3. [docs/02_deriv.md](docs/02_deriv.md)
4. [docs/03_result.md](docs/03_result.md)
5. [src/goell_q_solver.cpp](src/goell_q_solver.cpp)

Se o objetivo for apenas localizar formulas ou conferir a implementacao:

1. [docs/goell_01_field_expansions.md](docs/goell_01_field_expansions.md)
2. [docs/goell_02_matrix_and_normalization.md](docs/goell_02_matrix_and_normalization.md)
3. [src/goell_q_solver.cpp](src/goell_q_solver.cpp)

## Observacao Final

Este README nao tenta mais carregar sozinho a traducao do artigo inteiro. Essa funcao agora foi distribuida entre os arquivos de `docs/`, onde a explicacao ficou mais organizada. O papel do README passou a ser o que ele faz melhor: servir como mapa geral do repositorio, do fluxo numerico e do estado da reproducao.
