# AGENTS.md

Este arquivo existe para orientar agentes automatizados e fluxos assistidos por IA neste repositorio.

Ele complementa, e nao substitui, o `README.md` e o `CONTRIBUTING.md`. Para contribuidores humanos, o guia principal de colaboracao deve ser o `CONTRIBUTING.md`; para agentes, este arquivo preserva restricoes e prioridades que nao devem ser inferidas automaticamente.

## Objetivo

Reproduzir, com clareza e fidelidade, o artigo de J. E. Goell (1969) sobre analise por harmonicos circulares de guias de onda dieletricos retangulares.

## Princípios de desenvolvimento

1. Priorizar clareza, fidelidade ao artigo e didática.
2. Não inventar fórmulas ausentes; quando faltar informação, registrar a lacuna e pedir verificação.
3. Marcar ambiguidades de OCR com `TODO`.
4. Preservar a rastreabilidade entre texto, equacoes, figuras, scripts e resultados numericos.
5. Manter o nucleo numerico em `C++17`, sem dependencias desnecessarias.
6. Tratar `src/goell_q_solver.cpp` como o solver central e os arquivos Python em `src/` como utilitarios de reproducao, diagnostico e plotagem.
7. Garantir que a saida numerica gerada pelo fluxo principal continue indo para `CSV` em `out/`, com figuras derivadas em `figures/`.
8. Nao introduzir estruturas de projeto ficticias ou herdadas de outros repositorios, como `src/apps/`, `scripts/` ou `data/`, sem que elas existam de fato e sejam adotadas explicitamente.
9. Comentar o codigo de forma clara e suficiente para facilitar revisao cientifica.
10. Registrar, sempre que possivel, quais equacoes, figuras ou tabelas do artigo cada modulo implementa.

## Convenções para documentação

1. Usar `$...$` para matemática inline em arquivos `.md`.
2. Usar `$$...$$` para blocos matemáticos em arquivos `.md`.
3. Preservar rastreabilidade entre texto, equações, figuras e resultados numéricos.
4. Quando houver dúvida de tradução, OCR ou notação, explicitar a incerteza em vez de ocultá-la.
5. Se um arquivo de `docs/` for renomeado ou promovido de categoria, atualizar tambem `docs/README.md` e os links do `README.md` da raiz.

## Convenções para evolução do repositório

1. Manter a estrutura principal em `docs/`, `docs/referencias/` e `src/`, respeitando a organizacao real do repositorio.
2. Tratar `build/`, `out/` e `figures/` como artefatos gerados, nao como documentacao canonica.
3. Antes de propor reorganizacoes maiores, conferir o estado atual do repositorio em vez de assumir a arquitetura de outros projetos.
4. Implementar primeiro uma base minima, verificavel e organizada; depois expandir a matematica ou o ferramental auxiliar.
