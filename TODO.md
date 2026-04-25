# Roadmap Técnico para Fechamento do Projeto

Este documento organiza as pendências em um plano de execução sequencial, dividido em fases, para levar o projeto a um estado "fechado": cientificamente validado, numericamente robusto, totalmente reproduzível e didaticamente claro.

---

## Fase 1: Validação Crítica do Modelo Físico

*O objetivo desta fase é validar a implementação dos campos (autovetores), que é o bloqueio mais crítico para a correção científica do projeto.*

### [ ] 1.1. Validar mapas de campo contra as Figs. 4–15 do artigo
-   **Descrição:** O código gera mapas de campo 2D, mas eles não foram sistematicamente comparados com os resultados publicados por Goell. A validação atual cobre apenas os autovalores (curvas de propagação), mas não os autovetores (padrões de campo).
-   **Impacto:** Crítico. Sem esta validação, não há garantia de que a implementação dos campos ($E_z, H_z, E_t, H_t$) está correta em fase, simetria e amplitude relativa.
-   **Como Validar:**
    1.  Criar um arquivo (`docs/referencias/06_presets_field_map.md`) para registrar os parâmetros de cada figura do artigo (4 a 15).
    2.  Usar `scripts/field_map.py` para gerar os mapas correspondentes.
    3.  Realizar uma comparação visual (painel a painel) e quantitativa (se possível) com os scans do artigo, documentando acertos e divergências.

---

## Fase 2: Consistência Científica e Documentação

*Com o modelo físico validado, o foco agora é garantir que a conexão entre a teoria, o código e a documentação seja explícita e correta.*

### [ ] 2.1. Formalizar a correspondência entre classe de paridade/fase e rótulo modal
-   **Ligação com Artigo:** Seções 2.3 e 2.4 (Designação dos modos $E^y_{mn}$, $E^x_{mn}$).
-   **O que Falta:** O código usa a nomenclatura interna (`odd/phi0`, `even/phi90`, etc.), enquanto o artigo usa rótulos físicos. A conexão entre os dois sistemas não está formalmente documentada, o que é uma lacuna didática crucial.
-   **Ação Necessária:**
    1.  Criar `docs/referencias/05_correspondencia_modal.md`.
    2.  Construir uma tabela de mapeamento, validando qual componente de campo ($E_z$ ou $H_z$) é dominante para cada classe usando a saída do solver (ex: `--null-vector`).
    3.  Justificar cada linha da tabela com base nas equações de simetria do artigo.

### [ ] 2.2. Documentar e validar o método de refinamento de raiz
-   **Problema:** O artigo menciona o "método de Newton", mas a implementação pode usar um método diferente (ex: interpolação parabólica, Brent) por razões de robustez. A escolha e suas implicações precisam ser documentadas.
-   **Ação Necessária:**
    1.  Criar uma nota de referência (`docs/referencias/07_metodos_numericos.md`) sobre a busca de raízes.
    2.  Documentar o método de refinamento final implementado em `goell_solver.cpp`.
    3.  Justificar a escolha em relação ao método de Newton (ex: robustez, não necessidade de derivadas).
    4.  Demonstrar com um exemplo que o método escolhido converge para a precisão necessária.

### [ ] 2.3. Documentar a estratégia de reescalonamento numérico
-   **Arquivos a Criar/Corrigir:** Adicionar à nota `docs/referencias/07_metodos_numericos.md`.
-   **Lacuna Conceitual:** O artigo (p. 2144) menciona uma função de escalonamento. O código implementa uma normalização genérica (`--rescale`). A decisão de usar `--no-rescale` como padrão e as implicações dessa escolha não estão documentadas. É preciso explicar o que o artigo sugere, o que o código faz e por que a escolha atual foi feita.

---

## Fase 3: Reprodutibilidade e Padronização

*O objetivo desta fase é garantir que qualquer pessoa possa, de forma fácil e confiável, compilar o código, executar as simulações e gerar todos os artefatos (tabelas e figuras) do artigo.*

### [ ] 3.1. Refatorar e padronizar a estrutura de scripts Python
-   **Arquivos Afetados:** Todos os scripts `.py` em `src/` e o `CONTRIBUTING.md`.
-   **Ação Necessária:** O arquivo `CONTRIBUTING.md` descreve uma estrutura de scripts Python modular (`scripts/lib`, `scripts/pipelines`, etc.) que não corresponde à estrutura atual (scripts monolíticos em `src/`). É preciso:
    1.  Mover todos os scripts Python de `src/` para `scripts/`.
    2.  Refatorar a lógica comum (ex: parsing de CSV, plotagem base) para módulos dentro de `scripts/lib/`.
    3.  Manter os scripts na raiz de `scripts/` como os pontos de entrada do pipeline (ex: `scripts/reproduce_table1.py`).
    4.  Atualizar o `Makefile` e o `run.sh` para refletir os novos caminhos.

### [ ] 3.2. Integrar a geração de mapas de campo ao pipeline principal
-   **Ajustes:** O `run.sh` não possui um comando para gerar mapas de campo de forma padronizada.
-   **Ação Necessária:**
    1.  Criar um sistema de "presets" (ex: em `scripts/presets.sh`) que defina os parâmetros para os modos canônicos a serem plotados.
    2.  Adicionar um novo alvo ao `run.sh`: `./run.sh field-map <nome_do_modo>`.
    3.  Este comando deve invocar `scripts/field_map.py` com os parâmetros do preset escolhido.
    4.  Atualizar a documentação (`README.md`, `CONTRIBUTING.md`) para incluir o novo comando.

### [ ] 3.3. Sincronizar `CONTRIBUTING.md` com a estrutura real do projeto
-   **Arquivos a Corrigir:** `CONTRIBUTING.md`.
-   **Lacuna Conceitual:** O arquivo descreve uma estrutura de pastas e scripts que está à frente da implementação atual (especialmente na parte de Python). Ele deve ser atualizado para refletir o estado do projeto após a padronização de código (item 4).

---

## Fase 4: Refinamento e Finalização (Melhorias não bloqueantes)

*Itens que aumentam a qualidade e a manutenibilidade do projeto a longo prazo, mas não bloqueiam a validação científica.*

### [ ] 4.1. Adotar um framework de testes unitários para C++
-   **Organização:** O `Makefile` já possui um alvo `check-cpp` que executa um "smoke test". Para maior robustez, o ideal seria adotar um framework leve como o Catch2 ou GoogleTest.
-   **Clareza Didática:** Permitiria criar testes isolados para cada módulo (`goell_bessel`, `goell_layout`, etc.), validando cada peça do sistema de forma independente e documentando o comportamento esperado.

---

## Critério de Conclusão

O projeto será considerado **FINALIZADO** quando todos os itens a seguir forem concluídos:

-   [ ] **Fase 1 Concluída:** Os mapas de campo gerados pelo código reproduzem fielmente os resultados das Figuras 4-15 do artigo.
-   [ ] **Fase 2 Concluída:** A correspondência modal está documentada e as escolhas de métodos numéricos (refinamento de raiz, reescalonamento) estão justificadas por escrito.
-   [ ] **Fase 3 Concluída:** O pipeline de reprodutibilidade está completo e a documentação para contribuidores (`CONTRIBUTING.md`) reflete a estrutura final do projeto.
-   [ ] **Documentação Completa:** Não há lacunas conceituais entre o artigo, a documentação `.md` e o código. Todos os guias (`README.md`, `CONTRIBUTING.md`) estão sincronizados com o estado final do projeto (item 6).
