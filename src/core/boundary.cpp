/*
 * boundary.cpp — Cálculo dos pontos de casamento na fronteira retangular.
 *
 * Contexto físico (artigo §2.2, Eqs. 4a-b):
 *   O guia retangular (largura a, altura b) tem quatro faces.  Por simetria,
 *   considera-se apenas o primeiro quadrante.  A fronteira é parametrizada
 *   pelo ângulo θ ∈ (0, π/2).  O ângulo de canto é:
 *
 *       θ_c = arctan(b/a) = arctan(1 / (a/b))
 *
 *   Para θ < θ_c: ponto está na face lateral   (x = a/2)
 *   Para θ > θ_c: ponto está na face superior  (y = b/2)
 *
 *   As componentes tangenciais do campo elétrico e magnético na borda são
 *   obtidas pela projeção de (E_r, E_θ) na direção tangente à face [Eqs. 4a-b]:
 *
 *     Face lateral (x = a/2):
 *       E_t = E_r·sin θ + E_θ·cos θ   → R = sin θ,  T = cos θ
 *
 *     Face superior (y = b/2):
 *       E_t = −E_r·cos θ + E_θ·sin θ  → R = −cos θ, T = sin θ
 *
 *   (Expressões análogas para H_t.  O sinal do bloco exterior vem do fato de
 *    k² − k_z² ser positivo no interior e negativo no exterior [Eqs. 3a-d].)
 *
 * Referência: docs/02.2_selecao_dos_pontos_de_ajuste.md  docs/simbolos.md §"Pontos de casamento".
 */

#include "goell/boundary.hpp"

#include <cmath>

// Calcula o BoundaryPoint completo para o ângulo θ e a razão a/b.
// As coordenadas r, R, T são em unidades onde b = 1 (altura normalizada).
BoundaryPoint boundary_point(double theta, double a_over_b, BoundaryGeometryMode geometry_mode)
{
    // θ_c = arctan(b/a): ângulo que a diagonal para o canto do 1º quadrante
    // forma com o eixo x.  Para a/b > 1 (guia largo), θ_c < π/4.
    const double theta_c = std::atan(1.0 / a_over_b);

    BoundaryPoint bp;
    bp.theta = theta;

    if (theta < theta_c)
    {
        // ── Face lateral: x = a/2 ─────────────────────────────────────────
        // Interseção do raio θ com a face x = a_over_b / 2 (em unidades de b):
        //   intersection: r·cos θ = a/2  →  r = (a/2)/cos θ
        //   literal:      r = (a/2)·cos θ  (modo experimental, ponto projetado)
        if (geometry_mode == BoundaryGeometryMode::literal)
            bp.r = (0.5 * a_over_b) * std::cos(theta);
        else
            bp.r = (0.5 * a_over_b) / std::cos(theta);  // geometria correta

        // Projeção tangencial [Eq. 4a]: tangente à face lateral = direção ŷ = sin θ·ê_r + cos θ·ê_θ
        bp.R = std::sin(theta);  // coeficiente de E_r em E_t
        bp.T = std::cos(theta);  // coeficiente de E_θ em E_t
    }
    else
    {
        // ── Face superior: y = b/2 ────────────────────────────────────────
        // Interseção do raio θ com a face y = 0.5 (em unidades de b):
        //   intersection: r·sin θ = 1/2  →  r = 0.5/sin θ
        //   literal:      r = 0.5·sin θ  (modo experimental)
        if (geometry_mode == BoundaryGeometryMode::literal)
            bp.r = 0.5 * std::sin(theta);
        else
            bp.r = 0.5 / std::sin(theta);  // geometria correta

        // Projeção tangencial [Eq. 4b]: tangente à face superior = direção x̂ = cos θ·ê_r − sin θ·ê_θ
        // O sinal negativo em R vem do artigo [Eq. 4b]: E_t = −E_r·cos θ + E_θ·sin θ.
        bp.R = -std::cos(theta); // coeficiente de E_r em E_t
        bp.T =  std::sin(theta); // coeficiente de E_θ em E_t
    }

    return bp;
}

// N pontos equidistantes em (0, π/2) para HarmonicParity::odd:
//   θₘ = (m − 1/2)·π/(2N),  m = 1…N
// Estes pontos evitam os ângulos extremos 0 e π/2 (singularidades geométricas).
std::vector<double> odd_case_thetas(int N)
{
    std::vector<double> thetas;
    thetas.reserve(N);
    for (int m = 1; m <= N; ++m)
        thetas.push_back((m - 0.5) * PI / (2.0 * N));
    return thetas;
}

// N pontos para o conjunto de "simetria par" — mesma fórmula que odd_case_thetas.
// Usado nas linhas de Q que impõem condições onde o campo tem paridade par em θ.
std::vector<double> even_symmetry_thetas(int N)
{
    std::vector<double> thetas;
    thetas.reserve(N);
    for (int m = 1; m <= N; ++m)
        thetas.push_back((m - 0.5) * PI / (2.0 * N));
    return thetas;
}

// N−1 pontos para o conjunto de "simetria ímpar":
//   θₘ = (m − 1/2)·π/(2(N−1)),  m = 1…N−1
// Usado quando a componente de campo tem paridade ímpar e a matriz precisar
// de um ponto a menos para ter posto completo (paridade even, §2.2).
std::vector<double> odd_symmetry_thetas(int N)
{
    std::vector<double> thetas;
    if (N <= 1)
        return thetas;
    thetas.reserve(N - 1);
    for (int m = 1; m <= N - 1; ++m)
        thetas.push_back((m - 0.5) * PI / (2.0 * (N - 1)));
    return thetas;
}

// Remove o primeiro e o último ângulo da lista.
// Necessário no modo "paper" para guia retangular com paridade even: os ângulos
// θ = 0 e θ = π/2 correspondem a cantos onde Ez longitudinal não é distinto de Hz,
// o que geraria linhas linearmente dependentes na matriz Q.
std::vector<double> omit_first_last(const std::vector<double> &thetas)
{
    if (thetas.size() <= 2)
        return {};
    return std::vector<double>(thetas.begin() + 1, thetas.end() - 1);
}

// Determina qual conjunto de θ (even ou odd) usar para um dado tipo de linha.
// A regra segue §2.1: a simetria par em θ corresponde ao campo que aparece
// com cos(nθ) na expansão, e ímpar ao que aparece com sin(nθ).
//   phi0:  Hz usa cos → simetria par (theta_full);  Ez usa sin → simetria ímpar
//   phi90: Ez usa cos → simetria par;               Hz usa sin → simetria ímpar
bool uses_even_symmetry_points(RowKind row_kind, PhaseFamily phase)
{
    if (phase == PhaseFamily::phi0)
        return row_kind == RowKind::hz_long || row_kind == RowKind::et_tan;
    return row_kind == RowKind::ez_long || row_kind == RowKind::ht_tan;
}

// Retorna o RowKind da equação longitudinal que usa o conjunto menor de pontos
// no modo "paper" para paridade even + retângulo geral.
// phi0  → ez_long usa N−2 pontos (omit_first_last de theta_full)
// phi90 → hz_long usa N−2 pontos
RowKind odd_z_row_kind(PhaseFamily phase)
{
    return (phase == PhaseFamily::phi0) ? RowKind::ez_long : RowKind::hz_long;
}
