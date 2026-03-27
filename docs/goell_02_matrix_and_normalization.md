# Goell 1969 - Elementos De Matriz, Determinante Global E Normalizacao

Estas notas reescrevem as equacoes do artigo que alimentam diretamente a montagem da matriz `Q`, a normalizacao usada nas curvas de propagacao e a busca das raizes nas Secoes 2.3, 2.6 e 2.7.

## Equacoes De Casamento Em Forma Matricial

Equacoes (6a)-(6d):

Para o campo eletrico longitudinal,

$$
E^{LA} A = E^{LC} C,
$$

para o campo magnetico longitudinal,

$$
H^{LB} B = H^{LD} D,
$$

para o campo eletrico tangencial,

$$
E^{TA} A + E^{TB} B = E^{TC} C + E^{TD} D,
$$

e para o campo magnetico tangencial,

$$
H^{TA} A + H^{TB} B = H^{TC} C + H^{TD} D.
$$

Aqui `A`, `B`, `C` e `D` sao vetores-coluna contendo os coeficientes modais `a_n`, `b_n`, `c_n` e `d_n`.

## Definicoes Usadas Nos Elementos De Matriz

Na p. 2140, o artigo define:

$$
S = \sin(n\theta_m + \phi),
\qquad
\phi = 0 \ \text{or} \ \frac{\pi}{2},
$$

$$
C = \cos(n\theta_m + \phi),
\qquad
\phi = 0 \ \text{or} \ \frac{\pi}{2},
$$

$$
J = J_n(h r_m),
\qquad
K = K_n(p r_m),
$$

$$
J' = J_n'(h r_m),
\qquad
K' = K_n'(p r_m),
$$

$$
\bar J' = \frac{J_n'(h r_m)}{h},
\qquad
\bar K' = \frac{K_n'(p r_m)}{p},
$$

e, para distinguir dos derivados radiais acima, eu mantenho os fatores angulares com barra:

$$
\bar J = \frac{n J_n(h r_m)}{h^2 r_m},
\qquad
\bar K = \frac{n K_n(p r_m)}{p^2 r_m}.
$$

O artigo tambem define

$$
Z_0 = \left(\frac{\mu_0}{\epsilon_0}\right)^{1/2},
\qquad
\epsilon_r = \frac{\epsilon_1}{\epsilon_0}.
$$

Na implementacao, estas quantidades aparecem de forma mais transparente se pensarmos assim:

- `J` e `K` entram nos blocos longitudinais;
- `\bar J'` e `\bar K'` representam os termos derivados que alimentam os blocos tangenciais;
- `\bar J` e `\bar K` representam os termos angulares proporcionais a `n/r_m`.

## Fatores Geometricos Da Fronteira

Para `\theta < \theta_c`,

$$
R = \sin\theta_m,
\qquad
T = \cos\theta_m,
\qquad
r_m = (a/2) \cos\theta_m,
$$

e para `\theta > \theta_c`,

$$
R = -\cos\theta_m,
\qquad
T = \sin\theta_m,
\qquad
r_m = (b/2) \sin\theta_m.
$$

No canto `\theta = \theta_c`, o artigo assume que a fronteira e perpendicular a reta radial, obtendo

$$
R = \cos\left(\theta_c + \frac{\pi}{4}\right),
\qquad
T = \cos\left(\theta_c - \frac{\pi}{4}\right),
\qquad
r_m = \frac{\sqrt{a^2 + b^2}}{4}.
$$

## Elementos De Matriz

Equacoes (7a)-(7l):

$$
e_{mn}^{LA} = J S,
$$

$$
e_{mn}^{LC} = K S,
$$

$$
h_{mn}^{LB} = J C,
$$

$$
h_{mn}^{LD} = K C,
$$

$$
e_{mn}^{TA} = -k_z (\bar J' S R + \bar J C T),
$$

$$
e_{mn}^{TB} = k_0 Z_0 (\bar J S R + \bar J' C T),
$$

$$
e_{mn}^{TC} = k_z (\bar K' S R + \bar K C T),
$$

$$
e_{mn}^{TD} = -k_0 Z_0 (\bar K S R + \bar K' C T),
$$

$$
h_{mn}^{TA} = \frac{\epsilon_r k_0}{Z_0} (\bar J C R - J' S T),
$$

$$
h_{mn}^{TB} = -k_z (\bar J' C R - \bar J S T),
$$

$$
h_{mn}^{TC} = -\frac{k_0}{Z_0} (\bar K C R - \bar K' S T),
$$

$$
h_{mn}^{TD} = k_z (\bar K' C R - \bar K S T).
$$

Observacao pratica para o codigo:

- as equacoes acima sao a referencia direta para os blocos preenchidos em `src/goell_q_solver.cpp`;
- quando o solver normaliza tudo por `k_0` e depois toma `Z_0 = 1`, os mesmos blocos aparecem com fatores reduzidos, mas preservando os zeros de `\det(Q)`.

## Nota Sobre Ortogonalidade Dos Campos Transversais

Equacoes (8)-(9):

Os campos transversais sao ortogonais somente quando

$$
E_t \cdot H_t = E_r H_r + E_{\theta} H_{\theta} = 0.
$$

Usando a eq. (3), o artigo escreve

$$
E_t \cdot H_t = \frac{k_z^2 - k^2}{k_z^2} \left( \frac{\partial H_z}{\partial r}\frac{\partial E_z}{\partial r} + \frac{1}{r^2} \frac{\partial H_z}{\partial \theta} \frac{\partial E_z}{\partial \theta}\right).
$$

Isso nao entra diretamente na busca de raizes, mas e importante quando formos voltar aos perfis de campo.

## Normalizacao

Eq. (10):

$$
h r = \left[k_1^2 - k_0^2 - p^2\right]^{1/2} r.
$$

Em seguida o artigo introduz a quantidade de propagacao normalizada e a quantidade radial normalizada.

Para deixar a ligacao com o repositorio explicita:

- eu uso `P^2_paper` para a quantidade do eixo vertical;
- e `P_paper = \sqrt{P^2_paper}` para a versao sem quadrado.

Eq. (11):

$$
P^2_{paper} = \frac{(k_z/k_0)^2 - 1}{n_r^2 - 1},
$$

Eq. (12):

$$
\Omega = r k_0 (n_r^2 - 1)^{1/2},
$$

Eq. (13):

$$
n_r = \left(\frac{k_1}{k_0}\right)^{1/2},
$$

e, portanto,

$$
p r = P_{paper}\Omega,
$$

$$
h r = \Omega(1 - P^2_{paper})^{1/2}.
$$

Para as curvas de propagacao, o artigo usa como variavel horizontal

$$
B_{paper} = \frac{2b}{\lambda_0}(n_r^2 - 1)^{1/2},
$$

com `\lambda_0 = 2\pi/k_0`.

Para pequena diferenca de indice, o artigo tambem fornece

$$
P^2_{paper}
\approx
\frac{k_z/k_0 - 1}{\Delta n_r},
\qquad
\Delta n_r = n_r - 1.
$$

Na implementacao, isso aparece assim:

- `B` controla a varredura horizontal da curva;
- `Pprime` guarda a variavel vertical normalizada;
- os argumentos de Bessel sao montados a partir de `B`, `Pprime` e do `r_m` normalizado.

## Equacao Matricial Global

Eq. (18):

$$
[Q][T] = 0,
$$

com

$$
Q =
\begin{bmatrix}
E^{LA} & 0      & -E^{LC} & 0 \\
0      & H^{LB} & 0       & -H^{LD} \\
E^{TA} & E^{TB} & -E^{TC} & -E^{TD} \\
H^{TA} & H^{TB} & -H^{TC} & -H^{TD}
\end{bmatrix},
$$

e

$$
[T] =
\begin{bmatrix}
A \\
B \\
C \\
D
\end{bmatrix}.
$$

A condicao modal e entao

$$
\det[Q] = 0.
$$

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
