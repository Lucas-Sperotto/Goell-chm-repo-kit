# IV. Conclusoes - O Que O Artigo Entrega E Como Ler Isso Hoje

## Traducao Comentada

Goell conclui que o metodo de harmonicos circulares fornece resultados excelentes para guias dielectricos retangulares de razao de aspecto moderada. Em particular:

- a convergencia com o numero de harmonicos e rapida;
- os resultados concordam com Marcatili onde a aproximacao de Marcatili e confiavel;
- os resultados tambem concordam bem com Schlosser nos modos principais.

O autor chama a atencao para uma comparacao importante:

- para valores maiores da variavel de propagacao normalizada, o metodo de Marcatili pode ser preferivel, porque entrega praticamente a mesma resposta com bem menos trabalho;
- para valores menores dessa variavel, isto e, mais perto do corte e das situacoes em que o campo externo pesa mais, o metodo de Goell se torna muito mais atraente.

Por fim, o artigo observa que, para grandes diferencas de indice e valores moderados da constante de propagacao normalizada, tanto o metodo de Goell quanto o de Schlosser podem ser usados.

## Comentario Fisico

Essa conclusao e mais sutil do que parece. Goell nao esta dizendo "meu metodo e sempre o melhor". O que ele mostra e algo mais honesto e mais util:

- existe um regime em que aproximacoes simples bastam;
- existe um regime em que a fronteira precisa ser tratada com mais fidelidade;
- o metodo de harmonicos circulares e especialmente forte nesse segundo regime.

Em outras palavras, o artigo nao vende um martelo universal. Ele identifica com precisao para que tipo de problema seu martelo foi feito.

## Comentario Computacional

Visto com olhos modernos, o artigo e notavel por tres razoes:

1. ele reduz um problema aberto de contorno a uma algebra linear organizada;
2. usa simetria de forma agressiva e inteligente;
3. entrega um fluxo reproduzivel: escolher a classe modal, montar `Q`, procurar `det(Q) = 0`, reconstruir campos.

Isso continua sendo uma excelente licao para alunos: boa modelagem numerica nao e apenas "rodar um solver", mas escolher uma representacao que capture a fisica certa com o menor custo possivel.

## O Que O Repositorio Acrescenta

O repositorio tenta levar esse espirito adiante em tres frentes:

- documentacao em portugues, para tornar a teoria estudavel;
- implementacao do solver, para tornar a teoria executavel;
- scripts de validacao, para testar a fidelidade do codigo contra o artigo.

Ao comparar os resultados do repositorio com o paper, aprendemos algo importante: mesmo quando a estrutura teorica esta correta, a identificacao numerica do ramo modal certo pode ser delicada. Isso nao diminui o valor do artigo; ao contrario, mostra como ele foi ambicioso e tecnicamente refinado.

## O Que Um Aluno Deve Levar Consigo

- O metodo de Goell e um metodo de casamento de contorno em base circular.
- O ganho principal esta no tratamento do problema aberto.
- As simetrias do problema sao essenciais para reduzir custo e organizar os modos.
- A comparacao com Marcatili e parte da mensagem do artigo, nao um detalhe secundario.
- Reproduzir as curvas e as tabelas do paper e uma excelente forma de aprender tanto eletromagnetismo quanto metodos numericos.

## Fechamento

Se eu tivesse de transformar a conclusao de Goell em uma mensagem de professor para aluno, seria esta:

> Quando a fronteira e a fisica do decaimento externo importam de verdade, vale a pena pagar o preco de uma formulacao mais rica. O metodo de harmonicos circulares e justamente essa troca entre sofisticao e fidelidade.
