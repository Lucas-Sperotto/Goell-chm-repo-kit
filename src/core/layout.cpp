/*
 * layout.cpp — Ordens harmônicas e leiaute de colunas da matriz Q.
 *
 * Contexto físico (artigo §2.1, Eqs. 1a-d):
 *   A expansão em harmônicos circulares inclui ordens n = 0, 1, 2, …
 *   A simetria do guia retangular restringe quais valores de n contribuem
 *   para cada família de modos:
 *
 *   Paridade ímpar (HarmonicParity::odd):
 *     Ez e Hz usam n = 1, 3, 5, … → nA = nB = N colunas por bloco.
 *     Tamanho total da matriz Q: 4N × 4N.
 *
 *   Paridade par (HarmonicParity::even):
 *     Família phi0:  Ez usa n = 2,4,…  (sem n=0, pois sin(0)=0)
 *                    Hz usa n = 0,2,4,…
 *     Família phi90: Ez usa n = 0,2,4,…
 *                    Hz usa n = 2,4,…  (sem n=0, pois cos(0·π/2)=const)
 *     nA ≠ nB, e ncols = 4N−2; tamanho da matriz: (4N−2) × (4N−2).
 *
 *   Referência: docs/02.1_efeitos_da_simetria.md  docs/simbolos.md §"Ordens harmônicas".
 */

#include "goell/layout.hpp"

#include <cmath>

// Gera as N primeiras ordens ímpares: 1, 3, 5, …, 2N−1.
std::vector<int> odd_orders(int N)
{
    std::vector<int> out;
    out.reserve(N);
    for (int k = 0; k < N; ++k)
        out.push_back(2 * k + 1);
    return out;
}

// Gera as N primeiras ordens pares: 0, 2, 4, …, 2(N−1).
std::vector<int> even_orders(int N)
{
    std::vector<int> out;
    out.reserve(N);
    for (int k = 0; k < N; ++k)
        out.push_back(2 * k);
    return out;
}

// Remove n = 0: sin(0·θ) = 0 em phi0, logo n=0 não contribui para Ez na família phi0.
std::vector<int> drop_zero(const std::vector<int> &orders)
{
    std::vector<int> out;
    for (int n : orders)
    {
        if (n != 0)
            out.push_back(n);
    }
    return out;
}

// S(n,θ) = fator angular que acompanha Ez na expansão [Eqs. 1a, 1c].
//   phi0  → sin(nθ):  Ez ∝ Jₙ(hr)·sin(nθ)·exp[i(kz·z−ωt)]
//   phi90 → cos(nθ):  Ez ∝ Jₙ(hr)·cos(nθ)·exp[i(kz·z−ωt)]
double S_term(int n, double theta, PhaseFamily phase)
{
    return (phase == PhaseFamily::phi0) ? std::sin(n * theta) : std::cos(n * theta);
}

// C(n,θ) = fator angular que acompanha Hz na expansão [Eqs. 1b, 1d].
//   phi0  → cos(nθ):   Hz ∝ Jₙ(hr)·cos(nθ)·exp[i(kz·z−ωt)]
//   phi90 → −sin(nθ):  (rotação de π/2 em φₙ desloca cos→−sin)
double C_term(int n, double theta, PhaseFamily phase)
{
    return (phase == PhaseFamily::phi0) ? std::cos(n * theta) : -std::sin(n * theta);
}

// Constrói o ColumnLayout para os parâmetros dados.
// As quatro colunas da estrutura [A|B|C|D] correspondem aos blocos
// de coeficientes {aₙ, bₙ, cₙ, dₙ} da expansão modal [Eqs. 1a-d].
ColumnLayout build_layout(const Params &P)
{
    ColumnLayout L;

    if (P.parity == HarmonicParity::odd)
    {
        // Paridade ímpar: Ez e Hz usam os mesmos harmônicos n=1,3,…,2N−1.
        // nA = nB = N  →  ncols = 4N.
        L.ez_orders = odd_orders(P.N);
        L.hz_orders = L.ez_orders;
    }
    else if (P.phase == PhaseFamily::phi0)
    {
        // Paridade par, família phi0:
        //   Ez usa n = 2,4,…  (n=0 excluído porque sin(0)=0)
        //   Hz usa n = 0,2,4,… (n=0 contribui, pois cos(0)=1)
        L.ez_orders = drop_zero(even_orders(P.N));
        L.hz_orders = even_orders(P.N);
    }
    else
    {
        // Paridade par, família phi90:
        //   Ez usa n = 0,2,4,… (cos(0·θ)=1 contribui)
        //   Hz usa n = 2,4,…  (n=0 excluído porque −sin(0)=0)
        L.ez_orders = even_orders(P.N);
        L.hz_orders = drop_zero(even_orders(P.N));
    }

    const int nA = static_cast<int>(L.ez_orders.size()); // colunas para Ez (A e C)
    const int nB = static_cast<int>(L.hz_orders.size()); // colunas para Hz (B e D)

    // Offsets sequenciais: [A | B | C | D] dentro de uma linha de Q.
    L.offset_A = 0;
    L.offset_B = L.offset_A + nA;
    L.offset_C = L.offset_B + nB;
    L.offset_D = L.offset_C + nA;
    L.ncols    = L.offset_D + nB; // = 2·nA + 2·nB = 4N (odd) ou 4N−2 (even)

    return L;
}
