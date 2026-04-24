/*
 * output.hpp — Funções de saída CSV do solver Goell CHM.
 *
 * O solver produz três tipos de saída CSV, cada um ativado por uma flag:
 *
 *   write_bessel_csv    — tabela de validação das funções de Bessel (--test-bessel)
 *   write_dump_scan_csv — varredura bruta de merit(P') para B fixo (--dump-scan)
 *   write_root_csv      — saída principal: modos localizados para cada B (padrão)
 *
 * Referência: docs/simbolos.md §"Colunas do CSV de saída"  docs/README.md.
 */

#ifndef GOELL_OUTPUT_HPP
#define GOELL_OUTPUT_HPP

#include "goell/common.hpp"

#include <iosfwd>

// Escreve uma tabela CSV de Jₙ, J'ₙ, Kₙ, K'ₙ para pares (n, x) de referência.
// Colunas: n, x, Jn, Jn_prime, Kn, Kn_prime.
// Usada pelo script validate_bessel.py para verificar 70 pontos contra tabelas.
void write_bessel_csv(std::ostream &out);

// Escreve uma varredura de merit(P') para o valor P.dump_B fixo.
// Colunas: B, Pprime, merit  [+ det_sign se --dump-det-sign]  parity, phase, geometry.
// Útil para inspecionar o perfil de det(Q) e diagnosticar problemas de busca de raízes.
void write_dump_scan_csv(std::ostream &out, const Params &P);

// Saída principal: para cada B no intervalo [B_min, B_max], localiza os modos
// e escreve uma linha por modo encontrado.
// Colunas: branch_id, B, Pprime, merit, parity, phase, geometry
//          [+ Ez_frac, Hz_frac, mode_class  se --null-vector]
// branch_id = índice do candidato por valor de B (começa em 0).
void write_root_csv(std::ostream &out, const Params &P);

#endif // GOELL_OUTPUT_HPP
