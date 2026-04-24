/*
 * bessel.hpp — Funções de Bessel Jₙ, Kₙ e suas derivadas.
 *
 * As expansões de campo nas Eqs. (1a-d) do artigo usam:
 *   Jₙ(hr)  — função de Bessel de 1ª espécie, ordem n, no interior do núcleo
 *   Kₙ(pr)  — função de Bessel modificada de 2ª espécie, ordem n, no exterior
 *
 * As derivadas J'ₙ e K'ₙ (em relação ao argumento) aparecem nas componentes
 * transversais dos campos Eqs. (3a-d), convertidas em seguida para a
 * componente tangencial na borda retangular [Eqs. 4a-b].
 *
 * Referência: docs/02_derivacao_das_equacoes.md §2 Eqs. (1), (2), (3).
 */

#ifndef GOELL_BESSEL_HPP
#define GOELL_BESSEL_HPP

#include <cmath>

// Jₙ(x) — Bessel de 1ª espécie, ordem n  [Eq. 1a, 1b: campo no interior]
double Jn(int n, double x);

// Kₙ(x) — Bessel modificada de 2ª espécie, ordem n  [Eq. 1c, 1d: campo no exterior]
// Kₙ → 0 quando x → ∞ (campo evanescente no meio externo, condição de radiação).
double Kn(int n, double x);

// J'ₙ(x) = dJₙ/dx — derivada de Jₙ em relação ao argumento  [Eq. 3a-d via ∂/∂r]
// Relação de recorrência: J'₀ = −J₁;  J'ₙ = (Jₙ₋₁ − Jₙ₊₁)/2.
double Jn_prime(int n, double x);

// K'ₙ(x) = dKₙ/dx — derivada de Kₙ em relação ao argumento  [Eq. 3a-d via ∂/∂r]
// Relação de recorrência: K'₀ = −K₁;  K'ₙ = −(Kₙ₋₁ + Kₙ₊₁)/2.
double Kn_prime(int n, double x);

#endif // GOELL_BESSEL_HPP
