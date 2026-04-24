/*
 * solver.cpp — Varredura em P' e busca de raízes de det(Q) = 0.
 *
 * Contexto físico (artigo §2.7, §3.3):
 *   Para um guia com n_r e a/b fixos, cada modo guiado traça uma curva
 *   P'(B) no plano (B, P') chamada curva de propagação (§3.3, Figs. 5-7).
 *   P' ∈ (0,1) é a constante de propagação normalizada [Eq. 6]:
 *     P' = 0 → corte (k_z = k₀, campo não decai no exterior)
 *     P' = 1 → confinamento total (k_z = k₁, campo zero no exterior)
 *
 *   Para cada valor de B, o solver resolve numericamente det(Q(B,P')) = 0
 *   usando um dos dois métodos configurados por Params::det_search:
 *
 *   Método "sign" (padrão):
 *     1. Varrer P' uniformemente → vetor de DetSample com sinal de det(Q)
 *     2. Detectar intervalos onde o sinal muda (zero por continuidade)
 *     3. Refinar por bissecção até convergência (~50 iterações → δP' ≈ 10⁻⁵)
 *
 *   Método "minima":
 *     1. Varrer P' → vetor de Sample com ln|det(Q)|
 *     2. Detectar mínimos locais (pontos mais baixos que seus vizinhos)
 *     3. Refinar por interpolação parabólica (20 iterações)
 *
 * Referência: docs/02.7_metodo_de_computacao.md  docs/03.1_precisao.md
 *             docs/03.3_curvas_de_propagacao.md
 */

#include "goell/solver.hpp"

#include "goell/diagnostics.hpp"
#include "goell/matrix.hpp"

#include <algorithm>
#include <cmath>

// Avalia a métrica de proximidade à singularidade para o ponto (B, P').
// "det" → ln|det(Q)|;  "sv" → log₁₀(σ_min/σ_max).
// Valores menores (mais negativos) indicam ponto mais próximo de um modo.
double merit_value(const Params &P, double B, double Pprime)
{
    const auto Q = assemble_Q(P, B, Pprime);
    return (P.metric == "sv") ? log10_sigma_rel(Q) : determinant_info(Q).logabs;
}

// Avalia DeterminantInfo completa (ln|det| + sinal) para o ponto (B, P').
DeterminantInfo determinant_value(const Params &P, double B, double Pprime)
{
    return determinant_info(assemble_Q(P, B, Pprime));
}

// Varre P' de (1/(Pscan+1)) até (Pscan/(Pscan+1)) em Pscan pontos igualmente
// espaçados no interior de (0,1), evitando os extremos onde h=0 ou p=0.
// Retorna amostras com a métrica escalar (ln|det| ou log₁₀(σ_min/σ_max)).
std::vector<Sample> scan_P(const Params &P, double B)
{
    std::vector<Sample> out;
    out.reserve(P.Pscan);

    for (int i = 0; i < P.Pscan; ++i)
    {
        // P' uniformemente distribuído em (0, 1) exclusive os extremos
        const double Pprime = (i + 1.0) / static_cast<double>(P.Pscan + 1);
        out.push_back({B, Pprime, merit_value(P, B, Pprime)});
    }

    return out;
}

// Mesma varredura que scan_P, mas retorna DetSample com o sinal de det(Q).
// Necessário para localizar mudanças de sinal (zeros de det) na busca "sign".
std::vector<DetSample> scan_P_det(const Params &P, double B)
{
    std::vector<DetSample> out;
    out.reserve(P.Pscan);

    for (int i = 0; i < P.Pscan; ++i)
    {
        const double Pprime = (i + 1.0) / static_cast<double>(P.Pscan + 1);
        const auto info = determinant_value(P, B, Pprime);
        out.push_back({B, Pprime, info.logabs, info.sign});
    }

    return out;
}

// Retorna os mínimos locais interiores da varredura (P'[i-1] > P'[i] < P'[i+1]).
// Esses pontos são candidatos a modos para o método "minima".
// Exclui pontos com merit = ±∞ (singularidades numéricas).
std::vector<Sample> local_minima(const std::vector<Sample> &samples)
{
    std::vector<Sample> mins;
    if (samples.size() < 2)
        return mins;

    for (std::size_t i = 1; i + 1 < samples.size(); ++i)
    {
        if (std::isfinite(samples[i].merit) &&
            samples[i].merit < samples[i - 1].merit &&
            samples[i].merit < samples[i + 1].merit)
        {
            mins.push_back(samples[i]);
        }
    }

    return mins;
}

// Retorna mínimos nas bordas da varredura (primeiro e último ponto).
// Ativo somente com --allow-edge-minima; capta modos perto de P'→0 ou P'→1.
std::vector<Sample> edge_minima(const std::vector<Sample> &samples)
{
    std::vector<Sample> mins;
    if (samples.size() < 2)
        return mins;

    if (std::isfinite(samples.front().merit) && samples.front().merit < samples[1].merit)
        mins.push_back(samples.front());

    if (std::isfinite(samples.back().merit) &&
        samples.back().merit < samples[samples.size() - 2].merit)
    {
        mins.push_back(samples.back());
    }

    return mins;
}

// Localiza raízes de det(Q) = 0 por detecção de mudança de sinal + bissecção.
//
// Algoritmo:
//   Para cada par consecutivo (left, right) na varredura DetSample:
//     - Se left.sign == 0 → raiz exata em left.Pprime → registrar diretamente.
//     - Se left.sign != right.sign → raiz no intervalo [left.Pprime, right.Pprime].
//       Bissecção de até 50 iterações: divide ao meio e mantém o sub-intervalo
//       onde o sinal muda.  Converge para δP' ≈ (1/Pscan) / 2⁵⁰ ≈ 10⁻¹⁷.
//
// Raízes muito próximas (δP' < 10⁻⁵) são suprimidas para evitar duplicatas
// (dois zeros muito próximos do mesmo modo por ruído numérico).
std::vector<Sample> sign_change_roots(const Params &P, double B, const std::vector<DetSample> &samples)
{
    std::vector<Sample> roots;
    if (samples.size() < 2)
        return roots;

    // Adiciona raiz clampada a (1e-6, 1-1e-6) e suprime duplicatas próximas.
    auto append_unique = [&](double pref)
    {
        const double clamped = std::min(1.0 - 1e-6, std::max(1e-6, pref));
        if (!roots.empty() && std::fabs(roots.back().Pprime - clamped) < 1e-5)
            return; // suprime duplicata
        const auto info = determinant_value(P, B, clamped);
        roots.push_back({B, clamped, info.logabs});
    };

    for (std::size_t i = 0; i + 1 < samples.size(); ++i)
    {
        const auto &left  = samples[i];
        const auto &right = samples[i + 1];

        if (left.sign == 0)
        {
            append_unique(left.Pprime); // zero exato na grade
            continue;
        }

        if (left.sign != 0 && right.sign != 0 && left.sign != right.sign)
        {
            // Mudança de sinal detectada: det(Q) cruza zero entre left e right.
            // Bissecção para refinar a posição da raiz.
            double a = left.Pprime,  b = right.Pprime;
            int    sa = left.sign,   sb = right.sign;

            for (int it = 0; it < 50; ++it)
            {
                const double mid       = 0.5 * (a + b);
                const auto   info_mid  = determinant_value(P, B, mid);

                if (info_mid.sign == 0) // zero exato no meio
                {
                    a = b = mid;
                    break;
                }

                // Mantém o sub-intervalo onde o sinal ainda muda.
                if (sa != 0 && sa != info_mid.sign)
                {
                    b  = mid;
                    sb = info_mid.sign;
                }
                else if (sb != 0 && sb != info_mid.sign)
                {
                    a  = mid;
                    sa = info_mid.sign;
                }
                else
                {
                    break; // sinal intermediário = 0 ou inconsistente
                }
            }

            append_unique(0.5 * (a + b));
        }
    }

    return roots;
}

// Refina a posição de um mínimo local de merit(P') usando interpolação parabólica.
//
// Algoritmo (20 iterações):
//   Dados três pontos (x1, y1), (x2, y2), (x3, y3) com x1 < x2 < x3,
//   ajusta uma parábola e estima o mínimo em xm = −B/(2A) (vértice da parábola).
//   Contrai o intervalo [x1, x3] em torno de xm a cada iteração.
//   Retorna x2 (melhor estimativa do mínimo).
double refine_local_minimum(const Params &P, double B, double x0)
{
    // Avalia merit com P' clampado a (1e-6, 1-1e-6) para evitar extremos.
    auto f = [&](double x)
    {
        return merit_value(P, B, std::min(1.0 - 1e-6, std::max(1e-6, x)));
    };

    // Inicializa três pontos ao redor do candidato x0.
    const double dx = 2.0 / static_cast<double>(P.Pscan + 1); // passo da grade de varredura
    double x1 = std::max(1e-6, x0 - dx);
    double x2 = x0;
    double x3 = std::min(1.0 - 1e-6, x0 + dx);

    for (int it = 0; it < 20; ++it)
    {
        const double y1 = f(x1), y2 = f(x2), y3 = f(x3);

        // Coeficientes da parábola y = A·x² + Bc·x + C ajustada aos 3 pontos.
        const double D = (x1 - x2) * (x1 - x3) * (x2 - x3);
        if (std::fabs(D) < 1e-18)
            break;

        const double A  = (y1 * (x2 - x3) + y2 * (x3 - x1) + y3 * (x1 - x2)) / D;
        const double Bc = (y1 * (x3*x3 - x2*x2) + y2 * (x1*x1 - x3*x3) + y3 * (x2*x2 - x1*x1)) / D;

        if (std::fabs(A) < 1e-18)
            break; // parábola degenerada (reta)

        // Vértice da parábola: xm = −Bc/(2A)
        const double xm = -Bc / (2.0 * A);
        if (!std::isfinite(xm))
            break;

        // Novo intervalo centrado em xm, 40% menor que o anterior.
        const double xmid = std::min(x3, std::max(x1, xm));
        const double h    = 0.6 * 0.5 * (x3 - x1);
        x1 = std::max(1e-6,       xmid - h);
        x2 = std::min(1.0 - 1e-6, std::max(1e-6, xmid));
        x3 = std::min(1.0 - 1e-6, xmid + h);
    }

    return x2;
}
