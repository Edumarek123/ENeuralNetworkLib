# Documentação — ENeuralNetworkLib

Esta pasta reúne a documentação técnica do projeto. Leia as páginas na ordem sugerida:

| Documento | Conteúdo |
|---|---|
| [Arquitetura](architecture.md) | Componentes, fluxo matemático e organização das versões |
| [Instalação e compilação](build.md) | Requisitos, Makefile, Linux e Arduino |
| [ENeuralNetwork](neural-network.md) | Topologia, API, propagação e treinamento |
| [EMatrix](matrix.md) | Estruturas matriciais, operações e funções numéricas |
| [Dados e treinamento](data-and-training.md) | Formato dos arquivos, divisão dos conjuntos e métricas |
| [Exemplos](examples.md) | Exemplos de construção e uso |
| [Limitações e roadmap](known-limitations.md) | Itens que precisam de correção/validação |

## Qual implementação devo usar?

- **`src/` (raiz):** implementação original, com API mais simples e funções de perda/treinamento ainda experimentais.
- **`rna/src/`:** variante posterior, que acrescenta abstração de métricas, tolerância e diferenças no fluxo de treinamento/exportação.

As duas árvores têm assinaturas e definições diferentes. Compile apenas uma delas por projeto e não misture seus arquivos `.cpp` e `.hpp`.

A documentação descreve o código presente no repositório. Onde o comportamento está incompleto ou apresenta riscos identificados na inspeção, isso é indicado explicitamente.
