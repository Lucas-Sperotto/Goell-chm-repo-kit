/*
 * matrix.hpp — Montagem da matriz de casamento Q do método CHM.
 *
 * A condição modal do guia retangular é det(Q) = 0 [Eq. 8 / §2.7], onde Q é
 * a matriz de casamento de condições de contorno formulada em §2.3.
 *
 * Dimensões de Q:
 *   Paridade ímpar:  4N × 4N
 *   Paridade par:   (4N−2) × (4N−2)
 *
 * Leiaute de colunas [A|B|C|D]:
 *   A (offset_A): coeficientes aₙ — Ez interior  [Eq. 1a]
 *   B (offset_B): coeficientes bₙ — Hz interior  [Eq. 1b]
 *   C (offset_C): coeficientes cₙ — Ez exterior  [Eq. 1c]
 *   D (offset_D): coeficientes dₙ — Hz exterior  [Eq. 1d]
 *
 * Leiaute de linhas (4 grupos × N pontos):
 *   Grupo 1 — ez_long: continuidade de Ez  [Eqs. 7a-b]
 *   Grupo 2 — hz_long: continuidade de Hz  [Eqs. 7c-d]
 *   Grupo 3 — et_tan:  continuidade de Et  [Eqs. 7e-h]
 *   Grupo 4 — ht_tan:  continuidade de Ht  [Eqs. 7i-l]
 *
 * Referência: docs/02.3_formulacao_dos_elementos_de_matriz.md  docs/simbolos.md §"Blocos de Q".
 */

#ifndef GOELL_MATRIX_HPP
#define GOELL_MATRIX_HPP

#include "goell/common.hpp"

#include <Eigen/Dense>

// Monta e retorna a matriz Q para os parâmetros P, frequência B e propagação P'.
//   B      = 2b/λ₀·√(n²_r−1)  — frequência normalizada  [Eq. 6 / §2.6]
//   Pprime = P' ∈ (0,1)        — constante de propagação normalizada  [Eq. 6]
// Se P.rescale_matrix == true, Q é normalizada por linha e coluna antes de retornar.
Eigen::MatrixXd assemble_Q(const Params &P, double B, double Pprime);

#endif // GOELL_MATRIX_HPP
