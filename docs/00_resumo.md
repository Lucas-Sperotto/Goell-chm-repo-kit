# Goell 1969 - Guia De Estudo Do Repositorio

Este diretorio foi reorganizado para servir como uma leitura guiada do artigo de J. E. Goell, *A Circular-Harmonic Computer Analysis of Rectangular Dielectric Waveguides* (1969). A ideia nao e apenas traduzir o texto original, mas transformar o artigo em um conjunto de notas de estudo: ao ler estes arquivos, o estudante deve entender a fisica do problema, a matematica da formulacao e a logica computacional da implementacao.

## O Que O Artigo Faz

O problema central e encontrar os modos guiados de um guia de onda dieletrico retangular. Em um guia metalico retangular, a fronteira combina muito bem com coordenadas cartesianas. Em um guia dieletrico aberto, no entanto, o campo dentro do nucleo precisa se conectar a um campo evanescente no meio externo, e essa conexao ao longo de uma fronteira retangular nao e simples.

O passo conceitual de Goell e este:

1. expandir os campos longitudinais em harmonicos circulares;
2. usar funcoes de Bessel no interior e funcoes de Bessel modificadas no exterior;
3. impor o casamento dos campos tangenciais em pontos discretos da fronteira;
4. transformar o problema modal em uma condicao de determinante nulo.

Em linguagem moderna, o metodo e uma forma elegante de *point matching* baseada em uma base que respeita melhor a natureza aberta do problema do que uma expansao puramente cartesiana.

## O Que O Estudante Deve Aprender Aqui

Ao final desta leitura, a meta e que voce consiga responder com clareza:

- por que o problema pede funcoes de Bessel e nao apenas senos e cossenos;
- como a simetria reduz o numero de harmonicos e de pontos de casamento;
- como surgem os blocos da matriz `Q`;
- por que a condicao modal e `det(Q) = 0`;
- como a normalizacao do artigo produz as curvas das Figuras 16 a 22;
- por que a reproducao numerica pode gerar raizes fisicas e raizes espurias quando o truncamento harmonico nao esta sob controle.

## Como Ler Este Diretorio

- [01_intro.md](./01_intro.md): leitura comentada da introducao e do problema historico.
- [02_deriv.md](./02_deriv.md): o coracao teorico do artigo, com derivacao, simetria, matriz global e metodo numerico.
- [03_result.md](./03_result.md): leitura guiada da secao de resultados, incluindo Tabela I e Figuras 4 a 22.
- [04_conclusions.md](./04_conclusions.md): conclusoes do artigo em portugues, com comentario critico.

Os arquivos abaixo sao referencia compacta:

- [goell_01_field_expansions.md](./goell_01_field_expansions.md): expansoes de campo, simetria e pontos de casamento.
- [goell_02_matrix_and_normalization.md](./goell_02_matrix_and_normalization.md): elementos de matriz, normalizacao e equacao global.
- [goell_03_review_checklist.md](./goell_03_review_checklist.md): roteiro de conferencia de notacao e glifos do PDF.
- [table1_variation_notes.md](./table1_variation_notes.md): nota do repositorio sobre a reproducao numerica da Tabela I.

## Como O Artigo Conversa Com O Codigo

O artigo entra no codigo principalmente por tres caminhos:

- `src/goell_q_solver.cpp`: montagem da matriz `Q`, escolha da classe de simetria, busca de raizes e avaliacao do determinante;
- `src/presets.sh`: execucoes padrao para figuras e testes;
- scripts de validacao em `src/`: reproducao da Tabela I, rastreamento de raizes e analise de variacao.

Ao longo destas notas, o objetivo nao e esconder a implementacao, mas mostrar exatamente onde a teoria toca o codigo.

## Uma Observacao Importante Sobre Notacao

O scan do artigo nao e perfeito e alguns glifos ficam visualmente ambiguos, em especial nas variaveis normalizadas usadas nas curvas de propagacao. Para nao interromper a leitura principal com duvidas de OCR, estas notas adotam duas regras:

- a variavel vertical das curvas sera chamada de `P^2_paper`;
- a variavel horizontal da eq. (16) sera chamada de `B_paper`.

No codigo do repositorio, essas quantidades aparecem sobretudo como `Pprime` e `B`. O importante pedagogicamente e entender o papel fisico dessas variaveis: uma mede a propagacao longitudinal normalizada; a outra mede o tamanho transversal normalizado do guia.

## O Espirito Destas Notas

O artigo de Goell e curto, mas bastante denso. Ele supoe familiaridade com:

- equacoes de Maxwell em meios isotropicos;
- funcoes de Bessel;
- condicoes de contorno eletromagneticas;
- algebra linear numerica;
- classificacao modal em guias de onda.

Estas notas tentam preencher exatamente esse espaco entre "ler o artigo" e "entender o artigo". O objetivo nao e substituir o PDF como documento historico, mas fazer dele algo estudavel.
