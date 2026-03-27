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

Equacoes (1a)-(1d) do artigo:

$$
E_{z1} = \sum_{n=0}^{\infty} a_n J_n(hr) \sin(n\theta + \phi_n) \exp[i(k_z z - \omega t)],
$$

$$
H_{z1} = \sum_{n=0}^{\infty} b_n J_n(hr) \sin(n\theta + \psi_n) \exp[i(k_z z - \omega t)],
$$

dentro do nucleo, e

$$
E_{z0} = \sum_{n=0}^{\infty} c_n K_n(pr) \sin(n\theta + \phi_n) \exp[i(k_z z - \omega t)],
$$

$$
H_{z0} = \sum_{n=0}^{\infty} d_n K_n(pr) \sin(n\theta + \psi_n) \exp[i(k_z z - \omega t)],
$$

fora do nucleo.

## Numeros De Onda Radiais

Equacoes (2a)-(2b):

$$
h = (k_1^2 - k_z^2)^{1/2},
$$

$$
p = (k_z^2 - k_0^2)^{1/2},
$$

com

$$
k_1 = \omega(\mu_0 \epsilon_1)^{1/2},
\qquad
k_0 = \omega(\mu_0 \epsilon_0)^{1/2}.
$$

## Componentes Transversais Dos Campos

Equacoes (3a)-(3d), onde `k` pode ser `k_1` ou `k_0` dependendo da regiao:

$$
E_r = \frac{i k_z}{k^2 - k_z^2} \left[\frac{\partial E_z}{\partial r} + \frac{\mu_0 \omega}{k_z r} \frac{\partial H_z}{\partial \theta}\right],
$$

$$
E_{\theta} = \frac{i k_z}{k^2 - k_z^2} \left[\frac{1}{r}\frac{\partial E_z}{\partial \theta} - \frac{\mu_0 \omega}{k_z} \frac{\partial H_z}{\partial r}\right],
$$

$$
H_r = \frac{i k_z}{k^2 - k_z^2} \left[ - \frac{k^2}{\mu_0 \omega k_z r} \frac{\partial E_z}{\partial \theta} + \frac{\partial H_z}{\partial r}\right],
$$

$$
H_{\theta} = \frac{i k_z}{k^2 - k_z^2} \left[ \frac{k^2}{\mu_0 \omega k_z} \frac{\partial E_z}{\partial r} + \frac{1}{r}\frac{\partial H_z}{\partial \theta}\right].
$$

Estas expressoes sao a base das equacoes tangenciais depois empacotadas nas matrizes da Secao 2.3.

## Campo Tangencial Na Fronteira Retangular

Equacoes (4a)-(4b):

Nos lados verticais,

$$
E_t = \pm\left(E_r \sin\theta + E_{\theta}\cos\theta\right),
\qquad
-\theta_c < \theta < \theta_c,
\qquad
\pi-\theta_c < \theta < \pi+\theta_c,
$$

e nos lados horizontais,

$$
E_t = \pm\left(-E_r \cos\theta + E_{\theta}\sin\theta\right),
\qquad
\theta_c < \theta < \pi-\theta_c,
\qquad
\pi+\theta_c < \theta < 2\pi-\theta_c.
$$

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

$$
E_{zo} = \sum_{n=0}^{\infty} c_n K_n(pr) [ \sin(n\alpha)\cos\frac{n\pi}{2} + \cos(n\alpha)\sin\frac{n\pi}{2} ].
$$

A partir disso, o artigo conclui:

- simetria pura em relacao ao eixo `y` exige todos os `n` impares;
- antissimetria pura em relacao ao eixo `y` exige todos os `n` pares.

Esse e o ponto central que leva a separar o solver em classes `odd` e `even`.

## Pontos De Casamento

A Secao 2.2 fornece as regras para a escolha dos pontos de casamento usados nos resultados da Secao 3.

### Casos Com Harmonicos Impares

Para harmonicos impares, os pontos de casamento sao

$$
\theta_m = \frac{(m - 1/2)\pi}{2N},
\qquad
m = 1,2,\ldots,N,
$$

onde `N` e o numero de harmonicos espaciais.

### Casos Com Harmonicos Pares E Razao De Aspecto Unitaria

Para harmonicos pares e `a/b = 1`:

- os componentes com simetria par em torno de `\theta = 0` usam

$$
\theta_m = \frac{(m - 1/2)\pi}{2N},
\qquad
m = 1,2,\ldots,N,
$$

- os componentes com simetria impar em torno de `\theta = 0` usam

$$
\theta_m = \frac{(m - N - 1/2)\pi}{2(N-1)},
\qquad
m = N+1, N+2, \ldots, 2N-1.
$$

Isso reduz o total para `4N - 2` coeficientes desconhecidos, em vez de `4N`.

### Casos Com Harmonicos Pares E Razao De Aspecto Nao Unitaria

Para `a/b \neq 1`, o artigo afirma:

- todos os pontos seguem a primeira formula acima;
- exceto os primeiros e ultimos pontos do componente `z` impar, que sao omitidos.

Essa regra especial e facil de implementar errado, entao vale a pena mantela destacada ao lado do codigo.
