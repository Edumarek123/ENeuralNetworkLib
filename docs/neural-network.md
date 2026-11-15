# Referência: ENeuralNetwork

Arquivo: `src/eneural_network_lib/eneural_network_lib.hpp`.

## Construtor

```cpp
ENeuralNetwork(
    int inputNodesNumber,
    int outputNodesNumber,
    int *hiddenNodesStructure,
    int hiddenLayersNumber,
    _ponto alfaLearn
);
```

| Parâmetro | Descrição |
|---|---|
| `inputNodesNumber` | Quantidade de entradas |
| `outputNodesNumber` | Quantidade de saídas |
| `hiddenNodesStructure` | Array com neurônios por camada oculta |
| `hiddenLayersNumber` | Quantidade de camadas ocultas |
| `alfaLearn` | Taxa de aprendizado |

Exemplo: `{8, 4}` com duas camadas define `input → 8 → 4 → output`. O construtor sem argumentos é deletado.

## Campos públicos

| Campo | Significado |
|---|---|
| `learningRate` | Taxa de aprendizado |
| `layersNumber` | Total de camadas, incluindo entrada e saída |
| `nodesLayerX` / `nodesLayerY` | Quantidade de entradas / saídas |
| `layersStructure` | Tamanho de cada camada |
| `funLayers` / `DfunLayers` | Funções de ativação e derivadas |
| `outputLayerIndex` | Índice da última camada de transformação |
| `weightsLayers` / `biasLayers` | Parâmetros da rede |
| `errorsLayers` / `outputsLayers` | Erros e ativações intermediárias |
| `allocatedBytes` | Estimativa retornada por `allocated_memory()` |

Os campos são públicos; alterá-los diretamente pode quebrar invariantes internos.

## Métodos

- `print()`: imprime topologia, pesos e bias.
- `FeedFoward(EMatrix inputs)`: executa inferência. A entrada esperada é um vetor coluna `inputNodesNumber × 1`.
- `ComputeError(EMatrix expectedOutputs)`: calcula erro na saída e propaga erros para trás.
- `BackPropagation(EMatrix inputs)`: atualiza pesos e bias usando a taxa de aprendizado.
- `ChangeActivationFunction(int function, int layer)`: altera ativação da camada de transformação indicada.
- `RealizarTeste(const EMatrix*, const EMatrix*)`: executa inferência e compara com o esperado usando limiar interno fixo.
- `eload(_ponto *data)`: carrega pesos e depois bias de um buffer; não recebe o tamanho, portanto o chamador deve garantir o comprimento.
- `eexport(const char *path)`: tenta gerar cabeçalho com pesos/bias no Linux; a versão Arduino da raiz está vazia.
- `allocated_memory()`: estimativa parcial; não contabiliza todas as matrizes.

## Funções de ativação

A enumeração principal define `_eSigmoid`, `_eRelu`, `_eTahn` (grafia original no código) e `_eSin`. A rede inicia com sigmoide em todas as camadas.

```cpp
network.ChangeActivationFunction(ActivationFunction::_eRelu, 0);
```

Os índices começam em zero e identificam camadas de transformação, não a camada de entrada.

## Treinamento

Linux:

```cpp
float Train(const std::string &pathInputs,
            const std::string &pathExpectedOutputs,
            float testsPercentage, int epochs,
            _ponto coeficienteErro);
```

Arduino:

```cpp
float Train(const EMatrix *inputs, const EMatrix *outputs,
            int dataLength, float testsPercentage, int epochs);
```

Consulte [Dados e treinamento](data-and-training.md) para o formato de arquivo e as ressalvas.