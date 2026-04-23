# TODO

Pendencias tecnicas priorizadas depois do fechamento inicial da pasta `docs/` e da primeira rodada de codificacao do pipeline de reproducao.

## Concluido Nesta Fase

- Revisado o bloco `H^{TA}` em `src/goell_q_solver.cpp`.
- Separada a protecao numerica da avaliacao fisica dos argumentos `hr` e `pr`.
- Reexecutada a Tabela I com selecao autonoma e thresholds objetivos.
- Criado `run.sh` como interface publica de build, reproducao e validacao.
- Estabilizado o pipeline das Figs. 16-19 com CSVs brutos, estaveis e rastreados.
- Adicionado scaffold das Figs. 20-22 para os dois modos principais.

## Validacao modal

- Fechar a correspondencia fisica entre `principal_phi0` / `principal_phi90` e os rotulos `E^y_11` / `E^x_11`.
- Comparar visualmente os PNGs finais das Figs. 16-22 contra o scan do artigo.
- Manter `even-rect-mode=square-split` apenas como diagnostico do setor `even`.

## Proxima Fase: Campos

- Implementar calculo do vetor nulo da matriz `Q`.
- Recuperar coeficientes modais a partir do vetor nulo.
- Amostrar campos em grid para as Figs. 4-15.
- Exportar CSVs de intensidade e componentes de campo.
- Gerar mapas de intensidade em Python.
- Planejar, depois dos mapas de intensidade, linhas de campo.

## Pendencias Tecnicas De Fundo

- Reavaliar a nota de reescalonamento da p. 2144 do paper.
- Registrar em documentacao permanente qualquer decisao nova sobre rotulagem modal.
