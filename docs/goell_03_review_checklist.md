# Goell 1969 - Checklist De Revisao Cruzada Com O PDF

Este arquivo e curto de proposito. A ideia e usalo lado a lado com as equacoes renderizadas em:

- [goell_01_field_expansions.md](./goell_01_field_expansions.md)
- [goell_02_matrix_and_normalization.md](./goell_02_matrix_and_normalization.md)

## Itens Prioritarios Para Conferencia No Scan

1. Confirmar o glifo exato usado para a variavel vertical da eq. (11) e para o eixo vertical das Figs. 16-22.
   No repositorio ela aparece como `Pprime`, mas nas notas foi nomeada como `P^2_paper`.

2. Confirmar o glifo exato usado para a variavel horizontal da eq. (16) e para o eixo horizontal das Figs. 16-22.
   No repositorio ela aparece como `B`, mas o scan pode se parecer com beta ou com um `B` estilizado.

3. Confirmar a afirmacao da Secao 2.1 sobre as familias de fase:

   - primeiro tipo: `\phi_n = 0`, `\psi_n = \pi/2`
   - segundo tipo: `\phi_n = \pi/2`, `\psi_n = \pi`

4. Confirmar se as definicoes de `S` e `C` na p. 2140 usam mesmo o mesmo parametro `\phi` para ambas:

   $$
   S = \sin(n\theta_m + \phi),
   \qquad
   C = \cos(n\theta_m + \phi),
   \qquad
   \phi = 0 \ \text{or} \ \pi/2.
   $$

5. Confirmar os sinais em `(7e)`-(`7l`), especialmente:

   - o sinal de menos na frente de `(7e)`
   - o fator `k_0 Z_0` em `(7f)` e `(7h)`
   - o fator `\epsilon_r k_0 / Z_0` em `(7i)`
   - o sinal de menos na frente de `(7k)`

6. Confirmar as definicoes dos fatores angulares:

   $$
   \bar J = \frac{n J_n(h r_m)}{h^2 r_m},
   \qquad
   \bar K = \frac{n K_n(p r_m)}{p^2 r_m}.
   $$

7. Confirmar os fatores geometricos por trechos:

   - para `\theta < \theta_c`:
     `R = \sin\theta_m`, `T = \cos\theta_m`, `r_m = (a/2)\cos\theta_m`
   - para `\theta > \theta_c`:
     `R = -\cos\theta_m`, `T = \sin\theta_m`, `r_m = (b/2)\sin\theta_m`

8. Confirmar a regra especial da Secao 2.2 para harmonicos pares quando `a/b \neq 1`:

   - todos os pontos seguem a primeira formula,
   - exceto os primeiros e ultimos pontos do componente `z` impar, que sao omitidos.

9. Confirmar a estrutura em blocos de `Q` na eq. (18), especialmente os sinais de menos na terceira e quarta linhas de blocos.

10. Confirmar as formulas de escalonamento da p. 2144.
    Este continua sendo o trecho menos confiavel da extracao.

## Como Usar Este Checklist

Se voltarmos a revisar o PDF no futuro, o loop mais simples e:

- marcar qualquer item simbolicamente errado;
- marcar qualquer item numericamente equivalente, mas escrito de forma diferente do artigo;
- marcar qualquer glifo cujo nome deva mudar para bater com o paper.

Depois dessa passada, o proximo ajuste natural e levar a versao final das equacoes para dentro do solver e dos scripts de plot.
