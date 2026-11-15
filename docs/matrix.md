# Referência: EMatrix

Arquivo: `src/ematrix_lib/ematrix_lib.hpp`.

## Tipos

```cpp
typedef double _ponto;
typedef _ponto (*funType)(_ponto);
```

A estrutura pública contém `lines`, `columns` e `_ponto **matrix`.

## Construção

```cpp
EMatrix();                    // 0 × 0
EMatrix(size_t lin, size_t col);
EMatrix(const EMatrix &m);    // cópia profunda
~EMatrix();
```

O construtor dimensional preenche valores aleatórios. Para preencher com constante:

```cpp
EMatrix input(3, 1);
input.initilize_values(0.0, false);
```

O gerador atual usa `rand()`, selecionando valores em passos de 0.1 entre -1.0 e 1.0 e repetindo quando o valor é zero.

## Métodos de instância

| Método | Comportamento |
|---|---|
| `initilize_values(n, isRand)` | Preenche com constante ou aleatórios |
| `print()` | Imprime a matriz |
| `sum(m)` | Soma elemento a elemento |
| `hadamard_multiplication(m)` | Produto elemento a elemento |
| `scalar_multiplication(n)` | Multiplica por escalar |
| `map(f)` | Aplica função a todos os elementos |
| `verificar_igualdade(m)` | Compara valores exatamente |
| `softmax()` | Softmax de vetor linha ou coluna |
| `binary()` | Limiar 0.5: maior vira 1, demais 0 |

## Funções livres

`sub(A,B)`, `scalar_multiplication(M,n)`, `dot_multiplication(A,B)`, `transpose(M)`, `map(M,f)`, `verificar_igualdade(A,B)`, `softmax(M)`, `binary(M)` e `loss(A,B)`.

Para produto matricial, `A.columns` deve ser igual a `B.lines`; o resultado tem forma `A.lines × B.columns`. A API presume dimensões compatíveis e não valida todos os casos.

## Funções de ativação

| Função | Fórmula |
|---|---|
| `eSigmoide(x)` | `1 / (1 + exp(-x))` |
| `eDsigmoide(x)` | `x * (1 - x)` (entrada já ativada) |
| `eReLU(x)` | `max(0, x)` |
| `eDReLU(x)` | 1 se x > 0; caso contrário 0 |
| `eTanh(x)` | `tanh(x)` |
| `eDTanh(x)` | `1 - tanh(x)^2` |
| `eSin(x)` | `sin(x)` |

## Cuidados de memória

A implementação usa `malloc/free`. Há construtor de cópia profunda, mas não operadores de atribuição/movimento explícitos. A atribuição padrão pode copiar apenas o ponteiro interno; evite atribuição por valor até a classe implementar corretamente a regra dos cinco.