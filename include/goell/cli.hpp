/*
 * cli.hpp — Análise dos argumentos de linha de comando (CLI).
 *
 * O executável goell_q_solver recebe todos os parâmetros via flags --nome valor.
 * As funções aqui declaradas convertem strings em tipos enumerados e preenchem
 * a struct Params, que é então usada por todos os demais módulos do solver.
 *
 * Exemplo de chamada completa (reproduz a Tabela I do artigo, modo HE₁₁):
 *   ./build/goell_q_solver \
 *     --parity odd --phase phi0 --a_over_b 1 --nr 1.01 \
 *     --N 5 --Bmin 0.5 --Bmax 3.5 --NB 30 \
 *     --Pscan 240 --metric det --det-search sign \
 *     --even-rect-mode paper --all-minima
 *
 * Referência: docs/simbolos.md §"Parâmetros do CLI".
 */

#ifndef GOELL_CLI_HPP
#define GOELL_CLI_HPP

#include "goell/common.hpp"

#include <string>

// Converte string → HarmonicParity: "odd" ou "even".
// Lança std::runtime_error para valores inválidos.
HarmonicParity parse_parity(const std::string &value);

// Converte string → PhaseFamily: "phi0" (ou "0") / "phi90" (ou "pi/2", "pi2").
PhaseFamily parse_phase(const std::string &value);

// Valida o modo de busca de raízes: "minima" ou "sign".
// Retorna a string normalizada ou lança std::runtime_error.
std::string parse_det_search_mode(const std::string &value);

// Converte string → BoundaryGeometryMode: "literal" ou "intersection" / "secant".
BoundaryGeometryMode parse_geometry_mode(const std::string &value);

// Converte string → EvenRectMatchingMode: "paper" ou "square-split" / "square_split" / "square".
EvenRectMatchingMode parse_even_rect_mode(const std::string &value);

// Analisa argv[1..argc-1] e preenche P com os parâmetros encontrados.
// Valores não especificados mantêm os defaults de Params.
// Lança std::runtime_error para flags inválidas ou valores ausentes.
void parse_args(int argc, char **argv, Params &P);

#endif // GOELL_CLI_HPP
