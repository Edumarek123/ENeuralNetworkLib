# Arquitetura

## Objetivo

ENeuralNetworkLib é uma implementação compacta de redes neurais densas (*fully connected feedforward*) em C++. Usa matrizes de `double` e permite selecionar funções de ativação por camada.

O projeto tem dois subsistemas: **EMatrix**, para operações numéricas e armazenamento matricial, e **ENeuralNetwork**, para topologia, parâmetros, ativações, erros e treinamento.

## Fluxo de inferência

Para cada camada densa:

```text
z(l) = W(l) · a(l-1) + b(l)
a(l) = f(l)(z(l))
```

Na primeira camada, a entrada é o vetor fornecido pelo chamador. As ativações ficam em `outputsLayers`; pesos e bias ficam em `weightsLayers` e `biasLayers`.

## Fluxo de treinamento

1. `FeedFoward(inputs)`: calcula as ativações.
2. `ComputeError(expectedOutputs)`: calcula o erro na saída e o propaga para camadas anteriores.
3. `BackPropagation(inputs)`: atualiza pesos e bias com `learningRate`.

A atualização é feita diretamente nos parâmetros. Não há API pública de minibatches, otimizadores avançados ou scheduler no código inspecionado.

## Organização

```text
.
├── src/                         # implementação principal
│   ├── ematrix_lib/
│   └── eneural_network_lib/
├── rna/                         # variante de desenvolvimento
│   ├── src/ematrix_lib/
│   ├── src/eneural_network_lib/
│   ├── main.cpp
│   └── Makefile
├── rna.ino                      # sketch Arduino
├── Makefile                     # build desktop
├── todo                         # pendências
└── docs/                        # documentação
```

O repositório contém também objetos compilados e executáveis versionados. Esses artefatos podem estar desatualizados em relação aos fontes.

## Diferenças entre as árvores

| Assunto | `src/` | `rna/src/` |
|---|---|---|
| Seleção de ativação | Sim | Sim |
| Seleção de métrica | Não | Sim, via `ChangeMetricFunction` |
| Tolerância de acurácia | Não configurável pela API | Macro `ACCURACY_TOLERANCE` |
| Treinamento desktop | Caminhos + percentual + épocas + coeficiente de erro | Caminhos + percentual + épocas |
| Treinamento Arduino | Rotina presente, validação/teste incompletos | Rotina presente; requer validação |
| Exportação Arduino | Corpo vazio | Contrato do ponteiro precisa ser revisto |

A pasta `rna/` contém implementação distinta, não apenas exemplos. Trate-a como uma linha separada de desenvolvimento.