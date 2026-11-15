# Exemplos

Os exemplos abaixo usam a implementação da raiz.

## Criar uma rede

```cpp
#include "src/eneural_network_lib/eneural_network_lib.hpp"

int main()
{
    int hidden[] = {4, 3};
    ENeuralNetwork network(2, 1, hidden, 2, 0.1);
    network.print();
    return 0;
}
```

Topologia: `2 → 4 → 3 → 1`.

## Inferência

```cpp
#include "src/eneural_network_lib/eneural_network_lib.hpp"

int main()
{
    int hidden[] = {3};
    ENeuralNetwork network(2, 1, hidden, 1, 0.1);

    EMatrix input(2, 1);
    input.matrix[0][0] = 0.25;
    input.matrix[1][0] = 0.75;

    network.FeedFoward(input);
    network.outputsLayers[network.outputLayerIndex]->print();
    return 0;
}
```

Os pesos ainda são aleatórios; a saída não representa um modelo treinado.

## Ativação

```cpp
network.ChangeActivationFunction(ActivationFunction::_eRelu, 0);
```

## Operações de matriz

```cpp
#include "src/ematrix_lib/ematrix_lib.hpp"

int main()
{
    EMatrix input(2, 1);
    input.initilize_values(0.0, false);
    input.matrix[0][0] = 1.0;
    input.matrix[1][0] = 2.0;

    EMatrix scaled = scalar_multiplication(&input, 2.0);
    EMatrix transposed = transpose(&input);
    scaled.print();
    transposed.print();
}
```

## Treinar no desktop

```cpp
int hidden[] = {4};
ENeuralNetwork network(2, 1, hidden, 1, 0.1);
float result = network.Train("inputs.txt", "expected.txt", 20.0f, 500, 0.05);
```

Use arquivos no formato de [Dados e treinamento](data-and-training.md). Os dados precisam corresponder à topologia.

## Arduino

O sketch atual apenas inicia Serial e instancia a rede. Um ponto de partida:

```cpp
#include "src/eneural_network_lib/eneural_network_lib.hpp"

int hidden[] = {2};
ENeuralNetwork *network = nullptr;

void setup()
{
    Serial.begin(115200);
    network = new ENeuralNetwork(2, 1, hidden, 1, 0.1);
    network->print();
}

void loop()
{
    // Monte um vetor-coluna EMatrix e chame FeedFoward().
}
```

Ajuste a organização dos arquivos para o Arduino IDE/core e planeje a alocação dinâmica.