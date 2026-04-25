/*
 * solver.hpp — Varredura em P' e localização das raízes de det(Q) = 0.
 *
 * Para cada frequência normalizada B, o solver busca os valores de P' ∈ (0,1)
 * nos quais a condição modal det(Q(B, P')) = 0 é satisfeita [Eq. 8 / §2.7].
 * Cada raiz corresponde a um modo guiado com a curva de propagação P'(B)
 * chamada de "curva de propagação" ou "curva de dispersão" (§3.3).
 *
 * Estratégias de busca de raízes (Params::det_search):
 *   "sign"   — detecção por mudança de sinal de det(Q) + refinamento (padrão)
 *   "minima" — busca de mínimos locais de ln|det(Q)| com refinamento parabólico
 *
 * Métodos de refinamento pós-detecção de sinal (Params::det_refine):
 *   "bisect"   — bissecção (Algoritmo 2.1): divide o intervalo ao meio
 *   "falsepos" — falsa posição (Algoritmo 2.5): interpolação linear com bracket
 *   "secant"   — secante (Algoritmo 2.4): interpolação sem bracket garantido
 *   "newton"   — Newton com diferenças finitas (Algoritmo 2.3 adaptado)
 *   "brent"    — Brent (1973): IQI + secante + bissecção adaptativa (padrão)
 *
 * Divergência com o artigo: Goell (§2.7.1, p. 2144) cita o "método de Newton".
 * Adotamos Brent como padrão: sem derivada analítica, convergência garantida.
 * Veja docs/referencias/06_metodos_busca_raiz.md para a discussão completa.
 *
 * Referência: docs/02.7_metodo_de_computacao.md  docs/03.3_curvas_de_propagacao.md
 *             docs/referencias/06_metodos_busca_raiz.md.
 */

#ifndef GOELL_SOLVER_HPP
#define GOELL_SOLVER_HPP

#include "goell/common.hpp"
#include "goell/diagnostics.hpp"

#include <vector>

// ─── Amostra de varredura (métrica escalar) ───────────────────────────────────
//
// Um ponto na grade (B, P') com o valor da métrica merit = ln|det(Q)|
// ou log₁₀(σ_min/σ_max), dependendo de Params::metric.
struct Sample
{
    double B      = 0.0; // frequência normalizada B = 2b/λ₀·√(n²_r−1)  [Eq. 6]
    double Pprime = 0.0; // constante de propagação normalizada P' ∈ (0,1)  [Eq. 6]
    double merit  = 0.0; // ln|det(Q)| ou log₁₀(σ_min/σ_max); mínimo absoluto em um modo
};

// ─── Amostra de varredura (determinante com sinal) ────────────────────────────
//
// Estende Sample com o sinal de det(Q), necessário para a busca por mudança
// de sinal.  det(Q) muda de sinal ao cruzar uma raiz simples (zero simples).
struct DetSample
{
    double B      = 0.0;
    double Pprime = 0.0;
    double merit  = 0.0; // ln|det(Q)|
    int    sign   = 0;   // sinal de det(Q): +1, −1 ou 0 (zero exato / singular)
};

// Avalia a métrica no ponto (B, P'): ln|det(Q)| ou log₁₀(σ_min/σ_max).
double merit_value(const Params &P, double B, double Pprime);

// Avalia DeterminantInfo (ln|det| + sinal) no ponto (B, P').
DeterminantInfo determinant_value(const Params &P, double B, double Pprime);

// Varre P' de 0 a 1 em Params::Pscan pontos para B fixo.
// Retorna uma amostra por ponto com a métrica escalar (ln|det| ou sv).
std::vector<Sample> scan_P(const Params &P, double B);

// Mesma varredura, mas retorna DetSample com o sinal de det(Q).
// Necessário para a estratégia de busca "sign".
std::vector<DetSample> scan_P_det(const Params &P, double B);

// Retorna os mínimos locais interiores de uma varredura (usados para "minima").
// Um mínimo local é qualquer ponto com merit < vizinho à esquerda e à direita.
std::vector<Sample> local_minima(const std::vector<Sample> &samples);

// Retorna os mínimos nas bordas da varredura (P'=0+ e P'=1−).
// Necessário quando um modo está na borda do intervalo e --allow-edge-minima está ativo.
std::vector<Sample> edge_minima(const std::vector<Sample> &samples);

// Localiza as raízes de det(Q) = 0 por bissecção nos intervalos onde o sinal
// muda.  Para cada intervalo [P'_a, P'_b] com sinal oposto, itera até 50 vezes
// para refinar a raiz com precisão ≈ 10⁻⁵ em P'.
std::vector<Sample> sign_change_roots(const Params &P, double B, const std::vector<DetSample> &samples);

// Refina a posição de um mínimo local usando interpolação parabólica em 3 pontos.
// Itera até 20 vezes, reduzindo o intervalo [x1, x3] centrado em x2 a cada passo.
// Retorna o P' refinado mais próximo do mínimo real de merit(P').
double refine_local_minimum(const Params &P, double B, double x0);

#endif // GOELL_SOLVER_HPP
