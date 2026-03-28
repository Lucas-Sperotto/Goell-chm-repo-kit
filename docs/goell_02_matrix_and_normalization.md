# Goell 1969 - Elementos De Matriz, Determinante Global E Normalizacao

Estas notas reescrevem as equacoes do artigo que alimentam diretamente a montagem da matriz `Q`, a normalizacao usada nas curvas de propagacao e a busca das raizes nas Secoes 2.3, 2.6 e 2.7.

## Equacoes De Casamento Em Forma Matricial

Equacoes (6a)-(6d):

Para o campo eletrico longitudinal,

para o campo magnetico longitudinal,

para o campo eletrico tangencial,

e para o campo magnetico tangencial,

Aqui `A`, `B`, `C` e `D` sao vetores-coluna contendo os coeficientes modais `a_n`, `b_n`, `c_n` e `d_n`.

## Definicoes Usadas Nos Elementos De Matriz

Na p. 2140, o artigo define:

e, para distinguir dos derivados radiais acima, eu mantenho os fatores angulares com barra:

O artigo tambem define

Na implementacao, estas quantidades aparecem de forma mais transparente se pensarmos assim:

- `J` e `K` entram nos blocos longitudinais;
- `\bar J'` e `\bar K'` representam os termos derivados que alimentam os blocos tangenciais;
- `\bar J` e `\bar K` representam os termos angulares proporcionais a `n/r_m`.

## Fatores Geometricos Da Fronteira

## Elementos De Matriz

Equacoes (7a)-(7l):

Observacao pratica para o codigo:

- as equacoes acima sao a referencia direta para os blocos preenchidos em `src/goell_q_solver.cpp`;
- quando o solver normaliza tudo por `k_0` e depois toma `Z_0 = 1`, os mesmos blocos aparecem com fatores reduzidos, mas preservando os zeros de `\det(Q)`.

## Nota Sobre Ortogonalidade Dos Campos Transversais

Equacoes (8)-(9):

Os campos transversais sao ortogonais somente quando

Usando a eq. (3), o artigo escreve

Isso nao entra diretamente na busca de raizes, mas e importante quando formos voltar aos perfis de campo.

## Normalizacao

Eq. (10):

Em seguida o artigo introduz a quantidade de propagacao normalizada e a quantidade radial normalizada.

Para deixar a ligacao com o repositorio explicita:

- eu uso `P^2_paper` para a quantidade do eixo vertical;
- e `P_paper = \sqrt{P^2_paper}` para a versao sem quadrado.

Eq. (11):

Eq. (12):

Eq. (13):

e, portanto,

Para as curvas de propagacao, o artigo usa como variavel horizontal

com `\lambda_0 = 2\pi/k_0`.

Para pequena diferenca de indice, o artigo tambem fornece

Na implementacao, isso aparece assim:

- `B` controla a varredura horizontal da curva;
- `Pprime` guarda a variavel vertical normalizada;
- os argumentos de Bessel sao montados a partir de `B`, `Pprime` e do `r_m` normalizado.

## Equacao Matricial Global

Eq. (18):

com

e

A condicao modal e entao

## Observacoes Da Busca De Raizes Na Secao 2.7.1

O artigo informa que:

1. valores-teste da variavel de propagacao normalizada foram amostrados uniformemente em `(0, 1)`;
2. o metodo de Newton foi usado em seguida para refinar as raizes;
3. nas curvas de propagacao, em geral uma iteracao de Newton era suficiente;
4. com cinco harmonicos, cada avaliacao do determinante levava cerca de `0.1 s` em um IBM 360/65.

## Nota De Escalonamento Da Pagina 2144

Para controlar underflow e overflow, o artigo observa que linhas ou colunas do determinante podem ser multiplicadas por funcoes positivas sem deslocar seus zeros.

O texto diz que foi usado um escalonamento bruto:

- termos de Bessel multiplicados por

$$
\frac{h^d d}{\left|J_n(h b)\right|},
$$

- termos de Bessel modificado multiplicados por

$$
\frac{p^d d}{K_n(p b)},
$$

onde `d` e a dimensao media do guia.

O artigo tambem afirma que `Z_0` foi entao fixado em unidade, pois isso nao desloca os zeros do determinante.

Este e o unico trecho destas notas que ainda deve ser tratado com cautela: o scan dessa passagem continua sendo a parte menos segura da extracao.
