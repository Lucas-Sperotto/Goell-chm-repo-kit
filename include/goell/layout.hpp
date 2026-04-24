/*
 * layout.hpp — Estrutura de colunas da matriz Q e sequências de harmônicos.
 *
 * A matriz Q de casamento de condições de contorno (§2.3) tem colunas
 * organizadas em quatro blocos correspondentes aos coeficientes modais:
 *
 *   [  A  |  B  |  C  |  D  ]
 *    Ez_in  Hz_in  Ez_out Hz_out
 *
 * onde:
 *   A = {aₙ}: coeficientes de Jₙ em Ez interior  [Eq. 1a]
 *   B = {bₙ}: coeficientes de Jₙ em Hz interior  [Eq. 1b]
 *   C = {cₙ}: coeficientes de Kₙ em Ez exterior  [Eq. 1c]
 *   D = {dₙ}: coeficientes de Kₙ em Hz exterior  [Eq. 1d]
 *
 * A struct ColumnLayout armazena quais ordens n estão em cada bloco e os
 * offsets (índices de coluna) de cada bloco dentro de Q.
 *
 * As funções S_term / C_term calculam os fatores angulares sin(nθ+φₙ) e
 * cos(nθ+φₙ) que acompanham cada harmônico nas expansões [Eqs. 1a-d].
 *
 * Referência: docs/02.1_efeitos_da_simetria.md  docs/simbolos.md §"Coeficientes modais".
 */

#ifndef GOELL_LAYOUT_HPP
#define GOELL_LAYOUT_HPP

#include "goell/common.hpp"

#include <vector>

// ─── Layout de colunas da matriz Q ───────────────────────────────────────────
//
// Cada instância descreve a estrutura de Q para uma combinação (parity, phase).
// As ordens em ez_orders e hz_orders são as mesmas para paridade ímpar;
// diferem para paridade par por causa do n = 0 em Hz (§2.1).
struct ColumnLayout
{
    std::vector<int> ez_orders; // ordens n ativas para Ez (blocos A e C)
    std::vector<int> hz_orders; // ordens n ativas para Hz (blocos B e D)

    // Índice de início de cada bloco na matrix Q (colunas).
    // Leiaute: A=[offset_A, offset_B)  B=[offset_B, offset_C)  etc.
    int offset_A = 0; // início do bloco A (aₙ: Ez interior)
    int offset_B = 0; // início do bloco B (bₙ: Hz interior)
    int offset_C = 0; // início do bloco C (cₙ: Ez exterior)
    int offset_D = 0; // início do bloco D (dₙ: Hz exterior)
    int ncols    = 0; // total de colunas de Q = 4·N (odd) ou 4·N−2 (even)
};

// Retorna {1, 3, 5, …, 2N−1}: ordens ímpares para HarmonicParity::odd.
std::vector<int> odd_orders(int N);

// Retorna {0, 2, 4, …, 2(N−1)}: ordens pares para HarmonicParity::even.
std::vector<int> even_orders(int N);

// Remove o harmônico n = 0 da lista (Ez não tem modo n=0 na família phi0, §2.1).
std::vector<int> drop_zero(const std::vector<int> &orders);

// Fator angular S(n,θ) = sin(nθ+φₙ) para φ₀, cos(nθ+φₙ) para φ₉₀.
// Aparece em Ez [Eqs. 1a, 1c] e nas componentes tangenciais derivadas [Eq. 3].
double S_term(int n, double theta, PhaseFamily phase);

// Fator angular C(n,θ) = cos(nθ+φₙ) para φ₀, −sin(nθ+φₙ) para φ₉₀.
// Aparece em Hz [Eqs. 1b, 1d] e nas componentes tangenciais derivadas [Eq. 3].
double C_term(int n, double theta, PhaseFamily phase);

// Constrói o ColumnLayout completo a partir dos parâmetros de simulação.
// Determina ez_orders, hz_orders e todos os offsets de acordo com §2.1.
ColumnLayout build_layout(const Params &P);

#endif // GOELL_LAYOUT_HPP
