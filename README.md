# ENeuralNetworkLib

<p align="center">
  <strong>Uma biblioteca experimental de redes neurais em C++ com foco em aplicações embarcadas.</strong>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/C%2B%2B-11%2B-00599C?style=for-the-badge&logo=cplusplus&logoColor=white" alt="C++">
  <img src="https://img.shields.io/badge/Arduino-compatible-00979D?style=for-the-badge&logo=arduino&logoColor=white" alt="Arduino">
  <img src="https://img.shields.io/badge/Platform-Linux%20%7C%20Arduino-24292e?style=for-the-badge&logo=linux&logoColor=white" alt="Platforms">
  <img src="https://img.shields.io/badge/Status-Experimental-orange?style=for-the-badge" alt="Experimental">
</p>

---

## Visão geral

**ENeuralNetworkLib** é um projeto em C++ para implementação e experimentação com redes neurais *feedforward*. A biblioteca reúne duas partes principais:

- **ENeuralNetwork:** estrutura da rede, camadas, pesos, bias, propagação direta e retropropagação.
- **EMatrix:** operações matriciais utilizadas nos cálculos da rede, incluindo multiplicação de matrizes, transposição, operações elemento a elemento e funções de ativação.

O código possui caminhos condicionais para execução em Linux e em placas Arduino, com impressão adaptada a `std::cout` ou `Serial`.

> **Estado do projeto:** experimental e em desenvolvimento. O repositório contém código-fonte, uma implementação principal na raiz e uma variante dentro de `rna/`. Algumas rotinas — especialmente treinamento embarcado e exportação — ainda precisam de validação e/ou implementação completa. Consulte as observações deste README antes de integrar a biblioteca a um projeto de produção.

## Arquitetura

A estrutura principal da rede é definida por:

```text
Input layer → Hidden layer(s) → Output layer
```

O construtor recebe a quantidade de entradas, a quantidade de saídas, a estrutura das camadas ocultas e a taxa de aprendizado. Pesos e bias são mantidos por camada.

### Componentes

| Componente | Responsabilidade |
|---|---|
| `ENeuralNetwork` | Estrutura e operações da rede neural |
| `EMatrix` | Armazenamento e operações com matrizes |
| `ActivationFunction` | Identificadores das funções de ativação |
| `FeedFoward()` | Calcula as ativações até a camada de saída |
| `ComputeError()` | Calcula os erros usados na retropropagação |
| `BackPropagation()` | Atualiza pesos e bias usando a taxa de aprendizado |
| `Train()` | Rotina de treinamento, com assinatura diferente entre Linux e Arduino |

## Funcionalidades encontradas

### Rede neural

- Configuração de número de entradas e saídas.
- Configuração de uma ou mais camadas ocultas.
- Inicialização de pesos e bias por meio de `EMatrix`.
- Propagação direta (*feedforward*).
- Cálculo de erro e retropropagação (*backpropagation*).
- Seleção de função de ativação por camada.
- Impressão da estrutura e dos pesos/bias.
- Rotinas de treinamento específicas para desktop e ambiente Arduino.

### Operações matriciais

A classe `EMatrix` oferece operações de soma, multiplicação de Hadamard, multiplicação escalar e aplicação de função a cada elemento.

Também existem funções livres para subtração, multiplicação escalar, produto matricial, transposição, mapeamento de função, comparação, *softmax*, binarização e cálculo de perda.

### Funções de ativação

Na implementação principal da raiz, os identificadores disponíveis são:

| Identificador | Função |
|---|---|
| `ActivationFunction::_eSigmoid` | Sigmoide |
| `ActivationFunction::_eRelu` | ReLU |
| `ActivationFunction::_eTahn` | Tangente hiperbólica |
| `ActivationFunction::_eSin` | Seno |

A rede inicializa as camadas com sigmoide. A função pode ser alterada com `ChangeActivationFunction(function, layer)`.

## Requisitos

Para compilar a versão desktop na raiz:

- Linux ou ambiente compatível com GNU Make.
- Compilador C++ (`g++` no Makefile atual).
- GNU Make.
- Biblioteca ENet disponível no ambiente de linkedição, pois o Makefile adiciona `-lenet`.

Para Arduino:

- Arduino IDE ou toolchain equivalente.
- Core da placa selecionada.
- Os arquivos-fonte e cabeçalhos da biblioteca adicionados ao projeto.

O código utiliza recursos de C++ e bibliotecas padrão como `<math.h>`, `<iostream>`, `<fstream>` e `<sstream>` nos caminhos desktop. A compatibilidade exata depende do core e do compilador da placa.

## Estrutura do repositório

```text
ENeuralNetworkLib/
├── Makefile
├── rna.ino
├── src/
│   ├── ematrix_lib/
│   │   ├── ematrix_lib.hpp
│   │   └── ematrix_lib.cpp
│   └── eneural_network_lib/
│       ├── eneural_network_lib.hpp
│       └── eneural_network_lib.cpp
└── rna/
    ├── Makefile
    ├── main.cpp
    └── src/
        ├── ematrix_lib/
        └── eneural_network_lib/
```

A pasta `src/` da raiz é a implementação principal descrita nas assinaturas deste README. A pasta `rna/` mantém uma variante separada, com diferenças na API de métricas, tolerância e exportação. Não misture arquivos das duas árvores no mesmo build: os cabeçalhos e as assinaturas não são idênticos.

> O repositório também contém arquivos de build (`.o` e executáveis) versionados. Em novos builds, prefira limpar e recompilar a partir dos fontes.

## Compilação no Linux

Na raiz do repositório:

```bash
make build
```

O alvo `build` compila os arquivos `.cpp` encontrados pelo Makefile e gera o executável `main`. Para executar:

```bash
./main
```

O alvo padrão `all` executa limpeza, compilação e execução em sequência:

```bash
make
```

Outros alvos disponíveis:

```bash
make clean  # remove objetos e executável
make debug  # abre o executável no GDB
make leak   # executa a análise com Valgrind
```

O alvo `leak` requer Valgrind instalado. O link do Makefile inclui `-lenet`; caso ENet não esteja instalada, será necessário instalar a dependência ou ajustar `NETFLAGS` se ela não for necessária para seu ambiente.

## Primeiros passos

O exemplo abaixo demonstra a construção de uma rede com duas entradas, uma camada oculta de dois neurônios e uma saída:

```cpp
#include "src/eneural_network_lib/eneural_network_lib.hpp"

int main()
{
    int hiddenLayers[] = {2};

    ENeuralNetwork network(
        2,            // número de entradas
        1,            // número de saídas
        hiddenLayers,  // neurônios por camada oculta
        1,            // quantidade de camadas ocultas
        0.1           // taxa de aprendizado
    );

    network.print();

    return 0;
}
```

O objeto recebe a topologia no construtor:

```cpp
ENeuralNetwork(
    int inputNodesNumber,
    int outputNodesNumber,
    int *hiddenNodesStructure,
    int hiddenLayersNumber,
    _ponto alfaLearn
);
```

Para uma rede sem camadas ocultas, use `hiddenLayersNumber = 0`. O ponteiro de camadas ocultas não será percorrido nesse caso.

## Usando EMatrix

`EMatrix` representa uma matriz dinâmica de valores do tipo `_ponto` (definido como `double`):

```cpp
#include "src/ematrix_lib/ematrix_lib.hpp"

int main()
{
    EMatrix A(2, 2);
    EMatrix B(2, 2);

    A.initilize_values(0.0, false);
    B.initilize_values(1.0, false);

    A.sum(&B);
    A.print();

    return 0;
}
```

Operações matriciais livres disponíveis na API principal:

```cpp
EMatrix C = sub(&A, &B);
EMatrix D = scalar_multiplication(&A, 0.5);
EMatrix E = dot_multiplication(&A, &B);
EMatrix T = transpose(&A);
EMatrix P = map(&A, eSigmoide);
EMatrix S = softmax(&A);
EMatrix Binarized = binary(&A);
```

A multiplicação matricial exige dimensões compatíveis: o número de colunas da primeira matriz deve corresponder ao número de linhas da segunda.

## Treinamento e dados

Na versão principal, a assinatura de treinamento varia por plataforma.

**Linux:**

```cpp
float Train(
    const std::string &pathInputs,
    const std::string &pathExpectedOutputs,
    float testsPercentage,
    int epochs,
    _ponto coeficienteErro
);
```

Os arquivos são lidos linha a linha. A rotina auxiliar `get_next_matrix_data_from_stream()` separa valores usando espaço (` `) e converte cada valor para `_ponto`. Portanto, os arquivos de entrada e saída devem ter linhas correspondentes e valores separados por espaços.

Exemplo de formato:

```text
0.0 1.0
1.0 0.0
0.5 0.5
```

Use um arquivo para entradas e outro para saídas esperadas, mantendo a mesma quantidade e ordem de linhas.

**Arduino:**

```cpp
float Train(
    const EMatrix *inputs,
    const EMatrix *outputs,
    int dataLength,
    float testsPercentage,
    int epochs
);
```

A rotina embarcada está presente no código, mas partes da validação/métrica aparecem comentadas. Considere-a incompleta até implementar e testar o fluxo de avaliação para a placa escolhida.

## Escolhendo a função de ativação

A API principal permite configurar a ativação de uma camada pelo índice:

```cpp
network.ChangeActivationFunction(
    ActivationFunction::_eRelu,
    0
);
```

Os índices começam em zero e identificam as camadas de transformação (não a camada de entrada). A rede é inicializada com sigmoide em todas as camadas.

## Limitações conhecidas

Este repositório está em desenvolvimento. A leitura do código atual mostra pontos que merecem atenção antes de uso em aplicações críticas:

- **Treinamento Arduino:** a rotina possui trechos de cálculo de validação e teste comentados.
- **Exportação:** `eexport()` na versão principal do Linux escreve um cabeçalho com um caminho de include aparentemente incorreto; valide o arquivo gerado antes de utilizá-lo. A implementação Arduino dessa rotina está vazia.
- **Dimensões:** várias operações matriciais pressupõem dimensões compatíveis e não fazem validação completa dos argumentos.
- **Memória:** a classe `EMatrix` gerencia memória manualmente. Faça testes de cópia, vida útil e uso intensivo antes de aplicações longas.
- **Treinamento:** revise a divisão entre treino, validação e teste, e valide os resultados com conjuntos de dados conhecidos.
- **Arquivos:** a rotina de leitura espera valores numéricos separados por espaço e não implementa um parser CSV genérico.

As observações acima descrevem o estado do código consultado; não representam uma garantia de comportamento em todas as plataformas.

## Roadmap

O arquivo `todo` do projeto registra itens ainda pendentes, incluindo:

- Finalizar a exportação para Arduino.
- Ajustar o cálculo dos tamanhos dos conjuntos de dados nas rotinas de treinamento.
- Completar o treinamento para Arduino.
- Adicionar opções de métricas e funções de perda.
- Expandir a documentação.


## Documentação técnica

A documentação detalhada foi organizada em [`docs/`](docs/README.md):

- [Arquitetura e organização das implementações](docs/architecture.md)
- [Instalação e compilação no Linux e Arduino](docs/build.md)
- [Referência da classe ENeuralNetwork](docs/neural-network.md)
- [Referência da classe EMatrix e operações numéricas](docs/matrix.md)
- [Formato dos dados, treinamento e métricas](docs/data-and-training.md)
- [Exemplos de utilização](docs/examples.md)
- [Limitações conhecidas e roadmap técnico](docs/known-limitations.md)

As páginas distinguem a implementação principal em `src/` da variante `rna/src/`, cujas APIs não são intercambiáveis.

## Contribuindo

Sugestões, correções e melhorias são bem-vindas. Ao propor uma alteração:

1. Descreva o problema ou a melhoria.
2. Inclua um exemplo mínimo reproduzível quando aplicável.
3. Informe plataforma, compilador e versão do core Arduino (quando pertinente).
4. Teste a alteração na plataforma alvo e documente limitações conhecidas.

## Licença

Nenhum arquivo de licença foi identificado na raiz do repositório no momento da documentação. Consulte o proprietário antes de redistribuir ou incorporar o código em outro projeto.

---

<p align="center">
  Feito por <a href="https://github.com/Edumarek123">Eduardo Marek</a>
</p>
