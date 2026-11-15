# Instalação e compilação

## Linux / desktop

Requisitos: GNU Make, `g++` e biblioteca ENet disponível para o linker, conforme `NETFLAGS = -lenet` no Makefile. GDB e Valgrind são opcionais.

Na raiz:

```bash
make clean
make build
./main
```

O Makefile procura arquivos `.cpp` recursivamente, compila objetos e vincula `main`. O alvo padrão `make` executa limpeza, compilação e execução em sequência.

| Comando | Ação |
|---|---|
| `make build` | Compila e vincula `main` |
| `make run` | Executa `./main` |
| `make clean` | Remove objetos e executável |
| `make debug` | Abre `main` no GDB |
| `make leak` | Executa Valgrind |
| `make` | Limpa, compila e executa |

Se a linkedição falhar por ENet ausente e o código usado não depender dela, revise `NETFLAGS`. Não remova a flag sem verificar as dependências.

## Arduino

A raiz contém `rna.ino`, que inclui:

```cpp
#include "src/eneural_network_lib/eneural_network_lib.hpp"
```

Para usar no Arduino, preserve a hierarquia relativa de `src/ematrix_lib` e `src/eneural_network_lib`, selecione a placa/core e compile. A implementação escolhe entre headers desktop e `Arduino.h` por macros de plataforma.

O sketch atual apenas inicializa Serial e constrói uma rede. Ele não demonstra inferência nem treinamento completos. Valide memória e compatibilidade no hardware final.

## Variante rna/

A pasta `rna/` possui seu próprio Makefile e fontes. Compile essa árvore isoladamente; não combine seus objetos com os da raiz. As assinaturas das APIs não são idênticas.