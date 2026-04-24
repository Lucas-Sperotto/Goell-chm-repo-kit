/*
 * boundary.hpp — Pontos de casamento e geometria da fronteira retangular.
 *
 * Para impor as condições de contorno na superfície do guia retangular, o CHM
 * utiliza N pontos de casamento (matching points) distribuídos no primeiro
 * quadrante da fronteira (§2.2).  Para cada ângulo θ, calcula-se:
 *   - r(θ): distância radial da origem ao ponto na borda do retângulo
 *   - R(θ), T(θ): projeções que convertem (E_r, E_θ) na componente tangencial
 *
 * A componente tangencial de E na borda retangular é [Eqs. 4a-b]:
 *   Caso θ < θ_c (face lateral x = a/2):   E_t = E_r·sin θ + E_θ·cos θ
 *   Caso θ > θ_c (face superior y = b/2):  E_t = −E_r·cos θ + E_θ·sin θ
 * onde θ_c = arctan(b/a) é o ângulo do canto no 1º quadrante.
 *
 * Referência: docs/02.2_selecao_dos_pontos_de_ajuste.md  §2.2 Eqs. (4a-b).
 */

#ifndef GOELL_BOUNDARY_HPP
#define GOELL_BOUNDARY_HPP

#include "goell/common.hpp"

#include <vector>

// ─── Ponto de casamento na fronteira retangular ───────────────────────────────
//
// Para o ângulo θ dado, armazena a posição em coordenadas cilíndricas e os
// coeficientes de projeção para calcular E_t = E_r·R + E_θ·T [Eqs. 4a-b].
struct BoundaryPoint
{
    double theta = 0.0; // ângulo azimutal do ponto de casamento (radianos)
    double r     = 0.0; // distância radial à borda do retângulo (unidades de b)

    // Coeficientes de projeção tangencial [Eqs. 4a-b e expressões análogas para H]:
    double R = 0.0; // peso de E_r em E_t: sin θ (face lateral) ou −cos θ (face superior)
    double T = 0.0; // peso de E_θ em E_t: cos θ (face lateral) ou  sin θ (face superior)
};

// Calcula o BoundaryPoint para o ângulo θ dado.
// geometry_mode controla como r(θ) é calculado (intersection ou literal).
BoundaryPoint boundary_point(double theta, double a_over_b, BoundaryGeometryMode geometry_mode);

// Distribuição de N pontos de casamento para HarmonicParity::odd.
// θₘ = (m − 1/2)·π/(2N),  m = 1…N  (§2.2, equidistantes em [0, π/2]).
std::vector<double> odd_case_thetas(int N);

// Distribuição de N pontos para o conjunto de "simetria par" (even symmetry).
// Mesma fórmula que odd_case_thetas; usado nos blocos de equações pares.
std::vector<double> even_symmetry_thetas(int N);

// Distribuição de N−1 pontos para o conjunto de "simetria ímpar" (odd symmetry).
// θₘ = (m − 1/2)·π/(2(N−1)),  m = 1…N−1.
// Usado quando o modo de paridade par exige um conjunto menor de pontos [§2.2].
std::vector<double> odd_symmetry_thetas(int N);

// Remove o primeiro e o último ponto de uma lista de ângulos.
// Necessário para os modos pares em guia retangular (a/b ≠ 1), onde os pontos
// θ=0 e θ=π/2 coincidiriam com os cantos e gerariam linhas LD repetidas.
std::vector<double> omit_first_last(const std::vector<double> &thetas);

// Retorna true se o tipo de equação usa os pontos de "simetria par" (theta_full).
// A escolha depende de qual componente de campo (Ez ou Hz) tem simetria par no
// ângulo θ para a família de fase dada [§2.1].
bool uses_even_symmetry_points(RowKind row_kind, PhaseFamily phase);

// Retorna RowKind::ez_long (phi0) ou RowKind::hz_long (phi90): o tipo de equação
// longitudinal que usa o conjunto menor de pontos no modo ímpar-par retangular.
RowKind odd_z_row_kind(PhaseFamily phase);

#endif // GOELL_BOUNDARY_HPP
