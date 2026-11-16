# ENeuralNetworkLib

<p align="center">
  <strong>An experimental C++ neural-network library focused on embedded applications.</strong>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/C%2B%2B-11%2B-00599C?style=for-the-badge&logo=cplusplus&logoColor=white" alt="C++">
  <img src="https://img.shields.io/badge/Arduino-compatible-00979D?style=for-the-badge&logo=arduino&logoColor=white" alt="Arduino">
  <img src="https://img.shields.io/badge/Platform-Linux%20%7C%20Arduino-24292e?style=for-the-badge&logo=linux&logoColor=white" alt="Platforms">
  <img src="https://img.shields.io/badge/Status-Experimental-orange?style=for-the-badge" alt="Experimental">
</p>

---

## Overview

**ENeuralNetworkLib** is a C++ project for implementing and experimenting with feedforward neural networks. The library has two main components:

- **ENeuralNetwork:** network structure, layers, weights, biases, forward propagation, and backpropagation.
- **EMatrix:** matrix operations used by the network, including matrix multiplication, transposition, element-wise operations, and activation functions.

Conditional compilation paths support Linux and Arduino boards, with output adapted to `std::cout` or `Serial`.

> **Project status:** Experimental and under active development. The repository contains source code, a primary implementation in the repository root, and a separate variant under `rna/`. Some routines—especially embedded training and export—still require validation and/or complete implementation. Review the notes in this README before integrating the library into a production project.

## Architecture

The main network structure follows this pattern:

```text
Input layer → Hidden layer(s) → Output layer
```

The constructor receives the number of inputs, the number of outputs, the hidden-layer structure, and the learning rate. Weights and biases are maintained per layer.

### Components

| Component | Responsibility |
|---|---|
| `ENeuralNetwork` | Neural-network structure and operations |
| `EMatrix` | Matrix storage and operations |
| `ActivationFunction` | Activation-function identifiers |
| `FeedFoward()` | Computes activations through the output layer |
| `ComputeError()` | Computes errors used during backpropagation |
| `BackPropagation()` | Updates weights and biases using the learning rate |
| `Train()` | Training routine, with different signatures for Linux and Arduino |

## Implemented features

### Neural network

- Configure the number of input and output nodes.
- Configure one or more hidden layers.
- Initialize weights and biases through `EMatrix`.
- Forward propagation (*feedforward*).
- Error calculation and backpropagation.
- Select an activation function per layer.
- Print the network structure, weights, and biases.
- Platform-specific training routines for desktop and Arduino environments.

### Matrix operations

The `EMatrix` class provides addition, Hadamard multiplication, scalar multiplication, and element-wise function application.

The main API also provides free functions for subtraction, scalar multiplication, matrix multiplication, transposition, function mapping, comparison, *softmax*, binarization, and loss calculation.

### Activation functions

The activation-function identifiers available in the primary root implementation are:

| Identifier | Function |
|---|---|
| `ActivationFunction::_eSigmoid` | Sigmoid |
| `ActivationFunction::_eRelu` | ReLU |
| `ActivationFunction::_eTahn` | Hyperbolic tangent |
| `ActivationFunction::_eSin` | Sine |

All layers are initialized with sigmoid. You can change a layer's activation function with `ChangeActivationFunction(function, layer)`.

## Requirements

To build the desktop version in the repository root:

- Linux or an environment compatible with GNU Make.
- A C++ compiler (`g++` in the current Makefile).
- GNU Make.
- The ENet library available to the linker, because the Makefile adds `-lenet`.

For Arduino:

- Arduino IDE or an equivalent toolchain.
- The selected board's Arduino core.
- The library source and header files added to your project.

The desktop code uses C++ and standard libraries such as `<math.h>`, `<iostream>`, `<fstream>`, and `<sstream>`. Exact compatibility depends on the board core and compiler.

## Repository structure

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

The root `src/` directory is the primary implementation described by the API signatures in this README. The `rna/` directory contains a separate variant with differences in its metrics, tolerance, and export APIs. Do not mix files from the two trees in the same build: their headers and function signatures are not identical.

> The repository also contains versioned build artifacts (`.o` files and executables). For new builds, clean the project and recompile from source.

## Building on Linux

From the repository root:

```bash
make build
```

The `build` target compiles the `.cpp` files found by the Makefile and creates the `main` executable. Run it with:

```bash
./main
```

The default `all` target runs cleaning, compilation, and execution in sequence:

```bash
make
```

Other available targets:

```bash
make clean  # remove object files and executable
make debug  # launch the executable in GDB
make leak   # run the analysis with Valgrind
```

The `leak` target requires Valgrind. The Makefile links with `-lenet`; if ENet is not installed, install the dependency or adjust `NETFLAGS` if it is not required in your environment.

## Getting started

The following example creates a network with two inputs, one hidden layer containing two neurons, and one output:

```cpp
#include "src/eneural_network_lib/eneural_network_lib.hpp"

int main()
{
    int hiddenLayers[] = {2};

    ENeuralNetwork network(
        2,             // number of inputs
        1,             // number of outputs
        hiddenLayers,  // neurons in each hidden layer
        1,             // number of hidden layers
        0.1            // learning rate
    );

    network.print();

    return 0;
}
```

The constructor receives the network topology:

```cpp
ENeuralNetwork(
    int inputNodesNumber,
    int outputNodesNumber,
    int *hiddenNodesStructure,
    int hiddenLayersNumber,
    _ponto alfaLearn
);
```

For a network without hidden layers, set `hiddenLayersNumber = 0`. The hidden-layer pointer is not traversed in this case.

## Using EMatrix

`EMatrix` represents a dynamic matrix of `_ponto` values (defined as `double`):

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

Free matrix operations available in the primary API:

```cpp
EMatrix C = sub(&A, &B);
EMatrix D = scalar_multiplication(&A, 0.5);
EMatrix E = dot_multiplication(&A, &B);
EMatrix T = transpose(&A);
EMatrix P = map(&A, eSigmoide);
EMatrix S = softmax(&A);
EMatrix Binarized = binary(&A);
```

Matrix multiplication requires compatible dimensions: the number of columns in the first matrix must match the number of rows in the second matrix.

## Training and data

In the primary implementation, the training function signature differs by platform.

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

Files are read line by line. The helper function `get_next_matrix_data_from_stream()` splits values using a space (` `) and converts each value to `_ponto`. Therefore, input and expected-output files must have matching lines, with values separated by spaces.

Example format:

```text
0.0 1.0
1.0 0.0
0.5 0.5
```

Use one file for inputs and another for expected outputs, keeping the same number and order of lines in both files.

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

The embedded routine is present in the code, but parts of its validation and metric calculations are commented out. Consider it incomplete until the evaluation workflow has been implemented and tested on the target board.

## Selecting an activation function

The primary API lets you configure a layer's activation function by index:

```cpp
network.ChangeActivationFunction(
    ActivationFunction::_eRelu,
    0
);
```

Indices start at zero and identify transformation layers (not the input layer). The network initializes all layers with sigmoid.

## Known limitations

This repository is under development. The current code has several areas that should be reviewed before use in critical applications:

- **Arduino training:** parts of the validation and test calculations are commented out.
- **Export:** `eexport()` in the primary Linux implementation writes a header with an apparently incorrect include path; validate generated files before using them. The Arduino implementation of this routine is empty.
- **Dimensions:** several matrix operations assume compatible dimensions and do not fully validate their arguments.
- **Memory:** `EMatrix` manages memory manually. Test copying, object lifetimes, and intensive use before relying on it in long-running applications.
- **Training:** review the split between training, validation, and test data, and validate results against known datasets.
- **Files:** the data reader expects numeric values separated by spaces; it is not a generic CSV parser.

These notes describe the code reviewed for this README and do not guarantee behavior on every platform.

## Roadmap

The project's `todo` file lists pending work, including:

- Complete Arduino export.
- Adjust dataset-size calculations in the training routines.
- Complete Arduino training.
- Add metric and loss-function options.
- Expand the documentation.

## Technical documentation

Detailed documentation is available in [`docs/`](docs/README.md):

- [Architecture and implementation layout](docs/architecture.md)
- [Installation and building on Linux and Arduino](docs/build.md)
- [ENeuralNetwork class reference](docs/neural-network.md)
- [EMatrix class and numerical operations reference](docs/matrix.md)
- [Data format, training, and metrics](docs/data-and-training.md)
- [Usage examples](docs/examples.md)
- [Known limitations and technical roadmap](docs/known-limitations.md)

These pages distinguish the primary implementation in `src/` from the `rna/src/` variant, whose APIs are not interchangeable.

## Contributing

Suggestions, bug reports, and improvements are welcome. When proposing a change:

1. Describe the problem or improvement.
2. Include a minimal reproducible example when applicable.
3. Specify the platform, compiler, and Arduino core version when relevant.
4. Test the change on the target platform and document known limitations.

## License

This project is licensed under the **GNU General Public License v3.0 only (GPL-3.0-only)**. You may use, study, modify, and redistribute this project under the terms of the license.

See [LICENSE](LICENSE) for the complete license text. Third-party components and assets remain subject to their own licenses and notices.

---

<p align="center">
  Created by <a href="https://github.com/Edumarek123">Eduardo Marek</a>
</p>
