# Goell 1969 - Roteiro De Conferencia E Pontos Delicados

Este arquivo existe para quem quiser revisar o PDF em paralelo com as notas do repositorio. Ele nao e uma lista de "erros", mas um mapa dos pontos em que leitura de scan, notacao matematica e implementacao tendem a se desencontrar.

## 1. Glifos Das Variaveis Normalizadas

Conferir no PDF:

- o glifo exato da variavel vertical da eq. (11) e das Figuras 16 a 22;
- o glifo exato da variavel horizontal da eq. (16) e das Figuras 16 a 22.

Nas notas do repositorio, usamos:

- `P^2_paper` para a variavel vertical;
- `B_paper` para a variavel horizontal.

No codigo, essas quantidades aparecem sobretudo como:

- `Pprime`
- `B`

## 2. Familias De Fase

Conferir se o artigo efetivamente fixa:

$$
\phi_n = 0,
\qquad
\psi_n = \frac{\pi}{2},
$$

e

$$
\phi_n = \frac{\pi}{2},
\qquad
\psi_n = \pi.
$$

Este ponto e importante porque ele gera, no solver, as familias `phi0` e `phi90`.

## 3. Paridade Dos Harmonicos

Conferir na Secao 2.1 que:

- modos de uma classe usam apenas harmonicos impares;
- modos da outra classe usam apenas harmonicos pares.

Isto e o que justifica a divisao `odd/even`.

## 4. Regra Especial Do Caso `even`

Conferir com cuidado a regra da Secao 2.2 para `a/b \neq 1`:

- todos os pontos seguem a primeira formula;
- exceto os dois extremos do componente `z` impar, que sao omitidos.

Este e um dos lugares mais sensiveis da implementacao.

## 5. Estrutura Em Blocos Da Matriz `Q`

Conferir na eq. (18):

- os sinais de menos nos blocos externos;
- a disposicao exata das quatro linhas de blocos;
- a correspondencia entre os blocos longitudinais e tangenciais.

## 6. Reescalonamento Da Pagina 2144

Este continua sendo o trecho mais dificil de ler no scan. O que importa conferir e:

- se os fatores de reescalonamento foram copiados corretamente;
- se o papel deles e apenas numerico, sem deslocar os zeros de `det(Q)`.

## 7. Como Usar Este Arquivo

Quando surgir uma divergencia entre PDF, notas e codigo, siga esta ordem:

1. conferir o glifo ou sinal no PDF;
2. conferir a versao resumida das equacoes em [goell_01_field_expansions.md](./goell_01_field_expansions.md) e [goell_02_matrix_and_normalization.md](./goell_02_matrix_and_normalization.md);
3. so depois mexer na implementacao.

Esse procedimento evita um erro muito comum em reproducao numerica: ajustar o codigo para compensar uma leitura equivocada do artigo.
