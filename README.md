# Goell 1969 - Guia De Reprodução, Leitura E Implementação

Este repositório é uma base de trabalho para reproduzir numericamente o artigo:

J. E. Goell, *A Circular-Harmonic Computer Analysis of Rectangular Dielectric Waveguides*, *Bell System Technical Journal*, setembro de 1969.

O objetivo aqui não é apenas "rodar um solver", mas reconstruir com cuidado a formulação do artigo, revisar a leitura do PDF, mapear cada etapa teórica para código e, por fim, reproduzir as curvas e figuras publicadas.

## O Que Este Repositório Contém Hoje

- Solver principal: [src/goell_q_solver.cpp](src/goell_q_solver.cpp)
- Runner com presets das figuras principais: [src/presets.sh](src/presets.sh)
- Plotador das curvas exportadas: [src/plot_compare.py](src/plot_compare.py)
- Notas de equações das Seções 2.1 a 2.7:
  - [docs/goell_01_field_expansions.md](docs/goell_01_field_expansions.md)
  - [docs/goell_02_matrix_and_normalization.md](docs/goell_02_matrix_and_normalization.md)
  - [docs/goell_03_review_checklist.md](docs/goell_03_review_checklist.md)
- PDF local de referência: [j.1538-7305.1969.tb01168.x.pdf](j.1538-7305.1969.tb01168.x.pdf)

## Estado Atual Da Reprodução

O projeto já foi reorganizado em torno da formulação matricial do artigo, com separação explícita entre:

- paridade dos harmônicos: `odd` e `even`
- família de fase: `phi0` e `phi90`

Isso gera as quatro classes corretas do método:

- `odd / phi0`
- `odd / phi90`
- `even / phi0`
- `even / phi90`

O solver já monta `Q` diretamente a partir das expressões da Seção 2, mas a reprodução das curvas publicadas ainda não está fechada. No momento, o principal gargalo não é "achar soluções" e sim separar melhor os ramos físicos dos mínimos numéricos espúrios, especialmente perto das bordas da variável normalizada.

## Como Compilar E Rodar

Dependências:

- `g++` com suporte a C++17
- headers do Eigen, por exemplo `libeigen3-dev`
- Python 3 com `matplotlib`

Compilação manual:

```bash
mkdir -p build
g++ -O3 -std=c++17 src/goell_q_solver.cpp -I /usr/include/eigen3 -o build/goell_q_solver
```

Execução dos presets:

```bash
bash src/presets.sh fig16
bash src/presets.sh fig17
bash src/presets.sh fig18
bash src/presets.sh fig19
```

Cada execução gera:

- `out/<figura>_odd_phi0.csv`
- `out/<figura>_odd_phi90.csv`
- `out/<figura>_even_phi0.csv`
- `out/<figura>_even_phi90.csv`
- `figures/<figura>_classes.png`

Também é possível sobrescrever parâmetros por variáveis de ambiente:

```bash
N=7 NB=80 PSCAN=320 bash src/presets.sh fig17
EVEN_PHI0_N=7 EVEN_PHI0_PSCAN=320 bash src/presets.sh fig17
OUT_PREFIX=fig16_det METRIC=det bash src/presets.sh fig16
```

O solver exporta colunas:

- `branch_id`
- `B`
- `Pprime`
- `merit`
- `parity`
- `phase`

Neste estágio, `branch_id` ainda é apenas o índice local do mínimo em cada valor de `B`; ele ainda não representa a identificação modal final usada no artigo.

## Como Ler Este README

Este README foi ampliado para funcionar também como uma leitura guiada do artigo em português. A ideia é simples:

- o texto abaixo organiza e traduz, em linguagem de trabalho, praticamente todo o conteúdo conceitual do paper;
- as equações exatas ficam concentradas nos arquivos de `docs/`;
- o código em `src/` deve ser lido em paralelo com estas duas coisas.

Em outras palavras, este README agora serve como:

- introdução ao tema;
- tradução orientada à implementação;
- mapa entre o artigo e o repositório;
- registro do que já foi entendido e do que ainda precisa ser refinado.

## Resumo Do Artigo

O artigo descreve uma análise computacional dos modos propagantes de um guia dielétrico retangular. A formulação é baseada em uma expansão do campo eletromagnético em termos de uma série de harmônicos circulares, isto é, funções de Bessel e funções de Bessel modificadas multiplicadas por funções trigonométricas.

Dentro do núcleo do guia, os campos elétrico e magnético são representados por séries de Bessel; fora do núcleo, por séries de Bessel modificadas. Em vez de particionar o plano transversal em muitas regiões, como em outros métodos da época, Goell faz o casamento dos campos diretamente na fronteira do retângulo, em pontos apropriados distribuídos ao longo do perímetro. Esse casamento gera um sistema homogêneo cuja condição de existência de solução não trivial fornece as constantes de propagação.

O paper apresenta os resultados principalmente em duas formas:

- curvas de propagação;
- padrões de campo e intensidade.

As curvas de propagação são escritas em uma forma normalizada que procura tornar os resultados tão independentes quanto possível do valor absoluto do índice, desde que a diferença entre o índice do núcleo e o do meio externo seja pequena. Além disso, o artigo também fornece resultados para diferenças de índice mais altas, relevantes para aplicações em micro-ondas.

## 1. Introdução - Tradução De Trabalho

O artigo começa observando que guias dielétricos seriam blocos fundamentais de circuitos ópticos integrados. Eles poderiam servir não apenas como meios de transmissão de sinais ópticos e de alta diretividade, mas também como base para dispositivos como filtros e acopladores direcionais. Por isso, seria importante conhecer com precisão as propriedades de seus modos propagantes.

Guias dielétricos circulares já haviam recebido atenção considerável, em parte porque a geometria circular é comum em fibras ópticas. No entanto, em óptica integrada era natural esperar estruturas retangulares ou quase retangulares, compostas por um núcleo dielétrico imerso em um meio de índice um pouco menor. É justamente nessa geometria que a análise se torna mais difícil por causa da natureza da fronteira.

O texto discute então abordagens anteriores. Uma delas, associada a Marcatili, parte da hipótese de que a maior parte do fluxo de potência permanece confinada ao núcleo. Nessa formulação, usam-se campos com variação senoidal no núcleo e campos com decaimento exponencial fora dele, tomando apenas um modo por região. O resultado é relativamente simples de avaliar numericamente, mas depende de aproximações que podem não se comportar bem em limites extremos.

Outra linha, associada a Schlosser e Unger, divide o plano transversal em regiões e resolve o problema fazendo casamento de campo ao longo das fronteiras entre essas regiões. Em princípio, o método vale em uma faixa mais ampla, mas à custa de uma complexidade computacional maior. O próprio artigo comenta que, para um modo dado, o número de extremos de campo cresce com o comprimento de onda, o que torna o casamento entre regiões cada vez mais difícil.

Goell apresenta então uma alternativa: representar a variação radial dos campos longitudinais por uma soma de funções de Bessel no núcleo e de funções de Bessel modificadas fora dele, impondo o casamento dos campos ao longo de toda a fronteira do retângulo. Como a fronteira é a entidade geométrica natural do problema, a complexidade computacional deixa de crescer com o comprimento de onda da mesma forma que em outros esquemas.

O artigo diz explicitamente que:

- a Seção II apresenta a teoria;
- depois vêm comentários sobre técnicas computacionais e métodos gráficos;
- a Seção III é dividida em acurácia, padrões de campo e curvas de propagação.

## 2. Derivação Das Equações - Tradução De Trabalho

### 2.1 Geometria E Sistemas De Coordenadas

O guia considerado é um retângulo dielétrico de constante `\epsilon_1`, imerso em um meio infinito de constante `\epsilon_0`. Ambos os meios são isotrópicos e assumem permeabilidade de espaço livre `\mu_0`. O artigo usa coordenadas retangulares e cilíndricas, mas a formulação modal é construída em coordenadas cilíndricas porque as funções de Bessel aparecem naturalmente nas soluções radiais de Maxwell.

O sentido de propagação é o eixo `+z`, apontando para o observador. Esse detalhe de convenção é importante porque ele fixa o sinal dos fatores `\exp[i(k_z z - \omega t)]` nas expansões dos campos.

### 2.2 Campos Longitudinais

Em coordenadas cilíndricas, as componentes `z` dos campos elétrico e magnético são escritas como séries:

- no interior do núcleo, com funções de Bessel `J_n`;
- no exterior, com funções de Bessel modificadas `K_n`.

O artigo escolhe essas famílias porque:

- funções de Bessel de segunda espécie divergem na origem e por isso não servem dentro do núcleo;
- funções de Bessel modificadas de primeira espécie divergem no infinito e por isso não servem fora do núcleo;
- o comportamento físico correto exige oscilação dentro do guia e decaimento fora dele.

As expansões completas estão em [docs/goell_01_field_expansions.md](docs/goell_01_field_expansions.md).

### 2.3 Campos Transversais

Depois de definir `E_z` e `H_z`, o artigo obtém `E_r`, `E_\theta`, `H_r` e `H_\theta` por substituição direta nas equações de Maxwell. Essas relações aparecem como as eqs. `(3a)`-`(3d)` e são o elo entre a expansão longitudinal e as condições de contorno na fronteira do retângulo.

Esse passo é central porque o casamento não é imposto só aos campos longitudinais. O artigo iguala também os campos tangenciais, o que leva aos quatro tipos de equações matriciais da Seção 2.3.

### 2.4 Campo Tangencial Na Fronteira

Como a fronteira é retangular, o campo tangencial é escrito de forma diferente em cada lado:

- nos lados verticais, ele combina `E_r` e `E_\theta` com fatores `\sin\theta` e `\cos\theta`;
- nos lados horizontais, a combinação muda de sinal e de ordem.

O mesmo raciocínio vale para o campo magnético tangencial. Isso explica por que, mais adiante, os elementos de matriz aparecem multiplicados por fatores geométricos `R` e `T`.

### 2.5 Efeito Das Simetrias

Esta é uma das partes mais importantes do artigo para a implementação.

Primeiro, pela simetria em relação ao eixo `x`, os modos devem ser simétricos ou antissimétricos em torno desse eixo. Como a estrutura é invariante a rotações de 180 graus, o artigo conclui que existem duas famílias de fase:

- primeira família: `\phi_n = 0`, `\psi_n = \pi/2`
- segunda família: `\phi_n = \pi/2`, `\psi_n = \pi`

Na prática, isso leva à forma compacta posterior:

- `S = \sin(n\theta_m + \phi)`
- `C = \cos(n\theta_m + \phi)`

com `\phi = 0` ou `\pi/2`.

Depois, pela simetria em relação ao eixo `y`, o artigo mostra que um modo deve conter apenas harmônicos pares ou apenas harmônicos ímpares. Essa separação é obtida reescrevendo a dependência angular em torno de `\alpha = \theta - \pi/2`.

Essa é precisamente a razão pela qual o solver deste repositório foi reorganizado em quatro classes independentes:

- paridade `odd` + fase `phi0`
- paridade `odd` + fase `phi90`
- paridade `even` + fase `phi0`
- paridade `even` + fase `phi90`

### 2.6 Escolha Dos Pontos De Casamento

O artigo insiste que os pontos de casamento devem respeitar as simetrias do problema. Para os casos com harmônicos ímpares, usa-se um conjunto simples de `N` pontos no primeiro quadrante.

Para os casos com harmônicos pares, a situação é mais sutil por causa da presença do harmônico `n = 0`. O texto explica que:

- para `a/b = 1`, alguns componentes usam `N` pontos e outros usam `N-1`;
- isso leva a `4N-2` coeficientes, e não `4N`;
- para `a/b \neq 1`, usa-se essencialmente a primeira regra, exceto pelo componente `z` ímpar, que omite o primeiro e o último ponto.

Esse detalhe é um dos lugares mais fáceis de errar ao codificar o método.

### 2.7 Formulação Dos Elementos Da Matriz

Com os campos escritos e os pontos de casamento escolhidos, o artigo organiza o problema em forma matricial. Primeiro vêm as equações de casamento dos campos longitudinais:

- `E^{LA} A = E^{LC} C`
- `H^{LB} B = H^{LD} D`

Depois vêm as equações dos campos tangenciais:

- `E^{TA} A + E^{TB} B = E^{TC} C + E^{TD} D`
- `H^{TA} A + H^{TB} B = H^{TC} C + H^{TD} D`

As matrizes `A`, `B`, `C` e `D` são vetores-coluna dos coeficientes modais. Já os elementos de matriz `e_{mn}` e `h_{mn}` são construídos com:

- fatores trigonométricos `S` e `C`;
- funções de Bessel e Bessel modificadas;
- termos derivados e termos angulares;
- fatores geométricos `R`, `T` e `r_m`.

As equações completas `(7a)`-`(7l)` estão consolidadas em [docs/goell_02_matrix_and_normalization.md](docs/goell_02_matrix_and_normalization.md).

### 2.8 Designação Dos Modos

Ao contrário do guia metálico retangular, em que faz sentido falar de modos TE e TM puros, no guia dielétrico retangular os modos são mistos. Por isso o artigo propõe outra nomenclatura.

A ideia é olhar o limite de:

- grande razão de aspecto;
- pequeno comprimento de onda;
- pequena diferença de índice.

Nesse limite, o campo elétrico transverso tende a se alinhar predominantemente com um dos eixos. Assim:

- o modo é chamado `E^y_{mn}` se o campo tende a ficar paralelo a `y`;
- o modo é chamado `E^x_{mn}` se o campo tende a ficar paralelo a `x`.

Os índices `m` e `n` indicam o número de máximos nas direções `x` e `y`, respectivamente.

Essa convenção é a que aparece nas Figs. 16-22.

### 2.9 Diferenças Entre Funções De Campo Elétrico E Magnético

O artigo chama atenção para um aspecto físico importante: em um guia dielétrico misto, os campos `E_t` e `H_t` não são necessariamente perpendiculares nem precisam ter a mesma variação espacial.

Em guias metálicos com modos TE e TM puros, os campos transversais elétrico e magnético costumam ser ortogonais e o fluxo de potência não muda de sinal ao atravessar o guia. Já no caso dielétrico misto:

- a impedância pode variar com a posição;
- `E_t` e `H_t` podem não ser ortogonais;
- o fluxo de potência pode até mudar de sinal em regiões do corte transversal.

O artigo observa, no entanto, três casos especiais em que esse comportamento se simplifica:

- em uma das regiões, quando `k_z` é aproximadamente igual à constante de propagação do meio correspondente;
- em toda parte, no limite de pequena diferença de índice;
- em toda parte, quando há simetria circular tanto da estrutura quanto dos modos.

### 2.10 Normalização

Goell introduz variáveis normalizadas para tornar as curvas de propagação mais fáceis de comparar.

Ele define uma variável de propagação normalizada e uma variável radial normalizada. Na notação de trabalho deste repositório:

- a variável vertical é chamada `P^2_paper`;
- a variável horizontal das curvas é chamada `B_paper`.

O objetivo é reescrever os argumentos das funções de Bessel em termos de quantidades adimensionais, eliminando parte da dependência explícita dos índices dos meios.

Esse é um dos trechos mais sensíveis para a implementação, e por isso toda a normalização está documentada separadamente em [docs/goell_02_matrix_and_normalization.md](docs/goell_02_matrix_and_normalization.md).

### 2.11 Método Computacional

Com a matriz montada, o problema modal é reduzido a:

$$
[Q][T] = 0
$$

e à condição

$$
\det[Q] = 0.
$$

O artigo diz que, para encontrar os valores de propagação normalizada:

- primeiro eram testados valores distribuídos uniformemente em `(0,1)`;
- depois o método de Newton refinava as raízes;
- para as curvas de propagação, em geral uma única iteração de Newton bastava;
- com cinco harmônicos, uma avaliação do determinante levava cerca de `0.1 s` em um IBM 360/65.

O texto também comenta que havia grande faixa dinâmica nos coeficientes da matriz, exigindo cuidados com underflow e overflow. Por isso o autor usou multiplicações por fatores positivos em linhas e colunas, o que não altera os zeros do determinante. Esse ponto ainda merece cautela na leitura fina do scan.

### 2.12 Configurações De Modo E Geração Das Figuras

Depois de obter os coeficientes modais, o artigo descreve como os padrões de campo eram calculados. Primeiro encontrava-se `k_z` pela condição do determinante. Em seguida, esse valor era substituído de volta nas equações lineares, e os coeficientes restantes eram obtidos por técnicas matriciais padrão, fixando um elemento do vetor de coeficientes como unidade.

Para visualizar os modos, o autor menciona:

- cortes radiais do campo;
- isolinhas geradas em computador;
- figuras de intensidade.

As figuras eram produzidas em um sistema gráfico baseado em tubo de raios catódicos. Uma única quadrante era calculado e depois replicado por simetria. As imagens de intensidade eram construídas a partir da densidade de potência, com quantização em 21 níveis. Para pequena diferença de índice, o artigo comenta que a faixa dinâmica das imagens, em termos do campo elétrico transverso, era da ordem de 400.

## 3. Resultados Do Artigo - Tradução De Trabalho

### 3.1 Acurácia

O artigo relata que foram executados muitos testes para estimar a acurácia. A Tabela I apresenta resultados para o primeiro modo em `B = 2`, com vários números de harmônicos e várias razões de aspecto.

Os números da tabela indicam que, para pequenas razões de aspecto, a convergência é muito rápida. Já para razões de aspecto maiores, a convergência piora. O próprio texto destaca o caso `a/b = 4`, em que a variação ao longo da tabela é da ordem de `1.5%` do intervalo total possível.

Daí o artigo conclui que:

- os resultados apresentados na sequência são, em geral, bons a `1%`;
- nos casos com harmônicos pares e `a/b \neq 1`, a precisão esperada é melhor que `2%`;
- a acurácia tende a piorar conforme a ordem modal aumenta, embora não de forma monotônica.

Tabela I, em forma resumida:

| Número de harmônicos | `a/b = 1` | `a/b = 2` | `a/b = 3` | `a/b = 4` |
| --- | ---: | ---: | ---: | ---: |
| 3 | 0.714 | 0.811 | 0.820 | 0.828 |
| 4 | 0.713 | 0.811 | 0.820 | 0.819 |
| 5 | 0.715 | 0.808 | 0.819 | 0.813 |
| 6 | 0.714 | 0.808 | 0.822 | 0.820 |
| 7 | 0.715 | 0.808 | 0.820 | 0.813 |
| 8 | 0.715 | 0.807 | 0.820 | 0.814 |
| 9 | 0.715 | 0.807 | 0.823 | 0.815 |
| variação | 0.2% | 0.4% | 0.4% | 1.5% |

### 3.2 Configurações De Modo

O artigo diz que o número de harmônicos usado nos padrões de campo também é de interesse. Embora essa questão não tenha sido explorada de forma exaustiva, o autor mostra comparações de figuras de intensidade com cinco e nove harmônicos e conclui que cinco já fornecem boa representação dos modos apresentados.

Esse é o motivo pelo qual o default do repositório continua sendo `N = 5`.

As Figs. 5 e 6 apresentam os seis primeiros modos para:

- razão de aspecto unitária com `B = 3` e diferença de índice de `0.01`;
- razão de aspecto 2 com `B = 2` e a mesma diferença de índice.

O texto afirma que os gráficos são todos de modos `E^y_{mn}` e que, para pequena diferença de índice, eles são praticamente indistinguíveis dos modos correspondentes `E^x_{mn}`. Em outras palavras, esses pares podem ser vistos como quase duplos degenerados, diferindo essencialmente por uma troca entre campos elétrico e magnético.

O artigo comenta ainda que os padrões de campo são mais complicados do que os do guia retangular metálico porque:

- os campos se estendem para fora da fronteira do guia;
- a forma dos padrões depende não só do formato geométrico, mas também de parâmetros do guia.

Os modos `E^y_{11}` e `E^x_{11}` são apontados como os mais simples. As figuras seguintes mostram orientações dos vetores elétricos e magnéticos, regiões de alta intensidade, linhas de campo, e como esses padrões se deformam com a razão de aspecto.

Pontos destacados pelo texto:

- para grande razão de aspecto, alguns modos se parecem muito com os do guia metálico retangular;
- à medida que `a/b` se aproxima de 1, certos modos passam a se acoplar e seus padrões migram para os mostrados na Fig. 9;
- as Figs. 10, 11 e 12 mostram que alguns modos mudam pouco com a razão de aspecto;
- a Fig. 13 é usada como exemplo de um padrão relativamente complexo obtido com um número pequeno de harmônicos.

As Figs. 14 e 15 mostram como a distribuição de intensidade varia com `P^2`. O artigo ressalta que, mesmo para valores relativamente pequenos de `P^2` e `\Delta n_r`, a maior parte da energia continua confinada ao núcleo. Isso sustenta a observação de Marcatili de que a energia nas regiões de canto pode ser pequena numa faixa ampla de parâmetros.

### 3.3 Curvas De Propagação

As curvas de propagação são o alvo mais direto deste repositório.

O artigo informa que, em todas as curvas calculadas:

- o eixo horizontal é a quantidade normalizada da eq. `(16)`;
- o eixo vertical é a quantidade normalizada da eq. `(11)`.

#### Fig. 16

A Fig. 16 trata do caso de diferença de índice tendendo a zero e razão de aspecto 1. O artigo apresenta os 16 primeiros modos e observa a existência de grupos degenerados.

Além disso, comenta que alguns pares permanecem quase degenerados perto do corte, mas se separam por causa das diferenças em seus padrões de campo. A interpretação física dada é que certos modos exigem maior componente longitudinal do campo elétrico por causa das reversões de sinal ao longo das linhas de campo.

#### Fig. 17

A Fig. 17 mostra os 12 primeiros modos para `a/b = 2` no limite de diferença de índice pequena. O texto destaca um ponto interessante: ocorre cruzamento modal entre certos ramos, algo que não ocorreria em guias metálicos. Isso é possível porque as funções de campo dependem da frequência.

#### Fig. 18

A Fig. 18 mostra o efeito de diferença de índice finita para razão de aspecto unitária. Comparando-a com a Fig. 16, o artigo observa que os modos cujas linhas de campo se invertem ao atravessar a origem deixam de ser degenerados, enquanto outros ainda permanecem agrupados. Para quebrar todas as degenerescências, é preciso tanto diferença de índice finita quanto razão de aspecto diferente de 1.

#### Fig. 19

A Fig. 19 mostra justamente esse caso combinado: `a/b = 2` e diferença de índice finita. A separação entre ramos torna-se mais forte e o efeito conjunto dos dois parâmetros fica mais claro.

#### Fig. 20

A Fig. 20 compara os modos principais para vários valores de diferença de índice com razão de aspecto unitária. O texto afirma que, para circuitos ópticos na faixa `0 - 0.1`, a curva de diferença de índice nula é uma excelente aproximação. As maiores mudanças acontecem entre `0.1` e `10`, faixa vista como mais relevante para alguns problemas de micro-ondas.

#### Fig. 21

A Fig. 21 faz a mesma análise para `a/b = 2`. O artigo observa que o efeito da mudança de índice é muito mais forte sobre o modo `E^y_{11}` do que sobre `E^x_{11}`. Para o modo `E^y_{21}`, o efeito é relativamente pequeno, exceto perto do corte.

#### Fig. 22

A Fig. 22 mostra o efeito da razão de aspecto sobre os modos principais no limite de diferença de índice nula. O artigo inclui também a curva para razão de aspecto infinita, obtida da análise exata do guia em placa.

## 4. Conclusões - Tradução De Trabalho

O artigo conclui que o método de harmônicos circulares fornece resultados excelentes para guias de razão de aspecto moderada. A convergência é rápida e os resultados concordam com os de Marcatili nas regiões em que a aproximação dele se aplica. Para o modo principal, as curvas também concordam muito bem com as de Schlosser.

O autor compara explicitamente seu método com o de Marcatili e afirma que:

- ambos fornecem praticamente os mesmos valores de propagação normalizada, com diferença de poucos por cento, quando `P^2 > 0.5`;
- nessa faixa, o método de Marcatili é preferível por ser muito mais simples;
- para `P^2 < 0.5`, ou quando se deseja distinguir modos quase degenerados, é necessário usar outro método;
- o método de harmônicos circulares é particularmente atraente para `P^2` pequeno por tratar de forma natural a fronteira;
- para grandes diferenças de índice e `P^2` moderado, tanto o método de Goell quanto o de Schlosser podem ser usados.

## Como Este Texto Se Liga Ao Código

O mapeamento atual entre artigo e implementação é:

- expansões longitudinais: [docs/goell_01_field_expansions.md](docs/goell_01_field_expansions.md)
- campos transversais e elementos de matriz: [docs/goell_02_matrix_and_normalization.md](docs/goell_02_matrix_and_normalization.md)
- solver principal: [src/goell_q_solver.cpp](src/goell_q_solver.cpp)
- presets das curvas: [src/presets.sh](src/presets.sh)
- gráficos comparativos: [src/plot_compare.py](src/plot_compare.py)

O solver tenta seguir diretamente:

- eqs. `(1a)`-`(1d)` para `E_z` e `H_z`
- eqs. `(3a)`-`(3d)` para os campos transversais
- eqs. `(4a)`-`(4b)` para o campo tangencial
- eqs. `(6a)`-`(6d)` para o casamento matricial
- eqs. `(7a)`-`(7l)` para os elementos dos blocos
- eq. `(18)` para a montagem de `Q`
- eq. `(19)` para a condição modal

## O Que Já Parece Estável

- a separação entre paridade e família de fase;
- a estrutura de quatro classes modais;
- a regra especial de pontos para o caso `even` com `a/b \neq 1`;
- o uso de cinco harmônicos como default de trabalho;
- a organização dos CSVs e dos presets por figura.

## O Que Ainda Está Em Aberto

- a seleção correta dos ramos físicos nas curvas de propagação;
- a interpretação final dos mínimos muito próximos das bordas da variável normalizada;
- o ajuste fino da normalização para coincidir visualmente com as Figs. 16-19;
- a nota de escalonamento da p. 2144, que continua sendo a parte menos segura do scan.

## Próximos Passos Práticos

1. estabilizar a seleção dos ramos das Figs. 16-19;
2. validar a montagem dos blocos de `Q` com testes mais locais, em vez de apenas olhar a figura final;
3. só então avançar para as Figs. 20-22;
4. depois retomar as figuras de intensidade e configuração de campo, que exigem reconstrução dos coeficientes modais completos.

## Observação Final

Este README agora é deliberadamente mais longo do que o habitual porque ele foi transformado em um caderno de leitura do artigo, e não apenas em uma nota de uso do código. A intenção é que, daqui para frente, a teoria, a revisão do PDF e a implementação possam evoluir juntas, sem ficarmos repetindo a mesma reconstrução conceitual a cada nova rodada de testes.
