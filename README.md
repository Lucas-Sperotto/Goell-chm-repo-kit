# Goell 1969 — Rectangular Dielectric Waveguides

Reprodução computacional de:

J. E. Goell, *A Circular-Harmonic Computer Analysis of Rectangular Dielectric Waveguides*, Bell System Technical Journal, setembro de 1969.

O repositório está organizado para a parte mais diretamente reprodutível do artigo neste estágio: as **curvas de propagação** das Figs. 16–19, com a formulação matricial da Seção 2 e a leitura das observações de acurácia da Seção 3.

## Escopo Atual

- Solver principal: [src/goell_q_solver.cpp](src/goell_q_solver.cpp)
- Runner de presets do paper: [src/presets.sh](src/presets.sh)
- Plot das curvas: [src/plot_compare.py](src/plot_compare.py)
- PDF de referência: [j.1538-7305.1969.tb01168.x.pdf](j.1538-7305.1969.tb01168.x.pdf)

Arquivos antigos baseados em uma formulação genérica ou experimental foram removidos para reduzir ambiguidade.

## O Que Foi Fixado Em Relação Ao Paper

O ponto central da revisão foi separar corretamente os dois eixos de simetria do método de Goell:

1. **Paridade dos harmônicos** da Seção 2.1:
   - `odd`: `n = 1, 3, 5, ...`
   - `even`: `n = 0, 2, 4, ...`
2. **Família de fase** da Seção 2.1:
   - `phi0`: `S = sin(nθ)`, `C = cos(nθ)`
   - `phi90`: `S = cos(nθ)`, `C = -sin(nθ)`

Isso gera quatro classes independentes:

- `odd / phi0`
- `odd / phi90`
- `even / phi0`
- `even / phi90`

O solver atual monta `Q` diretamente a partir de:

- eqs. `(1a)–(1d)` para `E_z` e `H_z`
- eqs. `(3a)–(3d)` para campos transversos
- eqs. `(4a)–(4b)` para o campo tangencial
- eqs. `(6a)–(6d)` para o matching matricial
- eqs. `(7a)–(7l)` para os elementos de matriz
- eq. `(18)` para a matriz global `Q`
- eq. `(19)` para o critério modal `Det(Q)=0`

Também foi implementada a observação da **Seção 2.2** para o caso `even` com `a/b != 1`: todos os pontos seguem a primeira fórmula, exceto o componente `z` ímpar, que omite o primeiro e o último ponto.

## Leitura Das Seções 3.1–3.3

As páginas finais do artigo guiam diretamente a organização numérica do projeto:

- **Seção 3.1 (Accuracy)**:
  - a Tabela I usa o primeiro modo em `B = 2`
  - a variação total reportada é de aproximadamente `0.2%`, `0.4%`, `0.4%` e `1.5%` para `a/b = 1, 2, 3, 4`
  - o texto conclui que os resultados da seção seguinte são tipicamente bons a `1%`, exceto modos `even` com `a/b != 1`, que ficam melhor que `2%`
- **Seção 3.2 (Mode Configurations)**:
  - o artigo diz explicitamente que **cinco harmônicos** já davam boa representação
  - por isso o default deste repositório também é `N = 5`
- **Seção 3.3 (Propagation Curves)**:
  - o eixo horizontal é `B`
  - o eixo vertical é `rho^2`, que aqui exportamos como `P'`

## Figuras Do Paper

Os presets disponíveis hoje cobrem as condições das figuras:

- `fig16`: `a/b = 1`, `n_r ≈ 1.01`  (limite de diferença de índice pequena)
- `fig17`: `a/b = 2`, `n_r ≈ 1.01`
- `fig18`: `a/b = 1`, `n_r = 1.5`
- `fig19`: `a/b = 2`, `n_r = 1.5`

As Figs. 20–22 exigem seleção e acompanhamento de ramos principais específicos; isso fica como próximo passo sobre a base organizada atual.

## Compilar

Dependências:

- `g++` com suporte a C++17
- Eigen headers, por exemplo `libeigen3-dev`
- Python 3 com `matplotlib`

Compilação manual:

```bash
mkdir -p build
g++ -O3 -std=c++17 src/goell_q_solver.cpp -I /usr/include/eigen3 -o build/goell_q_solver
```

## Rodar Os Presets

Exemplos:

```bash
bash src/presets.sh fig16
bash src/presets.sh fig17
bash src/presets.sh fig18
bash src/presets.sh fig19
```

Cada execução gera quatro CSVs, um por classe de simetria:

- `out/<figura>_odd_phi0.csv`
- `out/<figura>_odd_phi90.csv`
- `out/<figura>_even_phi0.csv`
- `out/<figura>_even_phi90.csv`

e uma figura conjunta:

- `figures/<figura>_classes.png`

Parâmetros podem ser sobrescritos por variáveis de ambiente:

```bash
N=7 NB=80 PSCAN=320 bash src/presets.sh fig17
```

Também é possível ajustar uma classe específica:

```bash
EVEN_PHI0_N=7 EVEN_PHI0_PSCAN=320 bash src/presets.sh fig17
```

O runner usa `sigma_min` (`METRIC=sv`) como default para localizar candidatos e, por padrão, aplica um corte visual `PLOT_MERIT_MAX=-8` nos gráficos para reduzir pontos fracos.

## Saída

O solver exporta:

- `branch_id`
- `B`
- `Pprime`
- `merit`
- `parity`
- `phase`

Neste estágio, `branch_id` é apenas o índice local do mínimo em cada valor de `B`; ele **não** é ainda a identificação modal final do artigo.

## Próximos Passos

1. selecionar e rastrear os ramos publicados das Figs. 16–19
2. implementar as famílias especiais das Figs. 20–22
3. só depois voltar às figuras de intensidade e configuração de campo (Figs. 5–15)
