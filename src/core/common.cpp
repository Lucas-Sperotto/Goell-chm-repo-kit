/*
 * common.cpp — Conversores de enum para string (usados nas colunas do CSV).
 *
 * Estas funções serializam os enums de configuração como strings legíveis
 * nas colunas "parity", "phase" e "geometry" do CSV de saída.  Isso permite
 * que scripts Python identifiquem a família de modos de cada linha sem
 * precisar saber os valores inteiros dos enums.
 */

#include "goell/common.hpp"

// "odd"  → paridade ímpar (harmônicos n = 1,3,5,…)  [§2.1]
// "even" → paridade par   (harmônicos n = 0,2,4,…)  [§2.1]
std::string parity_name(HarmonicParity parity)
{
    return (parity == HarmonicParity::odd) ? "odd" : "even";
}

// "phi0"  → família φₙ = 0:   Ez ∝ sin(nθ)  [§2.1]
// "phi90" → família φₙ = π/2: Ez ∝ cos(nθ)  [§2.1]
std::string phase_name(PhaseFamily phase)
{
    return (phase == PhaseFamily::phi0) ? "phi0" : "phi90";
}

// "intersection" → raio θ intersecta geometricamente a borda retangular (padrão)
// "literal"      → modo experimental de cálculo de r(θ)
std::string geometry_name(BoundaryGeometryMode mode)
{
    return (mode == BoundaryGeometryMode::literal) ? "literal" : "intersection";
}
