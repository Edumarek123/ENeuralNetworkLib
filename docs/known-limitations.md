# Limitações conhecidas e roadmap

Esta página reúne pontos identificados na leitura do código. Não são garantias sobre todas as plataformas.

## Implementação principal (`src/`)

### Memória

- `EMatrix` usa `malloc/free` e gerencia as linhas manualmente.
- Há construtor de cópia profunda, mas não operadores explícitos de atribuição/movimento.
- A atribuição implícita pode copiar o ponteiro interno e causar double free ou uso após liberação.
- Falhas de alocação não são tratadas de forma uniforme.

### Dimensões

Operações matriciais presumem compatibilidade e não validam todos os tamanhos. Entradas incompatíveis podem causar acessos inválidos.

### Treinamento

- Divisão sequencial entre treino, validação e teste.
- Rotina Arduino contém trechos de validação/teste comentados.
- O teste tem limiar fixo.
- Não há API de minibatch, shuffle ou otimizadores.

### Exportação

- O include gravado por `eexport()` na raiz contém espaços no caminho e aparenta estar incorreto.
- Exportação Arduino da raiz está vazia.
- `eload()` não recebe tamanho do buffer.
- `allocated_memory()` não considera toda a memória das matrizes.

## Variante rna/

- `eexport(_ponto *data)` para Arduino libera/aloca um ponteiro recebido por valor; o endereço atualizado não retorna ao chamador.
- Alguns identificadores de métrica mapeiam para funções de significado diferente.
- MSE calcula quadrado da média das diferenças, não média dos quadrados.
- MAE/MAPE precisam ser revistas para aplicar módulo por elemento e tratar denominadores zero.
- A métrica de acurácia compara diferenças sem valor absoluto.
- Treinamento pode dividir por tamanhos de subconjunto zero em conjuntos pequenos.

## Pendências do arquivo todo

- Exportação Arduino.
- Ajuste dos tamanhos dos conjuntos de dados.
- Treinamento Arduino.
- Opções de métricas e perdas.
- Documentação.

A documentação cobre o estado observado, mas não corrige esses pontos.

## Roadmap sugerido

1. Adotar RAII e implementar a regra dos cinco.
2. Validar dimensões e tratar erros.
3. Corrigir métricas matemáticas e criar testes unitários.
4. Separar treino/validação/teste com índices explícitos e shuffle opcional.
5. Revisar loop e critério de parada.
6. Criar formato versionado de persistência dos pesos.
7. Corrigir exportação Arduino com contrato de buffer claro.
8. Adicionar CMake, CI e testes automatizados.
9. Consolidar as duas implementações e definir uma API oficial.