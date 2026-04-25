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

// ═══════════════════════════════════════════════════════════════════════════
// MÉTODOS DE BUSCA DE RAIZ — refinamento após detecção de mudança de sinal
// ═══════════════════════════════════════════════════════════════════════════
//
// Todos os métodos abaixo recebem um intervalo [a, b] onde det(Q) muda de
// sinal e retornam uma estimativa refinada da raiz.
//
// Referência didática: Prof. Lucas Kriesel Sperotto, Cálculo Numérico (2014),
//   Algoritmos 2.1 (Bissecção), 2.4 (Secante), 2.5 (Falsa Posição).
//   Newton-DF adapta o Algoritmo 2.3 com derivada por diferenças finitas.
//   Brent (1973) combina os três anteriores com convergência garantida.
//
// Divergência do artigo (§2.7, p. 2144):
//   Goell afirma usar o "método de Newton" para refinar raízes.  Newton exige
//   f'(P'), a derivada de det(Q) em relação a P' — não disponível de forma
//   analítica.  A alternativa sem derivadas mais próxima do espírito do artigo
//   é o método de Brent, adotado aqui como padrão.
//   Veja docs/referencias/06_metodos_busca_raiz.md para a discussão completa.

namespace
{

// Avalia sign(det(Q)) * |det(Q)|^(1/scale) no ponto (B, P'), com escala
// logarítmica para evitar overflow.  O resultado tem o mesmo sinal que det(Q)
// e magnitude suficiente para interpolações lineares estáveis.
double signed_det(const Params &P, double B, double Pprime)
{
    const auto info = determinant_value(P, B, Pprime);
    // Limita logabs a [-300, 30] para evitar underflow/overflow em exp().
    const double l = std::min(30.0, std::max(-300.0, info.logabs));
    return static_cast<double>(info.sign) * std::exp(l);
}

// ── ALGORITMO 2.1 — Bissecção ──────────────────────────────────────────────
//
// Divide o intervalo [a, b] ao meio e mantém o sub-intervalo onde o sinal
// muda.  Convergência garantida em O(log₂((b-a)/ε)) iterações.
// Taxa de convergência: linear (redução de 50% por iteração).
static double refine_bisect(const Params &P, double B,
                             double a, double b, int max_iter = 50)
{
    int sa = determinant_value(P, B, a).sign;
    for (int it = 0; it < max_iter; ++it)
    {
        const double mid = 0.5 * (a + b);
        const auto   info = determinant_value(P, B, mid);
        if (info.sign == 0) return mid;           // zero exato
        if (sa != 0 && sa != info.sign) b = mid;  // zero está em [a, mid]
        else { a = mid; sa = info.sign; }         // zero está em [mid, b]
    }
    return 0.5 * (a + b);
}

// ── ALGORITMO 2.5 — Falsa Posição (Regula Falsi) ──────────────────────────
//
// Usa interpolação linear entre f(a) e f(b) para estimar a raiz:
//   p = b − f(b)·(b−a) / (f(b)−f(a))
// Mantém o bracket: atualiza a ou b conforme o sinal de f(p).
// Convergência superlinear em funções monótonas; pode ser lento se f
// é muito curva (converge para um extremo fixo).
static double refine_falsepos(const Params &P, double B,
                               double a, double b, int max_iter = 50)
{
    double fa = signed_det(P, B, a);
    double fb = signed_det(P, B, b);
    double p  = b;
    for (int it = 0; it < max_iter; ++it)
    {
        if (std::fabs(fb - fa) < 1e-18) break;
        p = b - fb * (b - a) / (fb - fa);
        p = std::min(1.0 - 1e-6, std::max(1e-6, p));
        const double fp = signed_det(P, B, p);
        if (std::fabs(fp) < 1e-14) break;        // convergiu
        if (fa * fp < 0.0) { b = p; fb = fp; }
        else               { a = p; fa = fp; }
    }
    return p;
}

// ── ALGORITMO 2.4 — Secante ───────────────────────────────────────────────
//
// Usa dois iterados consecutivos para interpolar sem manter bracket:
//   p = p₁ − f(p₁)·(p₁−p₀) / (f(p₁)−f(p₀))
// Convergência superlinear (ordem ≈ 1.618); pode divergir se chute ruim.
// Inicializado com os extremos do bracket para máxima segurança.
static double refine_secant(const Params &P, double B,
                             double p0, double p1, int max_iter = 50)
{
    double f0 = signed_det(P, B, p0);
    double f1 = signed_det(P, B, p1);
    for (int it = 0; it < max_iter; ++it)
    {
        if (std::fabs(f1 - f0) < 1e-18) break;
        const double p = p1 - f1 * (p1 - p0) / (f1 - f0);
        const double pc = std::min(1.0 - 1e-6, std::max(1e-6, p));
        if (std::fabs(pc - p1) < 1e-12) return pc;
        p0 = p1; f0 = f1;
        p1 = pc; f1 = signed_det(P, B, pc);
    }
    return p1;
}

// ── ALGORITMO 2.3 com Diferenças Finitas — Newton-DF ──────────────────────
//
// Aproxima f'(x) por diferença central: f'(x) ≈ (f(x+h)−f(x−h)) / (2h)
// e aplica a iteração de Newton: x ← x − f(x)/f'(x).
// Convergência quadrática perto da raiz; pode divergir longe dela.
// Requer 3 avaliações de det(Q) por iteração (vs. 1 para bissecção).
// Inicia no ponto médio do bracket para máxima estabilidade inicial.
static double refine_newton(const Params &P, double B,
                             double a, double b, int max_iter = 50)
{
    const double h = 1e-7; // passo para diferença central
    double x = 0.5 * (a + b);
    for (int it = 0; it < max_iter; ++it)
    {
        const double fp      = signed_det(P, B, x);
        const double fp_plus = signed_det(P, B, x + h);
        const double fp_minus= signed_det(P, B, x - h);
        const double deriv   = (fp_plus - fp_minus) / (2.0 * h);
        if (std::fabs(deriv) < 1e-18) break;
        const double x_new = std::min(1.0 - 1e-6, std::max(1e-6, x - fp / deriv));
        if (std::fabs(x_new - x) < 1e-12) return x_new;
        x = x_new;
    }
    return x;
}

// ── Método de Brent (1973) ─────────────────────────────────────────────────
//
// Combina bissecção, secante e interpolação quadrática inversa (IQI):
//   - Usa IQI ou secante quando a interpolação cai dentro do intervalo seguro.
//   - Recorre à bissecção se a interpolação é arriscada (critérios de Brent).
//
// Garante: (a) bracket mantido em todo momento → convergência garantida;
//          (b) convergência superlinear quando a função é comportada.
//
// É o método que melhor captura o espírito do artigo ("método de Newton"
// sem exigir a derivada analítica) e é o padrão atual do solver.
static double refine_brent(const Params &P, double B,
                            double a, double b, int max_iter = 50)
{
    double fa = signed_det(P, B, a);
    double fb = signed_det(P, B, b);
    if (fa * fb > 0.0) return 0.5 * (a + b); // sem mudança de sinal: usa centro

    // Garante |f(a)| ≥ |f(b)| (b é sempre o melhor candidato à raiz).
    if (std::fabs(fa) < std::fabs(fb)) { std::swap(a, b); std::swap(fa, fb); }

    double c = a, fc = fa;  // c: iterado anterior; d: iterado antes de c
    double d = a;
    bool mflag = true;      // indica se a última iteração usou bissecção

    for (int it = 0; it < max_iter; ++it)
    {
        if (std::fabs(fb) < 1e-14 || std::fabs(b - a) < 1e-14) break;

        double s;
        if (fa != fc && fb != fc)
        {
            // Interpolação quadrática inversa (IQI) — usa três pontos a, b, c.
            s = (a*fb*fc) / ((fa-fb)*(fa-fc))
              + (b*fa*fc) / ((fb-fa)*(fb-fc))
              + (c*fa*fb) / ((fc-fa)*(fc-fb));
        }
        else
        {
            // Secante — usa dois pontos a, b.
            s = b - fb * (b - a) / (fb - fa);
        }

        // Critérios de Brent: se algum falhar, recorre à bissecção.
        const double lo = std::min((3.0*a + b) / 4.0, b);
        const double hi = std::max((3.0*a + b) / 4.0, b);
        const bool cond1 = !(lo < s && s < hi);
        const bool cond2 = mflag  && std::fabs(s - b) >= std::fabs(b - c) / 2.0;
        const bool cond3 = !mflag && std::fabs(s - b) >= std::fabs(c - d) / 2.0;
        const bool cond4 = mflag  && std::fabs(b - c) < 1e-14;
        const bool cond5 = !mflag && std::fabs(c - d) < 1e-14;

        if (cond1 || cond2 || cond3 || cond4 || cond5)
        {
            s = 0.5 * (a + b); // bissecção
            mflag = true;
        }
        else
        {
            mflag = false;
        }

        s = std::min(1.0 - 1e-6, std::max(1e-6, s));
        const double fs = signed_det(P, B, s);

        d = c; c = b; fc = fb;           // desloca histórico
        if (fa * fs < 0.0) { b = s; fb = fs; }
        else               { a = s; fa = fs; }

        // Mantém b como o melhor candidato.
        if (std::fabs(fa) < std::fabs(fb)) { std::swap(a, b); std::swap(fa, fb); }
    }
    return b;
}

// Despachante: chama o refinador configurado em P.det_refine.
static double refine_root(const Params &P, double B, double a, double b)
{
    if      (P.det_refine == "bisect")   return refine_bisect  (P, B, a, b);
    else if (P.det_refine == "falsepos") return refine_falsepos(P, B, a, b);
    else if (P.det_refine == "secant")   return refine_secant  (P, B, a, b);
    else if (P.det_refine == "newton")   return refine_newton  (P, B, a, b);
    else                                  return refine_brent   (P, B, a, b); // padrão
}

} // namespace (anon)

// Localiza raízes de det(Q) = 0 por detecção de mudança de sinal + refinamento.
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
            // Refina a raiz com o método configurado em P.det_refine.
            append_unique(refine_root(P, B, left.Pprime, right.Pprime));
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
