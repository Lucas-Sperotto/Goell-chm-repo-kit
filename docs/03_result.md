# III. Resultados - Como Ler As Figuras E A Tabela Do Artigo

## O Que Esta Secao Faz

A Secao III do artigo tem tres objetivos:

1. estimar a precisao dos calculos;
2. mostrar configuracoes de campo e figuras de intensidade;
3. apresentar as curvas de propagacao.

Esta e a parte em que a teoria da Secao II encontra a validacao numerica. Para o estudante, ela serve como ponte entre equacao e fenomenologia.

## 3.1 Precisao

Goell informa que executou varias rodadas numericas para estimar a precisao dos resultados. O teste sintetico mais importante aparece na Tabela I, para o primeiro modo com `B = 2`.

### Tabela I Do Artigo

| Numero de harmonicos | `a/b = 1` | `a/b = 2` | `a/b = 3` | `a/b = 4` |
|:--|--:|--:|--:|--:|
| 3 | 0.714 | 0.811 | 0.820 | 0.828 |
| 4 | 0.713 | 0.811 | 0.820 | 0.819 |
| 5 | 0.715 | 0.808 | 0.819 | 0.813 |
| 6 | 0.714 | 0.808 | 0.822 | 0.820 |
| 7 | 0.715 | 0.808 | 0.820 | 0.813 |
| 8 | 0.715 | 0.807 | 0.820 | 0.814 |
| 9 | 0.715 | 0.807 | 0.823 | 0.815 |
| Variacao | 0.2% | 0.4% | 0.4% | 1.5% |

### O Que A Tabela Esta Dizendo

A mensagem numerica da tabela e:

- para pequenas razoes de aspecto, a convergencia e muito rapida;
- para razoes de aspecto maiores, a convergencia piora, mas continua controlada;
- o uso de poucos harmonicos ja fornece numeros bastante estaveis.

Goell comenta que, para `a/b = 4`, a variacao total de `1.5%` sugere um erro de ordem de `3%` quando comparado ao limite de razao de aspecto infinita. Ainda assim, ele considera isso aceitavel porque existem solucoes exatas no limite de guia em lamina, e porque a perda de precisao para razao de aspecto grande nao compromete o metodo em seu regime de interesse principal.

### Interpretacao Didatica

Essa tabela ensina duas licoes importantes.

1. O truncamento harmonico e parte do metodo, nao um detalhe posterior.
2. Convergencia em problemas modais nao e necessariamente monotona; ela pode melhorar e piorar ligeiramente com `N`.

No repositorio, esta tabela e uma referencia central de validacao. Quando o solver reproduz mal a Tabela I, isso costuma indicar:

- erro na geometria dos pontos de fronteira;
- erro de classe modal;
- ou dificuldade em seguir o mesmo ramo fisico ao variar `N`.

## 3.2 Configuracoes De Modo

Depois da secao de precisao, o artigo passa a mostrar padroes de campo. Aqui aparecem as figuras de intensidade e as figuras de linhas de campo.

## Figuras 4, 5 E 6 - Intensidade Dos Modos

### Figura 4

A Figura 4 compara a imagem de intensidade do modo fundamental para cinco e nove harmonicos, com `a/b = 2`, `B = 2` e `\Delta n_r = 0.01`.

A conclusao de Goell e muito importante: cinco harmonicos ja fornecem uma representacao muito boa dos modos mostrados no artigo.

Comentario didatico: isso legitima o uso de bases relativamente pequenas. Nao porque o problema seja trivial, mas porque a escolha da base esta bem adaptada a ele.

### Figura 5

A Figura 5 mostra os seis primeiros modos para:

- `a/b = 1`
- `B = 3`
- `\Delta n_r = 0.01`

Todos sao modos do tipo `E_{mn}`. Goell comenta que, para pequeno contraste de indice, os conjuntos `E_{mn}` e `H_{mn}` quase se sobrepoem: os padroes sao muito parecidos, trocando-se essencialmente os papeis dos campos eletrico e magnetico.

### Figura 6

A Figura 6 repete a ideia para:

- `a/b = 2`
- `B = 2`
- `\Delta n_r = 0.01`

Aqui fica ainda mais claro que os modos de um guia dieletrico retangular nao sao simples deformacoes dos modos de um guia metalico retangular. Os lobos de intensidade podem se estender alem da fronteira e sua forma depende nao apenas da geometria, mas tambem do contraste de indice e da distancia ao corte.

## Figuras 7 A 13 - Linhas De Campo E Estrutura Modal

Nestas figuras, Goell passa da intensidade para a estrutura vetorial dos campos.

### Figura 7

Mostra a configuracao de campo do modo `E_{11}`. O artigo usa linhas escuras nas regioes de alto campo e linhas claras nas regioes de baixo campo. A ideia e destacar orientacao do campo eletrico e distribuicao espacial de energia.

### Figura 8

Apresenta os modos `E_{21}` e `E_{12}` para grande razao de aspecto, isto e, longe do caso quadrado. Nesse regime, os padroes lembram fortemente os de um guia metalico retangular.

### Figura 9

Mostra o caso quadrado. Aqui ocorre um fenomeno importante: modos que pareciam distintos em grande razao de aspecto se acoplam e se reorganizam quando `a/b` se aproxima de 1. O artigo ressalta especialmente a ligacao entre:

- `E_{12}` e `E_{21}`;
- `E_{31}` e `E_{13}`.

Comentario didatico: esse e um lembrete de que a classificacao modal continua util, mas a identidade fisica do modo pode mudar continuamente com o parametro geometrico.

### Figuras 10, 11 E 12

Mostram, respectivamente, configuracoes de campo para:

- `E_{22}`;
- `E_{31}`;
- `E_{13}`.

O interesse aqui e mostrar que a variedade topologica dos campos cresce rapidamente com a ordem modal.

### Figura 13

Mostra o modo `E_{32}` para razao de aspecto unitaria, incluindo:

- figura de intensidade;
- linhas de campo eletrico;
- linhas de campo magnetico;
- componentes TE/TM efetivas.

Essa figura e pedagogicamente preciosa, porque revela que uma base de poucos harmonicos circulares consegue reproduzir uma estrutura modal bastante rica.

## Figuras 14 E 15 - Variacao Com A Propagacao Normalizada

As Figuras 14 e 15 mostram como a distribuicao de intensidade muda com a variavel de propagacao normalizada para os modos `E_{11}` e `E_{21}`.

A leitura fisica e a seguinte:

- quando a propagacao normalizada diminui, o modo se espalha radialmente;
- quando ela aumenta, a energia fica mais confinada ao nucleo;
- mesmo para valores relativamente pequenos, grande parte da energia ainda pode permanecer dentro do nucleo.

Goell usa isso para reforcar um ponto importante: a hipotese de Marcatili, segundo a qual pouca energia vive na regiao dos cantos, e valida em uma faixa ampla, mas nao em toda a faixa de interesse.

## 3.3 Curvas De Propagacao

Aqui chegamos ao alvo principal do repositorio, porque e nesta parte que entram as Figuras 16 a 22.

O artigo plota:

- no eixo horizontal: `B_paper`, a altura normalizada do guia;
- no eixo vertical: `P^2_paper`, a propagacao longitudinal normalizada.

## Figura 16 - Caso Limite De Diferenca De Indice Nula E Razao De Aspecto Unitaria

Esta figura mostra os 16 primeiros modos para:

- `a/b = 1`
- `\Delta n_r \to 0`

O caso e especial porque existe forte degenerescencia modal. Goell lista grupos degenerados no limite quadrado de fraco guiamento. A mensagem fisica e clara:

- a simetria quadrada aumenta drasticamente as degenerescencias;
- pequenos desvios de geometria ou de contraste de indice podem quebrar esses grupos.

Para o repositorio, esta figura e um teste durissimo, porque qualquer erro de classificacao ou de rastreamento de ramo aparece logo.

## Figura 17 - Razao De Aspecto Dois E Diferenca De Indice Nula

Agora o guia deixa de ser quadrado:

- `a/b = 2`
- `\Delta n_r \to 0`

A maior parte das degenerescencias se quebra. O artigo observa um fenomeno especialmente interessante: ocorre um cruzamento modal entre `E_{31}` e `E_{12}`, algo impossivel em guias metalicos retangulares. A explicacao qualitativa envolve:

- reversoes de linha de campo no interior do nucleo;
- acoplamento entre padrao espacial e frequencia.

Comentario didatico: esta figura ensina que, em guias dielectricos, "parecido com guia metalico" nao significa "governado pela mesma topologia modal".

## Figura 18 - Razao De Aspecto Um E Contraste Finito

Aqui o guia volta a ser quadrado, mas o contraste de indice deixa de ser infinitesimal:

- `a/b = 1`
- `\Delta n_r = 0.5`

As curvas mostram como o contraste de indice desloca os ramos e modifica a separacao entre modos quase degenerados.

## Figura 19 - Razao De Aspecto Dois E Contraste Finito

Agora temos:

- `a/b = 2`
- `\Delta n_r = 0.5`

Esta figura combina os dois mecanismos de quebra de degenerescencia:

- mudanca de geometria;
- mudanca de contraste de indice.

Para a reproducao numerica, e um teste particularmente bom do equilibrio entre classificacao modal e continuidade de ramo.

## Figura 20 - Efeito Da Diferenca De Indice Nos Modos Principais, Com `a/b = 1`

Goell isola os modos principais e mostra suas curvas para varios valores de `\Delta n_r`.

A mensagem e:

- na faixa de interesse para circuitos opticos, o caso `\Delta n_r \to 0` ja e uma excelente aproximacao;
- as maiores diferencas aparecem numa faixa mais relevante para certos problemas de micro-ondas.

Comentario didatico: esta figura justifica por que o artigo trabalha tanto com variaveis normalizadas. Elas permitem comparar guias com contrastes diferentes em um mesmo diagrama.

## Figura 21 - Efeito Da Diferenca De Indice Nos Modos Principais, Com `a/b = 2`

A mesma ideia da Figura 20, agora para razao de aspecto dois. O artigo destaca que o efeito da diferenca de indice sobre o modo `E'_{11}` e muito mais forte do que sobre `E''_{11}`, exceto perto do corte.

Isso mostra que nem todos os modos "sentem" o contraste de indice da mesma forma.

## Figura 22 - Efeito Da Razao De Aspecto Nos Modos Principais, Com `\Delta n_r \to 0`

Por fim, Goell compara varias razoes de aspecto no regime de pequeno contraste. A conclusao e bastante pragmatica:

- para `P^2` maior que cerca de `0.5`, o metodo de Marcatili ja e muito bom;
- para `P^2` menor que isso, e especialmente perto do corte, a analise mais completa de Goell faz diferenca.

## O Que O Estudante Deve Tirar Da Secao III

- A Tabela I mostra que o metodo converge rapidamente para razoes de aspecto moderadas.
- As figuras de intensidade provam que poucos harmonicos ja descrevem bem os modos principais.
- As figuras de linhas de campo mostram que os modos sao hibridos e geometricamente ricos.
- As curvas de propagacao revelam degenerescencias, desdobramentos e cruzamentos que um modelo simplificado nao capta bem.

## Como Esta Secao Entra No Repositorio

No repositorio, a Secao III e a principal fonte de validacao:

- a Tabela I testa convergencia em `N`;
- as Figuras 16 a 19 testam a capacidade de localizar e rastrear ramos modais;
- as Figuras 20 a 22 testam a sensibilidade a `\Delta n_r` e a `a/b`.

Se a Secao II ensina como montar o problema, a Secao III ensina como julgar se o solver montado faz sentido.
