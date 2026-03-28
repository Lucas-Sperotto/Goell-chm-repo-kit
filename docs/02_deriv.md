# II. DERIVAÇÃO DAS EQUAÇÕES

O guia de onda considerado aqui consiste em um núcleo dielétrico retangular, de constante dielétrica constante, ε₀, cercado por um meio infinito de constante dielétrica ε₀. Ambos os meios são assumidos isotrópicos e com permeabilidade igual à do espaço livre. A Figura 2 mostra os sistemas de coordenadas (retangulares e cilíndricas) e as dimensões da seção transversal utilizadas neste artigo. A direção de propagação é no sentido do eixo z (em direção ao observador).

Em coordenadas cilíndricas, as soluções de campo das equações de Maxwell assumem a forma de funções de Bessel e funções de Bessel modificadas multiplicadas por funções trigonométricas, juntamente com suas derivadas. Para que a propagação ocorra na direção z, as soluções de campo devem ser funções de Bessel no núcleo e funções de Bessel modificadas no exterior.

Como as funções de Bessel de segunda espécie possuem um polo na origem e as funções de Bessel modificadas de primeira espécie possuem um polo no infinito, a variação radial dos campos é assumida como uma soma de funções de Bessel de primeira espécie e suas derivadas dentro do núcleo e uma soma de funções de Bessel modificadas e suas derivadas fora do núcleo.

Em coordenadas cilíndricas, as componentes na direção z dos campos elétrico e magnético são dadas por (1a)

$$
E_{z1} = \sum_{n=0}^{\infty} a_n J_n(hr) \sin(n\theta + \phi_n) \exp[i(k_z z - \omega t)],
$$

e (1b)

$$
H_{z1} = \sum_{n=0}^{\infty} b_n J_n(hr) \sin(n\theta + \psi_n) \exp[i(k_z z - \omega t)],
$$

dentro do nucleo, e por (1c)

$$
E_{z0} = \sum_{n=0}^{\infty} c_n K_n(pr) \sin(n\theta + \phi_n) \exp[i(k_z z - \omega t)],
$$

e (1d)

$$
H_{z0} = \sum_{n=0}^{\infty} d_n K_n(pr) \sin(n\theta + \psi_n) \exp[i(k_z z - \omega t)],
$$

fora do núcleo, onde ω é a frequência angular e k_z é a constante de propagação longitudinal. As constantes de propagação transversal são dadas por:

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

onde k₁ = ω√(μ₀ε₁) e k₀ = ω√(μ₀ε₀). Os termos Jₙ e Kₙ são funções de Bessel de ordem n e funções de Bessel modificadas, respectivamente, e ψₙ e φₙ são ângulos de fase arbitrários.

Os componentes transversais do campo são dados por:

$$
E_r = \frac{i k_z}{k^2 - k_z^2} \left[\frac{\partial E_z}{\partial r} + \frac{\mu_0 \omega}{k_z r} \frac{\partial H_z}{\partial \theta}\right],
$$

---

![Figura 2 – Dimensões e sistema de coordenadas](FIGURA_2_AQUI.png)

---

$$
E_{\theta} = \frac{i k_z}{k^2 - k_z^2} \left[\frac{1}{r}\frac{\partial E_z}{\partial \theta} - \frac{\mu_0 \omega}{k_z} \frac{\partial H_z}{\partial r}\right],
$$

$$
H_r = \frac{i k_z}{k^2 - k_z^2} \left[ - \frac{k^2}{\mu_0 \omega k_z r} \frac{\partial E_z}{\partial \theta} + \frac{\partial H_z}{\partial r}\right],
$$

$$
H_{\theta} = \frac{i k_z}{k^2 - k_z^2} \left[ \frac{k^2}{\mu_0 \omega k_z} \frac{\partial E_z}{\partial r} + \frac{1}{r}\frac{\partial H_z}{\partial \theta}\right].
$$

onde k pode ser k₁ ou k₀.

Finalmente, o componente do campo elétrico tangente ao núcleo retangular é dado por:

$$
E_t = \pm\left(E_r \sin\theta + E_{\theta}\cos\theta\right),
\qquad
-\theta_c < \theta < \theta_c,
\qquad
\pi-\theta_c < \theta < \pi+\theta_c,
$$

ou

$$
E_t = \pm\left(-E_r \cos\theta + E_{\theta}\sin\theta\right),
\qquad
\theta_c < \theta < \pi-\theta_c,
\qquad
\pi+\theta_c < \theta < 2\pi-\theta_c.
$$

onde θ_c é o ângulo que uma linha radial até o canto no primeiro quadrante faz com o eixo x. Expressões similares existem para o campo magnético tangencial.

---

## 2.1 Efeitos da Simetria

Como o guia de onda é simétrico em relação ao eixo x, os campos devem ser simétricos ou antissimétricos em relação a esse eixo. Isso ocorre porque a estrutura é invariante sob rotações de 180°, e portanto os padrões de campo devem ser invariantes sob essa rotação, exceto pelo sinal.

A partir disso, e do fato de que θ/θ aparece em cada equação (3), fica evidente que dois tipos de modos devem existir:

- o primeiro com φₙ = 0 e ψₙ = π/2  
- o segundo com φₙ = π/2 e ψₙ = π  

De forma semelhante, as funções de campo também devem ser simétricas ou antissimétricas em relação ao eixo y.

Suponha, por exemplo, que E_z₀ apresente variação angular senoidal em torno de θ = 0 (E_z₀ é ímpar em relação ao eixo x). Então, fazendo α = θ − π/2, a equação (1c) pode ser escrita na forma:

$$
E_{zo} = \sum_{n=0}^{\infty} c_n K_n(pr) [ \sin(n\alpha)\cos\frac{n\pi}{2} + \cos(n\alpha)\sin\frac{n\pi}{2} ].
$$

Para que E_z₀ seja puramente simétrico em relação a α = 0 (eixo y), todos os n devem ser ímpares.  
Para que seja antissimétrico, todos os n devem ser pares.

Como resultados semelhantes se aplicam para variação cossenoidal de E_z₀, θ = 0,  e como todas as demais funções de campo também o são, qualquer modo deve consistir apenas de harmônicos pares ou apenas de harmônicos ímpares.

Da análise anterior, fica claro que se os pontos de correspondência forem escolhidos simetricamente em relação aos eixos x e y, então, exceto pelo sinal, cada ponto terá um ponto equivalente em cada quadrante.

Assim, a correspondência dos campos precisa ser feita apenas em um quadrante. Isso reduz:

- o número de constantes por um fator 4  
- o número de pontos necessários para uma mesma precisão também por um fator 4  

---

## 2.2 Seleção dos Pontos de Correspondência

Como mencionado na Seção 2.1, os pontos devem ser escolhidos simetricamente em relação aos eixos x e y.

Para os casos com harmônicos ímpares, os pontos utilizados foram:

$$
\theta_m = \frac{(m - 1/2)\pi}{2N},
\qquad
m = 1,2,\ldots,N,
$$

com m = 1, ..., N, onde N é o número de harmônicos.

Para harmônicos pares, a escolha é mais complexa, pois a existência simultânea de um harmônico n = 0 para modos TE e TM é inconsistente com a simetria do guia.

Assim:

- número total de coeficientes = 4N − 2
- em vez de 4N  

Os pontos são escolhidos da seguinte forma:

- para componentes com simetria par:

$$
\theta_m = \frac{(m - 1/2)\pi}{2N},
\qquad
m = 1,2,\ldots,N,
$$

- para componentes com simetria ímpar:

$$
\theta_m = \frac{(m - N - 1/2)\pi}{2(N-1)},
\qquad
m = N+1, N+2, \ldots, 2N-1.
$$

Para razões de aspecto unitárias (a/b = 1), todos os pontos são escolhidos pela primeira fórmula, exceto os extremos.

---

## 2.3 Formulação dos Elementos de Matriz

Os coeficientes da equação (1) são obtidos pela correspondência dos campos elétricos e magnéticos tangenciais ao longo da fronteira do núcleo.

Como cada campo possui componentes:

- longitudinais  
- transversais  

existem quatro tipos de equações de correspondência.

Para facilitar a análise computacional, essas equações são colocadas na forma matricial.

As equações matriciais para os campos longitudinais são:

$$
E^{LA} A = E^{LC} C,
$$

para o campo eletrico e

$$
H^{LB} B = H^{LD} D,
$$  

para o campo magnético.

$$
E^{TA} A + E^{TB} B = E^{TC} C + E^{TD} D,
$$

$$
H^{TA} A + H^{TB} B = H^{TC} C + H^{TD} D.
$$

---

As matrizes A, B, C e D são vetores coluna com N elementos dos coeficientes.

Os elementos das matrizes são dados por:

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

---

onde:

$$
Z_0 = \left(\frac{\mu_0}{\epsilon_0}\right)^{1/2},
\qquad
\epsilon_r = \frac{\epsilon_1}{\epsilon_0}.
$$

e

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
\bar J = \frac{n J_n(h r_m)}{h^2 r_m},
\qquad
\bar K = \frac{n K_n(p r_m)}{p^2 r_m}.
$$

$$
\bar J' = \frac{J_n'(h r_m)}{h},
\qquad
\bar K' = \frac{K_n'(p r_m)}{p},
$$

- derivadas indicadas por '  

e

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

---

## 2.4 Designação dos Modos

Diferentemente de guias metálicos, os padrões de campo em guias dielétricos dependem de:

- diferença de índice de refração  
- comprimento de onda  
- razão de aspecto  

Isso dificulta uma classificação simples.

Para guias metálicos:

- modos são TE ou TM  
- classificados por máximos em x e y  

Para guias dielétricos:

- modos não são puramente TE ou TM  

A classificação adotada é baseada no limite de pequeno contraste de índice.

Assim:

- modos são denominados Eₘₙ ou Hₘₙ  
- dependendo da direção dominante do campo elétrico  

Onde:

- m = número de máximos em x  
- n = número de máximos em y  

---

## 2.5 Diferenças entre Campos Elétrico e Magnético

Para guias metálicos:

- E_t e H_t têm comportamento similar  
- impedância independe da posição  
- fluxo de potência não muda de sinal  

Para guias dielétricos:

- campos não são semelhantes  
- impedância depende da posição  

Para que os campos transversais sejam perpendiculares:

$$
E_t \cdot H_t = E_r H_r + E_{\theta} H_{\theta} = 0.
$$

A partir da equação (3):

$$
E_t \cdot H_t = \frac{k_z^2 - k^2}{k_z^2} \left( \frac{\partial H_z}{\partial r}\frac{\partial E_z}{\partial r} + \frac{1}{r^2} \frac{\partial H_z}{\partial \theta} \frac{\partial E_z}{\partial \theta}\right).
$$

Logo:

- E_t e H_t não são necessariamente perpendiculares  
- podem mudar de sinal  
- pode ocorrer fluxo de potência negativo  

Casos especiais onde isso não ocorre:

1. k ≈ k₁ ou k ≈ k₀  
2. pequeno contraste de índice  
3. simetria circular  

---

## 2.6 Normalização

Os argumentos das funções de Bessel são:

- h r = √(k₁² − k_z²) r  
- p r = √(k_z² − k₀²) r  

O primeiro pode ser escrito como:

$$
h r = \left[k_1^2 - k_0^2 - p^2\right]^{1/2} r.
$$

Definindo:

$$
P^2_{paper} = \frac{(k_z/k_0)^2 - 1}{n_r^2 - 1},
$$

$$
\Omega = r k_0 (n_r^2 - 1)^{1/2},
$$

$$
n_r = \left(\frac{k_1}{k_0}\right)^{1/2},
$$

onde n_r é o índice relativo.

Então:

$$
p r = P_{paper}\Omega,
$$

$$
h r = \Omega(1 - P^2_{paper})^{1/2}.
$$

---

As curvas da constante de propagação são expressas em termos de σ² e Ω:

$$
B_{paper} = \frac{2b}{\lambda_0}(n_r^2 - 1)^{1/2},
$$

onde λ₀ = 2π / k₀.

---

Para pequenos valores de Δn:

onde Δn = n_r − 1.

Isso justifica o uso de σ² como variável de plotagem.

---

A constante de propagação normalizada, σ², possui duas propriedades adicionais que tornam seu uso conveniente. Primeiro, seu intervalo de variação está em (0, 1). Segundo, para n_r ≈ 1:

$$
P^2_{paper}
\approx
\frac{k_z/k_0 - 1}{\Delta n_r},
\qquad
\Delta n_r = n_r - 1.
$$  

onde Δn_r = n_r − 1; portanto, para pequenos valores de n_r, σ² é proporcional a k_z − k₀. Essa última propriedade é a razão pela qual σ², e não Ω, foi utilizada como variável de plotagem.

---

## 2.7 Método de Computação

### 2.7.1 Constante de Propagação

A equação (6) resulta em 4N equações lineares homogêneas simultâneas para os coeficientes aₙ, bₙ, cₙ e dₙ no caso de modos ímpares, e 4N − 2 equações para os modos pares, utilizando os pontos de correspondência previamente descritos.

As equações podem ser combinadas para formar uma única equação matricial:

$$
[Q][T] = 0,
$$  

onde:

- Q é a matriz do sistema  
- T é o vetor coluna dos coeficientes  

e

$$
Q =
\begin{bmatrix}
E^{LA} & 0      & -E^{LC} & 0 \\
0      & H^{LB} & 0       & -H^{LD} \\
E^{TA} & E^{TB} & -E^{TC} & -E^{TD} \\
H^{TA} & H^{TB} & -H^{TC} & -H^{TD}
\end{bmatrix},
$$

$$
[T] =
\begin{bmatrix}
A \\
B \\
C \\
D
\end{bmatrix}.
$$  

Todas as quantidades nas matrizes [Q] e [T] são, elas próprias, matrizes definidas pelas equações (1), (6) e (7).

Para que exista uma solução não trivial da equação (18):

$$
\det[Q] = 0.
$$

---

A constante de propagação normalizada σ² foi obtida substituindo valores de teste na equação (19). Inicialmente, valores de σ² uniformemente distribuídos no intervalo (0, 1) foram utilizados para localizar aproximadamente as raízes. Em seguida, o método de Newton foi aplicado para encontrar as raízes com a precisão desejada.

Em geral:

- uma iteração de Newton foi suficiente para curvas de propagação  
- cerca de dez iterações foram necessárias para cálculos de padrões de campo  

---

Tanto o método simples de triangulação quanto o método mais complexo de condensação por pivô de Gauss foram utilizados para avaliar o determinante.

- O método de triangulação foi usado na maioria dos casos  
- O método de Gauss foi usado quando erros de arredondamento eram relevantes  

Em todos os casos, foi utilizada aritmética de dupla precisão.

Para cinco harmônicos espaciais:

- cerca de 0,1 s de tempo computacional (IBM 360/65) por valor de σ²  

---

Devido à ampla faixa dinâmica dos coeficientes, medidas foram tomadas para evitar:

- underflow  
- overflow  
- erros de arredondamento  

Multiplicar uma linha ou coluna da matriz por uma constante é equivalente a multiplicar o determinante por essa constante. Assim, qualquer linha ou coluna pode ser escalada sem alterar os zeros do determinante.

Embora exista uma teoria mais refinada para escolher as “melhores funções”, foi utilizado um método direto (“força bruta”), pois os métodos mais sofisticados aumentariam significativamente a complexidade do programa.

Verificou-se que:

- termos de Bessel → multiplicados por h²d / |Jₙ(hb)|  
- termos de Bessel modificados → multiplicados por p²d / Kₙ(pb)  

onde d é a média das dimensões do guia de onda

Isso manteve os termos sob controle numérico.

Uma simplificação adicional foi feita definindo:

- Z₀ = 1  

o que não altera os zeros do determinante.

---

### 2.7.2 Configurações dos Modos

Os campos elétricos e magnéticos foram calculados para casos representativos a partir da equação (3).

Para encontrar os coeficientes:

- primeiro calcula-se k_z pela equação (19)  
- depois substitui-se em (18)  

Ao fixar um dos elementos do vetor T como 1, os demais são obtidos por técnicas padrão de álgebra linear.

---

Diversas abordagens foram utilizadas para obter os padrões de campo:

- cálculo dos campos ao longo de cortes radiais  
- geração de isolinhas (direção do campo elétrico)  
- imagens dos modos geradas por computador  

---

As isolinhas e figuras foram desenhadas utilizando um plotter simulado Stromberg-Carlson SC-4020:

- resolução: 1024 × 1024  
- apenas um quadrante foi utilizado (simetria)  

As dimensões foram escaladas de modo que:

- o lado maior ocupasse 80% da largura  

Os pontos utilizados foram:

- (20m, 20n), com m, n = 0 até 49  

---

As isolinhas foram desenhadas traçando linhas paralelas ao campo elétrico em cada ponto (todas com o mesmo comprimento). Essas figuras serviram como base para os desenhos finais das linhas de campo.

---

Para gerar imagens dos modos:

- calculou-se a densidade de potência em cada ponto  
- utilizou-se a raiz quadrada da densidade de potência  
- normalizou-se pelo valor máximo  
- quantizou-se em 21 níveis  

Cada ponto foi representado por uma porção da figura correspondente ao nível quantizado.

Nos pontos onde a potência era zero:

- nenhum traçado foi feito  

---

Como o tamanho do ponto do CRT era aproximadamente igual ao espaçamento das linhas:

- as figuras resultantes ficaram preenchidas  

Assim:

- a intensidade luminosa ≈ densidade de potência

---

Para pequenas diferenças de índice:

- densidade de potência ∝ |E_t|²  
- faixa dinâmica ≈ 400  

---

As figuras completas foram obtidas a partir de um único quadrante:

- exposição quadruplicada do microfilme  

Tempo de computação:

- entre 30 e 60 segundos (IBM 360/65) por figura  

---
