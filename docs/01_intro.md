# I. INTRODUÇÃO

Prevê-se que guias de onda dielétricos sejam utilizados como blocos fundamentais dos circuitos ópticos integrados. Esses guias podem servir não apenas como meio de transmissão para confinar e direcionar sinais ópticos, mas também como base para circuitos como filtros e acopladores direcionais. Assim, é importante ter um conhecimento aprofundado das propriedades de seus modos.

Guias de onda dielétricos circulares têm recebido considerável atenção, pois a geometria circular é comumente utilizada em fibras ópticas. Em muitas aplicações de óptica integrada, espera-se que os guias de onda consistam de um núcleo dielétrico retangular, ou quase retangular, embutido em um meio dielétrico de índice de refração ligeiramente menor. Os modos para essa geometria são mais difíceis de analisar do que aqueles de guias de onda metálicos retangulares devido à natureza da fronteira.

Marcatili, utilizando aproximações baseadas na suposição de que a maior parte da potência está confinada ao núcleo do guia de onda, derivou uma forma fechada a partir das propriedades de um guia de onda dielétrico retangular. Em sua solução, campos com variação senoidal no núcleo são ajustados a campos exponencialmente decrescentes no meio externo. Em cada região, apenas um único modo é utilizado. Os resultados desse método são obtidos em forma relativamente simples para avaliação numérica.

As propriedades do modo principal do guia de onda dielétrico retangular foram estudadas por Schlosser e Unger utilizando um computador digital de alta velocidade. Em sua abordagem, o plano transversal foi dividido em regiões, como mostrado na Figura 1, e soluções em coordenadas retangulares foram assumidas em cada uma dessas regiões. A constante de propagação longitudinal foi então ajustada de modo que a correspondência dos campos pudesse ser obtida em pontos discretos ao longo da fronteira. Esse método fornece resultados que, teoricamente, são válidos em uma faixa mais ampla do que os de Marcatili, porém com um aumento significativo na dificuldade computacional.

Uma deficiência do método é que, para um modo fixo, à medida que o comprimento de onda aumenta, o campo externo aumenta, de modo que o limite torna-se cada vez mais difícil para ajustar os campos ao longo das fronteiras entre as regiões e entre regiões adjacentes.

Uma abordagem variacional foi desenvolvida por Shaw e outros. Eles assumem uma solução de teste com dois ou três parâmetros variáveis no núcleo. A partir dessa solução de teste, os campos fora do núcleo são então derivados, e os parâmetros são ajustados para alcançar uma solução consistente. Essa abordagem, assim como a de Schlosser, exige cálculos complexos. Além disso, apresenta a desvantagem de que a função de teste deve ser assumida previamente. Adicionalmente, alguns dos resultados preliminares não apresentam o comportamento adequado para os casos limite (dimensões do guia de onda muito grandes ou muito pequenas em comparação com o comprimento de onda).

---

![Figura 1 – Dimensões e sistema de coordenadas](FIGURA_1_AQUI.png)

---

Na presente análise, a variação radial dos campos elétrico e magnético longitudinais dos modos é representada por uma soma de funções de Bessel dentro do núcleo do guia de onda e por uma soma de funções de Bessel modificadas fora do núcleo. As soluções são obtidas ajustando os campos ao longo do perímetro do núcleo. Assim, a correspondência na fronteira não é função dos parâmetros do guia de onda, de modo que a complexidade computacional não aumenta com o comprimento de onda.

A Seção II discute a teoria subjacente da análise harmônica circular de guias de onda dielétricos retangulares. Em seguida, apresenta-se uma descrição das técnicas computacionais e dos métodos gráficos especiais utilizados. A Seção III é dividida em três partes: a primeira descreve a precisão dos cálculos, a segunda descreve os padrões de campo e a terceira apresenta as curvas de propagação.
