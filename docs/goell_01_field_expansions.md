# Goell 1969 - Expansoes de Campo, Simetria e Pontos de Casamento

Estas notas reescrevem, em Markdown renderizavel, as equacoes do artigo que entram diretamente na formulacao numerica das Secoes 2.0, 2.1 e 2.2.

## Convencao De Notacao Usada Nestas Notas

Dois simbolos do scan/OCR sao faceis de confundir visualmente, entao eu os nomeio aqui de forma explicita:

- `P^2_paper`: variavel de propagacao normalizada usada no eixo vertical das Figs. 16-22.
  No repositorio, ela ainda aparece na coluna `Pprime` dos CSVs.
- `B_paper`: variavel horizontal definida na eq. (16).
  No repositorio, ela ainda aparece simplesmente como `B`.

Se algum glifo do PDF usar um estilo diferente de letra, estas notas podem ser ajustadas depois sem alterar a estrutura da implementacao.

## Geometria

O nucleo e retangular, centrado na origem, com dimensoes completas `a` na direcao `x` e `b` na direcao `y`. A propagacao ocorre ao longo de `+z`.

No primeiro quadrante, o canto do retangulo esta em

$$
\left(\frac{a}{2}, \frac{b}{2}\right),
$$

e o angulo do canto e

$$
\theta_c = \tan^{-1}\!\left(\frac{b}{a}\right).
$$

## Expansoes Dos Campos Longitudinais

fora do nucleo.

## Numeros De Onda Radiais

Equacoes (2a)-(2b):

## Componentes Transversais Dos Campos

Equacoes (3a)-(3d), onde `k` pode ser `k_1` ou `k_0` dependendo da regiao:

Estas expressoes sao a base das equacoes tangenciais depois empacotadas nas matrizes da Secao 2.3.

## Campo Tangencial Na Fronteira Retangular

Equacoes (4a)-(4b):

Nos lados verticais,

e nos lados horizontais,

O artigo observa que expressoes analogas valem para o campo magnetico tangencial.

## Simetria Em Relacao Ao Eixo x

A Secao 2.1 afirma que devem existir duas familias de fase:

1. primeiro tipo:

$$
\phi_n = 0,
\qquad
\psi_n = \frac{\pi}{2},
$$

2. segundo tipo:

$$
\phi_n = \frac{\pi}{2},
\qquad
\psi_n = \pi.
$$

Mais adiante, o artigo condensa isso em um unico parametro `\phi` dentro dos fatores `S` e `C` usados nas equacoes de matriz:

$$
S = \sin(n\theta_m + \phi),
\qquad
C = \cos(n\theta_m + \phi),
\qquad
\phi \in \{0,\frac{\pi}{2}\}.
$$

Isso implica:

- se `\phi = 0`, entao `S = \sin(n\theta_m)` e `C = \cos(n\theta_m)`;
- se `\phi = \pi/2`, entao `S = \cos(n\theta_m)` e `C = -\sin(n\theta_m)`.

## Simetria Em Relacao Ao Eixo y

O artigo argumenta que qualquer modo deve ser composto inteiramente por harmonicos impares ou inteiramente por harmonicos pares.

No exemplo ilustrativo da Secao 2.1, tomando

$$
\alpha = \theta - \frac{\pi}{2},
$$

a eq. (1c) e reescrita como

A partir disso, o artigo conclui:

- simetria pura em relacao ao eixo `y` exige todos os `n` impares;
- antissimetria pura em relacao ao eixo `y` exige todos os `n` pares.

Esse e o ponto central que leva a separar o solver em classes `odd` e `even`.

## Pontos De Casamento

A Secao 2.2 fornece as regras para a escolha dos pontos de casamento usados nos resultados da Secao 3.

### Casos Com Harmonicos Impares

Para harmonicos impares, os pontos de casamento sao

onde `N` e o numero de harmonicos espaciais.

### Casos Com Harmonicos Pares E Razao De Aspecto Unitaria

Para harmonicos pares e `a/b = 1`:

- os componentes com simetria par em torno de `\theta = 0` usam

- os componentes com simetria impar em torno de `\theta = 0` usam

Isso reduz o total para `4N - 2` coeficientes desconhecidos, em vez de `4N`.

### Casos Com Harmonicos Pares E Razao De Aspecto Nao Unitaria

Para `a/b \neq 1`, o artigo afirma:

- todos os pontos seguem a primeira formula acima;
- exceto os primeiros e ultimos pontos do componente `z` impar, que sao omitidos.

Essa regra especial e facil de implementar errado, entao vale a pena mantela destacada ao lado do codigo.
