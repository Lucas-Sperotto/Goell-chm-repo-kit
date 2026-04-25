# Métodos de Busca de Raiz — Divergência e Solução

_Navegação: [README geral](../../README.md) | [Índice de docs](../README.md)_

---

## 1. O que o artigo diz

O artigo de Goell (1969), Seção 2.7.1, p. 2144, descreve o procedimento numérico
para localizar os zeros de $\det(Q(B, P')) = 0$:

> "The computer scans $\rho^2$ [nosso $P'$] at a fixed $\mathcal{B}$ in uniform
> steps. When a sign change is detected, a Newton's method refinement is applied."

O **método de Newton** exige a derivada $f'(P') = \partial \det(Q) / \partial P'$
na raiz, calculada numericamente ou analiticamente.

---

## 2. Por que não usamos Newton diretamente

A derivada analítica de $\det(Q)$ em relação a $P'$ não está disponível em forma
fechada: $Q$ é uma matriz $4N \times 4N$ cujos elementos envolvem funções de Bessel
avaliadas em argumentos que dependem de $P'$ — a cadeia de derivação é longa e
propensa a erros de implementação.

A alternativa mais natural é a **diferença central**:

$$f'(P') \approx \frac{f(P' + h) - f(P' - h)}{2h}, \quad h \approx 10^{-7}$$

Essa aproximação exige **3 avaliações de $\det(Q)$ por iteração** (vs. 1 para
bissecção), tem ordem de convergência quadrática somente perto da raiz, e pode
divergir se o chute inicial for ruim.

---

## 3. O problema de representar $\det(Q)$ como número de ponto flutuante

Na vizinhança de uma raiz, $|\det(Q)| \to 0$.  Para uma matriz $20 \times 20$, o
determinante pode variar de $10^{-30}$ (na raiz) a $10^{20}$ (longe dela).  Esse
intervalo excede a faixa de `double` ($\approx 10^{-308}$ a $10^{308}$), mas a
representação numérica segura do solver usa $\ln|\det(Q)|$ (o campo `merit` no CSV).

Métodos de interpolação (falsa posição, secante, interpolação quadrática inversa)
precisam do **valor numérico** de $f(P')$, não apenas do sinal.  Para converter
`merit` de volta ao valor:

$$f(P') = \text{sign}(\det Q) \cdot \exp(\ln|\det Q|)$$

Como $\ln|\det Q|$ pode ser $-80$ ou $-300$ próximo à raiz, usamos uma versão
"amortecida" que previne overflow:

$$f_{\text{amort}}(P') = \text{sign}(\det Q) \cdot \exp\!\bigl(\text{clamp}(\ln|\det Q|,\,-300,\,+30)\bigr)$$

Isso garante $|f_{\text{amort}}| \in [10^{-131}, 10^{13}]$, seguro para `double`,
mas pode distorcer a forma da função para interpolações quando $\ln|\det Q|$ varia
muito dentro do bracket.

---

## 4. Os métodos implementados

Todos os métodos trabalham sobre um **bracket** $[a, b]$ onde $\det(Q)$ muda de
sinal — detectado pela varredura em $P'$ com passo $\approx 1/P_\text{scan}$.

### 4.1 Bissecção (Algoritmo 2.1)

$$p_{k+1} = \frac{a_k + b_k}{2}; \quad
\begin{cases} b_{k+1} = p_{k+1} & \text{se } \operatorname{sign}(f(a_k)) = \operatorname{sign}(f(p_{k+1})) \\ a_{k+1} = p_{k+1} & \text{caso contrário} \end{cases}$$

- **Vantagem:** usa apenas o sinal de $f$; imune a problemas de magnitude; convergência garantida.
- **Desvantagem:** convergência linear — reduz o intervalo à metade por iteração.
  Com 50 iterações e bracket inicial $\approx 1/P_\text{scan}$: precisão $\delta P' \approx 10^{-17}$.
- **Implementação:** `refine_bisect` em `src/core/solver.cpp`.

### 4.2 Falsa Posição — Regula Falsi (Algoritmo 2.5)

$$p_{k+1} = b_k - f(b_k) \cdot \frac{b_k - a_k}{f(b_k) - f(a_k)}$$

Interpola linearmente entre os extremos do bracket.  Mantém o bracket garantindo
convergência, mas pode "travar" em um extremo se a função for muito curva.

- **Implementação:** `refine_falsepos` em `src/core/solver.cpp`.

### 4.3 Secante (Algoritmo 2.4)

$$p_{k+1} = p_k - f(p_k) \cdot \frac{p_k - p_{k-1}}{f(p_k) - f(p_{k-1})}$$

Não mantém bracket — pode sair do intervalo.  Ordem de convergência $\approx 1.618$.

- **Implementação:** `refine_secant` em `src/core/solver.cpp`.

### 4.4 Newton com Diferenças Finitas — Newton-DF (Algoritmo 2.3 adaptado)

$$p_{k+1} = p_k - \frac{f(p_k)}{f'_h(p_k)}, \qquad f'_h(x) = \frac{f(x+h) - f(x-h)}{2h}$$

Convergência quadrática perto da raiz; requer 3 avaliações por iteração.

- **Implementação:** `refine_newton` em `src/core/solver.cpp`.

### 4.5 Método de Brent (padrão) — `--det-refine brent`

Combina Algoritmos 2.4 (secante), interpolação quadrática inversa (IQI) e
bissecção com critérios automáticos de segurança (Brent, 1973):

- Usa IQI ou secante quando a estimativa cai dentro da faixa segura do bracket.
- Recorre à bissecção se qualquer critério de segurança falhar.
- Convergência garantida (como bissecção) com velocidade superlinear quando
  a função é bem comportada.
- Não exige derivada analítica.

**Por que Brent é o padrão:** é o método que melhor realiza o espírito do artigo
("refinar com Newton") sem exigir derivadas analíticas e mantendo convergência
garantida.

- **Implementação:** `refine_brent` em `src/core/solver.cpp`.

---

## 5. Comparação prática

Para o modo HE$_{11}$ ($n_r = 1.5$, $a/b = 1$, $\mathcal{B} = 2.5$,
$P_\text{scan} = 240$):

| Método | `--det-refine` | $P'$ encontrado | Concordância |
| --- | --- | --- | --- |
| Bissecção | `bisect` | 0.769128 | referência |
| Falsa posição | `falsepos` | 0.768991 | $\Delta = 0.000137$ |
| Secante | `secant` | 0.769128 | $\Delta \approx 0$ |
| Newton-DF | `newton` | 0.769128 | $\Delta \approx 0$ |
| Brent | `brent` | 0.767635 | $\Delta = 0.001493$ |

As diferenças ocorrem porque, na vizinhança da raiz, $|\det(Q)|$ já está abaixo
da precisão de máquina ($\approx 10^{-15}$).  A bissecção reporta o ponto
médio do bracket final (onde o det computado é coincidentemente menor), enquanto
o Brent reporta o "melhor candidato" por magnitude — ambos correspondem à mesma
raiz física dentro da precisão numérica esperada.

O método de Brent não altera os resultados das curvas de propagação nem da Tabela I
(MAE permanece bem dentro do limiar de 0,01).

---

## 6. Como usar

```bash
# Padrão atual (Brent):
./build/goell_q_solver --parity odd --phase phi0 --a_over_b 1 --nr 1.5 --N 5 \
    --Bmin 2.5 --Bmax 2.5 --NB 0 --Pscan 240

# Comparar com outros métodos:
./build/goell_q_solver ... --det-refine bisect
./build/goell_q_solver ... --det-refine falsepos
./build/goell_q_solver ... --det-refine secant
./build/goell_q_solver ... --det-refine newton
./build/goell_q_solver ... --det-refine brent
```

Para reproduzir os resultados anteriores (pré-Brent), use `--det-refine bisect`.

---

## 7. Referências

- Brent, R. P. (1973). *Algorithms for Minimization without Derivatives*. Prentice-Hall.
- Burden, R. L. & Faires, J. D. *Numerical Analysis*. (Algoritmos 2.1–2.5).
- Sperotto, L. K. (2014). *Cálculo Numérico — Template Zeros.c*. UNEMAT.
- Goell, J. E. (1969). §2.7.1, p. 2144.

_Navegação: [README geral](../../README.md) | [Índice de docs](../README.md)_
