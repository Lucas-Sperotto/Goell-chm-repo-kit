/*
 * matrix.cpp — Montagem da matriz de casamento Q (núcleo físico do CHM).
 *
 * ═══════════════════════════════════════════════════════════════════════════
 * CONTEXTO FÍSICO — ARTIGO GOELL (1969) §2.3
 * ═══════════════════════════════════════════════════════════════════════════
 *
 * O Método dos Harmônicos Circulares impõe as quatro condições de contorno
 * eletromagnéticas na superfície do núcleo retangular:
 *
 *   1. Continuidade de Ez  (componente longitudinal elétrica)
 *   2. Continuidade de Hz  (componente longitudinal magnética)
 *   3. Continuidade de Et  (componente tangencial elétrica)
 *   4. Continuidade de Ht  (componente tangencial magnética)
 *
 * Cada condição é avaliada em N pontos de casamento θₘ na fronteira do
 * primeiro quadrante.  Para P' e B fixos, isso gera um sistema linear
 * homogêneo Q·x = 0, onde x = [aₙ bₙ cₙ dₙ]ᵀ são os coeficientes modais.
 * A condição de modo guiado é det(Q) = 0  [Eq. 8].
 *
 * ═══════════════════════════════════════════════════════════════════════════
 * VARIÁVEIS NORMALIZADAS (§2.6, Eq. 6)
 * ═══════════════════════════════════════════════════════════════════════════
 *
 *   B  = 2b/λ₀ · √(n²_r − 1)       frequência normalizada
 *   P' = (k²_z/k²₀ − 1)/(n²_r − 1) constante de propagação normalizada, P' ∈ (0,1)
 *
 * Delas derivam:
 *   k_z/k₀  = √(1 + (n²_r − 1)·P')
 *   h·b = π·B·√(1 − P')             número de onda transversal interno × b  [Eq. 2a]
 *   p·b = π·B·√(P')                  número de onda transversal externo × b  [Eq. 2b]
 *
 * ═══════════════════════════════════════════════════════════════════════════
 * ESTRUTURA DOS ELEMENTOS DE MATRIZ (§2.3, Eqs. 7a-l)
 * ═══════════════════════════════════════════════════════════════════════════
 *
 * Para cada harmônico n e ponto de casamento θₘ, os elementos de Q são
 * calculados a partir das componentes de campo cilíndricas [Eqs. 3a-d]
 * projetadas na direção tangente à face retangular [Eqs. 4a-b].
 *
 * Definições intermediárias (para o n-ésimo harmônico no ponto bp):
 *
 *   J₀  = Jₙ(hr)           Bessel interior [Eq. 1a]
 *   K₀  = Kₙ(pr)           Bessel exterior [Eq. 1c]
 *   Jr  = J'ₙ(hr) / h      derivada radial normalizada (≡ ∂Jₙ/∂r dividido por h²)
 *   Kr  = K'ₙ(pr) / p      derivada radial normalizada
 *   Jth = n·Jₙ(hr)/(h²·r)  fator azimutal normalizado (de ∂E_z/∂θ ÷ h²)
 *   Kth = n·Kₙ(pr)/(p²·r)  fator azimutal normalizado (exterior)
 *   S   = sin(nθ+φₙ)        fator angular de Ez [S_term]
 *   C   = cos(nθ+φₙ)        fator angular de Hz [C_term]
 *   R   = componente de E_r em E_t (sin θ ou −cos θ) [Eqs. 4a-b]
 *   T   = componente de E_θ em E_t (cos θ ou  sin θ) [Eqs. 4a-b]
 *
 * Elementos dos 12 sub-blocos da matriz Q [Eqs. 7a-l]:
 *
 *   eLA = J₀·S                              Ez interior     [Eq. 7a]
 *   eLC = K₀·S                              Ez exterior     [Eq. 7b]
 *   hLB = J₀·C                              Hz interior     [Eq. 7c]
 *   hLD = K₀·C                              Hz exterior     [Eq. 7d]
 *
 *   eTA = −(k_z/k₀)·(Jr·S·R + Jth·C·T)     Et de Ez int.   [Eq. 7e]
 *   eTC = +(k_z/k₀)·(Kr·S·R + Kth·C·T)     Et de Ez ext.   [Eq. 7f]
 *   eTB = +(Jth·S·R + Jr·C·T)              Et de Hz int.   [Eq. 7g]
 *   eTD = −(Kth·S·R + Kr·C·T)              Et de Hz ext.   [Eq. 7h]
 *
 *   hTA = +ε_r·(Jth·C·R − Jr·S·T)          Ht de Ez int.   [Eq. 7i]
 *   hTC = −(Kth·C·R − Kr·S·T)              Ht de Ez ext.   [Eq. 7j]
 *   hTB = −(k_z/k₀)·(Jr·C·R − Jth·S·T)     Ht de Hz int.   [Eq. 7k]
 *   hTD = +(k_z/k₀)·(Kr·C·R − Kth·S·T)     Ht de Hz ext.   [Eq. 7l]
 *
 * Nota sobre sinais: cada linha de Q impõe campo_interior − campo_exterior = 0.
 * Por isso as contribuições dos blocos C e D entram com sinal negativo em Q:
 *   Q(row, colA) = +eLA;   Q(row, colC) = −eLC   →  (eLA·aₙ − eLC·cₙ = 0)
 *
 * O fator ε_r = n²_r em hTA vem do termo k²₁/h² nas Eqs. (3c-d) para o interior,
 * em contraste com k²₀/p² no exterior (que produz hTC sem ε_r).
 *
 * Referência: docs/02.3_formulacao_dos_elementos_de_matriz.md  docs/simbolos.md §"Blocos de Q".
 */

#include "goell/matrix.hpp"

#include "goell/bessel.hpp"
#include "goell/boundary.hpp"
#include "goell/layout.hpp"

#include <algorithm>
#include <cmath>
#include <vector>

using Eigen::MatrixXd;

namespace
{

// Garante denominador ≥ EPS para evitar divisão por zero em h=0 ou p=0.
// Ocorre apenas nos extremos P'→0 (h→0) ou P'→1 (p→0), que não são modos guiados.
double safe_denominator(double x)
{
    return (x < EPS) ? EPS : x;
}

// Normaliza cada linha de Q pelo seu valor máximo absoluto.
// Melhora o condicionamento numérico ao evitar disparidade de escala entre
// os sub-blocos de Ez (que usa Jₙ) e Ht (que envolve derivadas e ε_r).
void row_rescale(MatrixXd &Q)
{
    for (int i = 0; i < Q.rows(); ++i)
    {
        double max_abs = 0.0;
        for (int j = 0; j < Q.cols(); ++j)
            max_abs = std::max(max_abs, std::fabs(Q(i, j)));
        if (max_abs > 0.0)
            Q.row(i) /= max_abs;
    }
}

// Normaliza cada coluna de Q pelo seu valor máximo absoluto.
// Complementa a row_rescale para balancear as escalas dos coeficientes
// aₙ (Bessel J pequeno para hr << 1) versus cₙ (Bessel K possivelmente grande).
void col_rescale(MatrixXd &Q)
{
    for (int j = 0; j < Q.cols(); ++j)
    {
        double max_abs = 0.0;
        for (int i = 0; i < Q.rows(); ++i)
            max_abs = std::max(max_abs, std::fabs(Q(i, j)));
        if (max_abs > 0.0)
            Q.col(j) /= max_abs;
    }
}

// ─── fill_ez_columns ─────────────────────────────────────────────────────────
//
// Preenche os blocos A (interior) e C (exterior) de uma linha de Q.
// Estes blocos correspondem aos coeficientes {aₙ} e {cₙ} da expansão de Ez
// [Eqs. 1a, 1c].  Para cada harmônico n, calcula os três possíveis elementos
// dependendo do tipo de condição de contorno (row_kind):
//
//   ez_long → eLA [Eq. 7a], eLC [Eq. 7b]
//   et_tan  → eTA [Eq. 7e], eTC [Eq. 7f]
//   ht_tan  → hTA [Eq. 7i], hTC [Eq. 7j]
//
// Parâmetros de escala:
//   h_scaled = h·b  (número de onda transversal interno em unidades de 1/b)
//   p_scaled = p·b  (número de onda transversal externo em unidades de 1/b)
//   kz_over_k0 = k_z/k₀
void fill_ez_columns(
    MatrixXd &Q,
    int row,
    RowKind row_kind,
    const ColumnLayout &L,
    const Params &P,
    const BoundaryPoint &bp,
    double kz_over_k0,
    double h_scaled,
    double p_scaled)
{
    const double eps_r = P.n_r * P.n_r; // ε_r = n²_r = ε₁/ε₀

    for (int i = 0; i < static_cast<int>(L.ez_orders.size()); ++i)
    {
        const int n = L.ez_orders[i];

        // Fatores angulares da expansão de Ez e Hz [Eqs. 1a-d]
        const double S = S_term(n, bp.theta, P.phase); // sin(nθ+φₙ) ou cos(nθ+φₙ)
        const double C = C_term(n, bp.theta, P.phase); // cos(nθ+φₙ) ou −sin(nθ+φₙ)

        // Argumentos das funções de Bessel: hr e pr (adimensionais)
        const double hr = h_scaled * bp.r; // h · r  (ponto interno)
        const double pr = p_scaled * bp.r; // p · r  (ponto externo)

        // Valores das funções de Bessel no ponto de casamento
        const double J0 = Jn(n, hr); // Jₙ(hr)
        const double K0 = Kn(n, pr); // Kₙ(pr)

        // Derivadas radiais normalizadas: J'ₙ(hr)/h e K'ₙ(pr)/p
        // Surgem de ∂/∂r = h·d/d(hr) nas Eqs. (3a-d).
        const double Jr = Jn_prime(n, hr) / safe_denominator(h_scaled);
        const double Kr = Kn_prime(n, pr) / safe_denominator(p_scaled);

        // Fatores azimutais normalizados: n·Jₙ(hr)/(h²·r) e n·Kₙ(pr)/(p²·r)
        // Surgem de (1/r)·∂/∂θ = (n/r)·cos(nθ) / Jₙ nas Eqs. (3a-d).
        // Para n=0: ∂/∂θ = 0, portanto Jth = Kth = 0.
        const double Jth = (n == 0) ? 0.0 : (n * J0) / (safe_denominator(h_scaled * h_scaled) * bp.r);
        const double Kth = (n == 0) ? 0.0 : (n * K0) / (safe_denominator(p_scaled * p_scaled) * bp.r);

        // ── Elementos longitudinais [Eqs. 7a, 7b] ────────────────────────
        // Ez = Jₙ(hr)·S·aₙ  (interior) e  Ez = Kₙ(pr)·S·cₙ  (exterior)
        const double eLA = J0 * S; // Ez interior: contribuição do harmônico aₙ  [Eq. 7a]
        const double eLC = K0 * S; // Ez exterior: contribuição do harmônico cₙ  [Eq. 7b]

        // ── Elementos de Et tangencial de Ez [Eqs. 7e, 7f] ───────────────
        // Derivados de E_t = E_r·R + E_θ·T com E_r, E_θ de Eqs. (3a,3b):
        //   E_r (Ez) ∝ −(k_z/h²)·h·J'ₙ·S  →  −kz/k0·Jr·S
        //   E_θ (Ez) ∝ −(k_z/h²)·(n/r)·Jₙ·C  →  −kz/k0·Jth·C
        // (sinal do exterior inverte porque k² − k²_z = −p² < 0)
        const double eTA = -kz_over_k0 * (Jr * S * bp.R + Jth * C * bp.T); // [Eq. 7e]
        const double eTC = +kz_over_k0 * (Kr * S * bp.R + Kth * C * bp.T); // [Eq. 7f]

        // ── Elementos de Ht tangencial de Ez [Eqs. 7i, 7j] ───────────────
        // Derivados de H_t = H_r·R + H_θ·T com H_r, H_θ de Eqs. (3c,3d):
        //   H_r (Ez) ∝ −(k²/(μ₀ω·k_z·r))·(n/r)·Jₙ·C  →  −ε_r·Jth·C
        //   H_θ (Ez) ∝  (k²/(μ₀ω·k_z))·h·J'ₙ·S        →   ε_r·Jr·S
        // O fator ε_r vem de k²₁ = n²_r·k²₀ no interior; exterior usa k²₀ (ε_r=1).
        const double hTA = +eps_r * (Jth * C * bp.R - Jr * S * bp.T); // [Eq. 7i]
        const double hTC = -(Kth * C * bp.R - Kr * S * bp.T);         // [Eq. 7j]

        const int colA = L.offset_A + i; // coluna do coeficiente aₙ
        const int colC = L.offset_C + i; // coluna do coeficiente cₙ

        // Preenche a linha segundo o tipo de condição de contorno.
        // Sinal negativo em colC: equação = (interior) − (exterior) = 0.
        if (row_kind == RowKind::ez_long)
        {
            Q(row, colA) = eLA;  // [Eq. 7a]
            Q(row, colC) = -eLC; // [Eq. 7b]
        }
        else if (row_kind == RowKind::et_tan)
        {
            Q(row, colA) = eTA;  // [Eq. 7e]
            Q(row, colC) = -eTC; // [Eq. 7f]
        }
        else if (row_kind == RowKind::ht_tan)
        {
            Q(row, colA) = hTA;  // [Eq. 7i]
            Q(row, colC) = -hTC; // [Eq. 7j]
        }
    }
}

// ─── fill_hz_columns ─────────────────────────────────────────────────────────
//
// Preenche os blocos B (interior) e D (exterior) de uma linha de Q.
// Estes blocos correspondem aos coeficientes {bₙ} e {dₙ} da expansão de Hz
// [Eqs. 1b, 1d].  Para cada harmônico n, calcula:
//
//   hz_long → hLB [Eq. 7c], hLD [Eq. 7d]
//   et_tan  → eTB [Eq. 7g], eTD [Eq. 7h]
//   ht_tan  → hTB [Eq. 7k], hTD [Eq. 7l]
//
// Nota: as contribuições de Hz para Et e Ht não carregam o fator ε_r (a
// permeabilidade é μ₀ em ambos os meios, então não há assimetria de μ).
void fill_hz_columns(
    MatrixXd &Q,
    int row,
    RowKind row_kind,
    const ColumnLayout &L,
    const Params &P,
    const BoundaryPoint &bp,
    double kz_over_k0,
    double h_scaled,
    double p_scaled)
{
    for (int i = 0; i < static_cast<int>(L.hz_orders.size()); ++i)
    {
        const int n = L.hz_orders[i];

        // Fatores angulares da expansão de Hz [Eqs. 1b, 1d]
        const double S = S_term(n, bp.theta, P.phase);
        const double C = C_term(n, bp.theta, P.phase);

        const double hr = h_scaled * bp.r;
        const double pr = p_scaled * bp.r;

        const double J0 = Jn(n, hr);
        const double K0 = Kn(n, pr);

        const double Jr  = Jn_prime(n, hr) / safe_denominator(h_scaled);
        const double Kr  = Kn_prime(n, pr) / safe_denominator(p_scaled);
        const double Jth = (n == 0) ? 0.0 : (n * J0) / (safe_denominator(h_scaled * h_scaled) * bp.r);
        const double Kth = (n == 0) ? 0.0 : (n * K0) / (safe_denominator(p_scaled * p_scaled) * bp.r);

        // ── Elementos longitudinais [Eqs. 7c, 7d] ────────────────────────
        // Hz = Jₙ(hr)·C·bₙ  (interior)  e  Hz = Kₙ(pr)·C·dₙ  (exterior)
        const double hLB = J0 * C; // Hz interior  [Eq. 7c]
        const double hLD = K0 * C; // Hz exterior  [Eq. 7d]

        // ── Elementos de Et tangencial de Hz [Eqs. 7g, 7h] ───────────────
        // Derivados de E_t = E_r·R + E_θ·T com E_r, E_θ de Eqs. (3a,3b):
        //   E_r (Hz) ∝ +(μ₀ω/k_z)·(n/r)·Jₙ·S  →  +Jth·S
        //   E_θ (Hz) ∝ −(μ₀ω/k_z)·h·J'ₙ·C     →  −Jr·C   (via h·d/d(hr))
        // (sem fator ε_r: μ é uniforme)
        const double eTB = +(Jth * S * bp.R + Jr * C * bp.T);  // [Eq. 7g]
        const double eTD = -(Kth * S * bp.R + Kr * C * bp.T);  // [Eq. 7h]

        // ── Elementos de Ht tangencial de Hz [Eqs. 7k, 7l] ───────────────
        // Derivados de H_t = H_r·R + H_θ·T com H_r, H_θ de Eqs. (3c,3d):
        //   H_r (Hz) ∝ +h·J'ₙ·C / h²  →  +Jr·C
        //   H_θ (Hz) ∝ −(n/r)·Jₙ·S / h²  →  −Jth·S
        const double hTB = -kz_over_k0 * (Jr * C * bp.R - Jth * S * bp.T); // [Eq. 7k]
        const double hTD = +kz_over_k0 * (Kr * C * bp.R - Kth * S * bp.T); // [Eq. 7l]

        const int colB = L.offset_B + i; // coluna do coeficiente bₙ
        const int colD = L.offset_D + i; // coluna do coeficiente dₙ

        if (row_kind == RowKind::hz_long)
        {
            Q(row, colB) = hLB;  // [Eq. 7c]
            Q(row, colD) = -hLD; // [Eq. 7d]
        }
        else if (row_kind == RowKind::et_tan)
        {
            Q(row, colB) = eTB;  // [Eq. 7g]
            Q(row, colD) = -eTD; // [Eq. 7h]
        }
        else if (row_kind == RowKind::ht_tan)
        {
            Q(row, colB) = hTB;  // [Eq. 7k]
            Q(row, colD) = -hTD; // [Eq. 7l]
        }
    }
}

// Preenche uma linha completa de Q chamando fill_ez_columns e fill_hz_columns.
// Os blocos A/C (Ez) e B/D (Hz) são independentes dentro de uma mesma linha
// porque as condições de contorno são lineares nos coeficientes.
void append_row(
    MatrixXd &Q,
    int row,
    RowKind row_kind,
    const ColumnLayout &L,
    const Params &P,
    const BoundaryPoint &bp,
    double kz_over_k0,
    double h_scaled,
    double p_scaled)
{
    fill_ez_columns(Q, row, row_kind, L, P, bp, kz_over_k0, h_scaled, p_scaled);
    fill_hz_columns(Q, row, row_kind, L, P, bp, kz_over_k0, h_scaled, p_scaled);
}

} // namespace

// ─── assemble_Q ──────────────────────────────────────────────────────────────
//
// Monta a matriz Q completa para os parâmetros dados.
// O algoritmo percorre os quatro tipos de condição (RowKind) e, para cada um,
// itera sobre os pontos de casamento adequados, chamando append_row para
// preencher as N (ou N−1) linhas correspondentes.
//
// Após o preenchimento, se P.rescale_matrix == true, Q é normalizada por linha
// e coluna para melhorar o condicionamento numérico do cálculo de det(Q).
MatrixXd assemble_Q(const Params &P, double B, double Pprime)
{
    const ColumnLayout L = build_layout(P);

    const double eps_r = P.n_r * P.n_r;

    // ── Variáveis normalizadas [Eq. 6 / §2.6] ────────────────────────────
    // k_z/k₀ = √(1 + (n²_r − 1)·P')  — constante de propagação normalizada
    const double kz_over_k0 = std::sqrt(1.0 + (eps_r - 1.0) * Pprime);

    // radial_scale = π·B  (fator comum a h·b e p·b)
    // h·b = π·B·√(1 − P')   número de onda transversal interno × b  [Eq. 2a]
    // p·b = π·B·√(P')        número de onda transversal externo × b  [Eq. 2b]
    const double radial_scale = PI * std::max(0.0, B);
    const double h_scaled = radial_scale * std::sqrt(std::max(0.0, 1.0 - Pprime));
    const double p_scaled = radial_scale * std::sqrt(std::max(0.0, Pprime));

    MatrixXd Q;

    if (P.parity == HarmonicParity::odd)
    {
        // ── Paridade ímpar: matriz 4N × 4N ────────────────────────────────
        // N pontos equidistantes para todos os quatro tipos de condição.
        // Ordem das linhas: [ez_long | hz_long | et_tan | ht_tan],
        // cada grupo com N linhas (uma por ponto de casamento).
        const auto thetas = odd_case_thetas(P.N);
        Q = MatrixXd::Zero(4 * P.N, L.ncols);

        int row = 0;
        for (RowKind kind : {RowKind::ez_long, RowKind::hz_long, RowKind::et_tan, RowKind::ht_tan})
        {
            for (double theta : thetas)
            {
                const auto bp = boundary_point(theta, P.a_over_b, P.geometry_mode);
                append_row(Q, row++, kind, L, P, bp, kz_over_k0, h_scaled, p_scaled);
            }
        }
    }
    else
    {
        // ── Paridade par: matriz (4N−2) × (4N−2) ─────────────────────────
        // A redução de 2 linhas deve-se à exclusão de n=0 em um dos blocos
        // longitudinais [§2.1], que elimina 2 graus de liberdade.
        //
        // A distribuição de pontos depende da simetria de cada condição de
        // contorno (par ou ímpar em θ) e da razão a/b do guia [§2.2]:
        //
        //   Guia quadrado (a/b = 1):
        //     even_pts (N pontos)  para condições de simetria par em θ
        //     odd_pts  (N−1 pontos) para condições de simetria ímpar em θ
        //
        //   Guia retangular, modo "paper":
        //     theta_full (N pontos)  para a maioria das condições
        //     theta_odd_rect (N−2 pontos = omit_first_last) para a condição
        //     longitudinal ímpar (ez_long em phi0 ou hz_long em phi90)
        //
        //   Guia retangular, modo "square_split":
        //     mesmo esquema do guia quadrado
        const auto theta_full       = even_symmetry_thetas(P.N);
        const auto theta_odd_square = odd_symmetry_thetas(P.N);
        const auto theta_odd_rect   = omit_first_last(theta_full);
        const bool is_square = std::fabs(P.a_over_b - 1.0) <= 1e-12;

        Q = MatrixXd::Zero(4 * P.N - 2, L.ncols);

        int row = 0;
        for (RowKind kind : {RowKind::ez_long, RowKind::hz_long, RowKind::et_tan, RowKind::ht_tan})
        {
            const std::vector<double> *thetas = nullptr;
            if (is_square)
            {
                // Quadrado: alterna entre N e N−1 pontos conforme simetria de campo.
                const bool use_even_points = uses_even_symmetry_points(kind, P.phase);
                thetas = use_even_points ? &theta_full : &theta_odd_square;
            }
            else
            {
                if (P.even_rect_mode == EvenRectMatchingMode::paper)
                {
                    // Retângulo, modo "paper": a condição longitudinal ímpar
                    // usa N−2 pontos (exclui os extremos θ=0 e θ=π/2).
                    thetas = (kind == odd_z_row_kind(P.phase)) ? &theta_odd_rect : &theta_full;
                }
                else
                {
                    // Retângulo, modo "square_split": mesma lógica do quadrado.
                    const bool use_even_points = uses_even_symmetry_points(kind, P.phase);
                    thetas = use_even_points ? &theta_full : &theta_odd_square;
                }
            }

            for (double theta : *thetas)
            {
                const auto bp = boundary_point(theta, P.a_over_b, P.geometry_mode);
                append_row(Q, row++, kind, L, P, bp, kz_over_k0, h_scaled, p_scaled);
            }
        }
    }

    // ── Rescalonamento para melhor condicionamento numérico [§2.7] ────────
    // Aplicar antes de calcular det(Q) ou valores singulares.
    if (P.rescale_matrix)
    {
        row_rescale(Q); // normaliza cada linha pelo seu max absoluto
        col_rescale(Q); // normaliza cada coluna pelo seu max absoluto
    }

    return Q;
}
