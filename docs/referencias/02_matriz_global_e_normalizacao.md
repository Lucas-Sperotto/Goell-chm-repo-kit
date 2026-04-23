# Goell 1969 - Elementos De Matriz, Determinante Global E Normalizacao

_Navegacao: [README geral](../../README.md) | [Indice de docs](../README.md) | [Referencia anterior](01_expansoes_de_campo.md) | [Proxima referencia](03_checklist_de_conferencia.md)_

Este arquivo reune as equacoes da Secao 2.3 em diante que entram diretamente na montagem da matriz `Q` e na construcao das curvas de propagacao.

## 1. Equacoes De Casamento Em Forma Matricial

As condicoes de contorno sao agrupadas como:

$$
E^{LA} A = E^{LC} C,
$$

$$
H^{LB} B = H^{LD} D,
$$

$$
E^{TA} A + E^{TB} B = E^{TC} C + E^{TD} D,
$$

$$
H^{TA} A + H^{TB} B = H^{TC} C + H^{TD} D.
$$

Aqui `A`, `B`, `C` e `D` sao vetores-coluna de coeficientes modais.

## 2. Notacao Compacta Para Os Elementos

Para evitar a ambiguidade visual do scan entre barras e primas, estas notas separam explicitamente os termos radiais e angulares.

Definimos:

$$
S = \sin(n\theta_m + \phi),
\qquad
C = \cos(n\theta_m + \phi),
$$

$$
J = J_n(h r_m),
\qquad
K = K_n(p r_m),
$$

$$
J_r = \frac{J_n'(h r_m)}{h},
\qquad
K_r = \frac{K_n'(p r_m)}{p},
$$

$$
J_{\theta} = \frac{n J_n(h r_m)}{h^2 r_m},
\qquad
K_{\theta} = \frac{n K_n(p r_m)}{p^2 r_m},
$$

$$
Z_0 = \sqrt{\frac{\mu_0}{\epsilon_0}},
\qquad
\epsilon_r = \frac{\epsilon_1}{\epsilon_0}.
$$

## 3. Blocos Longitudinais

Os elementos dos blocos longitudinais sao:

$$
e_{mn}^{LA} = J S,
\qquad
e_{mn}^{LC} = K S,
$$

$$
h_{mn}^{LB} = J C,
\qquad
h_{mn}^{LD} = K C.
$$

Esses blocos codificam diretamente o casamento de `E_z` e `H_z`.

## 4. Blocos Tangenciais

Os blocos tangenciais sao:

$$
e_{mn}^{TA} =
-k_z \left(J_r S R + J_{\theta} C T\right),
$$

$$
e_{mn}^{TB} =
k_0 Z_0 \left(J_{\theta} S R + J_r C T\right),
$$

$$
e_{mn}^{TC} =
k_z \left(K_r S R + K_{\theta} C T\right),
$$

$$
e_{mn}^{TD} =
-k_0 Z_0 \left(K_{\theta} S R + K_r C T\right),
$$

$$
h_{mn}^{TA} =
\frac{\epsilon_r k_0}{Z_0}
\left(J_{\theta} C R - J_r S T\right),
$$

$$
h_{mn}^{TB} =
-k_z \left(J_r C R - J_{\theta} S T\right),
$$

$$
h_{mn}^{TC} =
-\frac{k_0}{Z_0}
\left(K_{\theta} C R - K_r S T\right),
$$

$$
h_{mn}^{TD} =
k_z \left(K_r C R - K_{\theta} S T\right).
$$

Interpretacao:

- os termos com `J_r` e `K_r` vem das derivadas radiais;
- os termos com `J_{\theta}` e `K_{\theta}` vem das derivadas angulares;
- `R` e `T` projetam o campo sobre a tangente local da fronteira.

## 5. Fatores Geometricos Da Fronteira

Para `\theta_m < \theta_c`,

$$
R = \sin\theta_m,
\qquad
T = \cos\theta_m,
\qquad
r_m = \frac{a/2}{\cos\theta_m}.
$$

Para `\theta_m > \theta_c`,

$$
R = -\cos\theta_m,
\qquad
T = \sin\theta_m,
\qquad
r_m = \frac{b/2}{\sin\theta_m}.
$$

Estas formulas sao particularmente importantes no repositorio porque uma leitura geometrica incorreta da fronteira altera sensivelmente as raizes obtidas.

## 6. Ortogonalidade Dos Campos Transversais

O artigo lembra que os campos transversais nao precisam ser ortogonais ponto a ponto. A condicao local de ortogonalidade seria

$$
E_t \cdot H_t = E_r H_r + E_{\theta} H_{\theta} = 0.
$$

Mas, em geral,

$$
E_t \cdot H_t =
\frac{k^2 - k_z^2}{k_z^2}
\left(
\frac{\partial H_z}{\partial r}\frac{\partial E_z}{\partial r}
+
\frac{1}{r^2}
\frac{\partial H_z}{\partial \theta}
\frac{\partial E_z}{\partial \theta}
\right).
$$

Isso explica por que os perfis de campo em guias dielectricos sao mais ricos do que nos casos metalicos ideais.

## 7. Normalizacao

O artigo introduz uma variavel de propagacao normalizada:

$$
P^2_{paper} =
\frac{(k_z/k_0)^2 - 1}{n_r^2 - 1},
$$

e uma coordenada radial normalizada:

$$
\Omega = r k_0 \sqrt{n_r^2 - 1}.
$$

Disso seguem:

$$
p r = P_{paper}\,\Omega,
$$

$$
h r = \Omega \sqrt{1 - P^2_{paper}}.
$$

Para as curvas de propagacao, a variavel horizontal e

$$
B_{paper} =
\frac{2b}{\lambda_0}\sqrt{n_r^2 - 1},
\qquad
\lambda_0 = \frac{2\pi}{k_0}.
$$

No limite de pequeno contraste de indice, `P^2_{paper}` mede aproximadamente o afastamento de `k_z` em relacao a `k_0`, e por isso e uma coordenada de plotagem muito conveniente.

## 8. Equacao Matricial Global

As quatro familias de equacoes sao reunidas em

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

A condicao modal e

$$
\det(Q) = 0.
$$

## 9. Busca De Raizes

Goell descreve o procedimento numerico da seguinte forma:

1. amostrar a variavel normalizada em `(0,1)`;
2. localizar aproximadamente as raizes;
3. refinar com Newton.

Com cinco harmonicos espaciais, o artigo relata cerca de `0.1 s` por avaliacao do determinante em um IBM 360/65.

## 10. Nota Sobre Reescalonamento

O artigo observa que linhas e colunas da matriz podem ser multiplicadas por fatores positivos sem deslocar os zeros do determinante. Isso permite controlar:

- underflow;
- overflow;
- perda de significancia numerica.

A forma exata do reescalonamento descrito na p. 2144 continua sendo o trecho menos nitido do scan. A ideia central, porem, e clara: melhorar o condicionamento numerico sem mudar a posicao das raizes.

## 11. Como Este Arquivo Deve Ser Usado

Use este arquivo quando a pergunta for:

- "qual e exatamente a estrutura de `Q`?"
- "que combinacao de derivadas entra em cada bloco?"
- "como `B` e `Pprime` se relacionam com as variaveis do artigo?"

Para a interpretacao fisica e a narrativa completa, a leitura principal continua sendo [../02_derivacao_das_equacoes.md](../02_derivacao_das_equacoes.md).

_Continua em: [03_checklist_de_conferencia.md](03_checklist_de_conferencia.md) | [Indice de docs](../README.md)_
