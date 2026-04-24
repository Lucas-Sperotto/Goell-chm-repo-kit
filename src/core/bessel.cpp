/*
 * bessel.cpp — Implementação das funções de Bessel Jₙ, Kₙ e derivadas.
 *
 * Contexto físico (artigo §2, Eqs. 1-3):
 *   Dentro do núcleo dielétrico (r < fronteira), a variação radial dos campos
 *   é dada por Jₙ(hr), onde h = √(k₁² − k_z²) é o número de onda transversal
 *   interno [Eq. 2a].  Jₙ é regular na origem (sem polo em r = 0).
 *
 *   Fora do núcleo (r > fronteira), a variação radial é dada por Kₙ(pr), onde
 *   p = √(k_z² − k₀²) é o número de onda transversal externo [Eq. 2b].
 *   Kₙ → 0 quando r → ∞ (modo guiado / campo confinado).
 *
 *   As derivadas J'ₙ e K'ₙ surgem ao calcular as componentes transversais
 *   E_r, E_θ, H_r, H_θ a partir de E_z e H_z [Eqs. 3a-d: operador ∂/∂r].
 *
 * Referência: docs/02_derivacao_das_equacoes.md  docs/simbolos.md §"Funções de Bessel".
 */

#include "goell/bessel.hpp"

// Jₙ(x): encapsula std::cyl_bessel_j do C++17 <cmath>.
double Jn(int n, double x) { return std::cyl_bessel_j(n, x); }

// Kₙ(x): encapsula std::cyl_bessel_k do C++17 <cmath>.
double Kn(int n, double x) { return std::cyl_bessel_k(n, x); }

// J'ₙ(x) via relação de recorrência [DLMF 10.6.1]:
//   J'₀ = −J₁
//   J'ₙ = (Jₙ₋₁ − Jₙ₊₁) / 2   para n ≥ 1
// Esta forma evita diferenças finitas e é numericamente estável.
double Jn_prime(int n, double x)
{
    if (n == 0)
        return -std::cyl_bessel_j(1, x);
    return 0.5 * (std::cyl_bessel_j(n - 1, x) - std::cyl_bessel_j(n + 1, x));
}

// K'ₙ(x) via relação de recorrência [DLMF 10.31.1]:
//   K'₀ = −K₁
//   K'ₙ = −(Kₙ₋₁ + Kₙ₊₁) / 2   para n ≥ 1
// O sinal negativo reflete o comportamento monotonamente decrescente de Kₙ.
double Kn_prime(int n, double x)
{
    if (n == 0)
        return -std::cyl_bessel_k(1, x);
    return -0.5 * (std::cyl_bessel_k(n - 1, x) + std::cyl_bessel_k(n + 1, x));
}
