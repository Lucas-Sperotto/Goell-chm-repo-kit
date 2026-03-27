# Tabela I - Hipoteses Para A Variacao Residual

Estas notas resumem a rodada de diagnostico feita apos a reproducao automatizada da Tabela I.

## Melhor Configuracao Atual

Entre as variantes testadas, a melhor foi:

- `geometry = intersection`
- `metric = det`
- `det-search = sign`
- `rescale = off` ou `on` com diferenca global desprezivel

Resumo dos erros medidos:

| geometry | det-search | rescale | RMSE | MAE | pior erro |
|:--|:--|:--|--:|--:|--:|
| `intersection` | `sign` | `off` | `0.012603` | `0.009735` | `0.031270` |
| `intersection` | `minima` | `off` | `0.037199` | `0.020985` | `0.111538` |
| `literal` | `sign` | `off` | `0.093350` | `0.072888` | `0.161010` |
| `intersection` | `sign` | `on` | `0.012576` | `0.009676` | `0.031270` |

## O Que Isso Sugere

- A leitura `intersection` da fronteira e muito melhor que a leitura `literal`.
- Procurar raizes reais por mudanca de sinal do determinante e muito melhor do que procurar minimos de `log|det|`.
- O reescalonamento da matriz nao parece ser a principal fonte da variacao residual.

## O Que Nao Parece Ser A Causa

Nos piores casos, aumentar `Pscan` de `120` para `800` nao alterou a raiz selecionada:

- `a/b = 1`, `N = 7`
- `a/b = 2`, `N = 9`
- `a/b = 3`, `N = 9`
- `a/b = 4`, `N = 7`

Entao a variacao restante nao parece ser um problema de resolucao da varredura em `P'`.

## Suspeita Principal

A classe modal selecionada muda com `N`, principalmente para `a/b = 3` e `a/b = 4`.

Exemplos:

- em `a/b = 3`, a selecao troca entre `odd/phi0`, `even/phi90` e `odd/phi90` conforme `N`;
- em `a/b = 4`, a selecao tambem troca entre `odd/phi0` e `odd/phi90`.

Isso sugere que a variacao residual da Tabela I vem mais de:

- competicao entre ramos proximos;
- identificacao modal instavel ao variar `N`;
- e possivelmente da implementacao dos casos mais delicados de simetria, em especial quando `a/b > 1`.

Em outras palavras: o problema atual parece estar mais na identificacao consistente do ramo correto do que na localizacao bruta das raizes de `det(Q) = 0`.
