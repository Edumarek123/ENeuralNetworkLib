# Dados e treinamento

## Formato dos arquivos desktop

A rotina recebe caminhos de entrada e de saída esperada. Cada linha representa uma amostra; os números são separados por espaços, não por vírgulas.

Entradas:

```text
0.0 0.0
0.0 1.0
1.0 0.0
1.0 1.0
```

Saídas esperadas:

```text
0.0
1.0
1.0
0.0
```

Os arquivos devem ter a mesma quantidade e ordem de linhas. Cada linha deve conter valores compatíveis com a dimensão do vetor de entrada/saída.

## Treinamento na versão raiz

```cpp
network.Train("inputs.txt", "expected.txt", 20.0f, 500, 0.05);
```

Parâmetros: caminhos de dados, percentual para teste final, número de épocas e coeficiente usado para limite interno de erro. A implementação separa treino, validação interna e teste final de modo sequencial.

**Atenção:** a lógica da rotina raiz tem particularidades no loop e na divisão de dados. Valide com amostras conhecidas antes de interpretar a métrica retornada.

## Variante rna/

A variante tem assinatura diferente:

```cpp
float Train(const std::string &pathInputs,
            const std::string &pathExpectedOutputs,
            double testsPercentage, size_t epochs);
```

O campo `testDataInTrainPercentage` inicia em 0.05. A rotina imprime métricas por época. A versão Arduino recebe arrays de `EMatrix` e percorre sequencialmente treino, validação e teste; organize os dados nessa ordem e teste os limites dos subconjuntos.

## Métricas da variante rna/

O namespace `MetricFunction` lista `_eMeanError`, `_eMeanSquaredError`, `_eMeanAbsoluteError`, `_eMeanAbsolutePercentageError`, `_eAccuracy`, `_eBinaryAccuracy`, `_eCategoricalAccuracy` e `_eSum`.

A escolha é feita por `ChangeMetricFunction()`. No código atual nem todos os identificadores mapeiam para métricas com o mesmo significado: `_eAccuracy` e `_eBinaryAccuracy` apontam para MSE e o identificador categórico não possui mapeamento específico. A métrica MSE atual também calcula o quadrado da média das diferenças, não a média dos quadrados. Revise as funções antes de usar como avaliação estatística.

## Recomendações

1. Comece com topologia pequena.
2. Normalize dados e mantenha escalas coerentes com as ativações.
3. Separe dados de treino, validação e teste.
4. Registre topologia, taxa de aprendizado e seed.
5. Compare resultados contra casos esperados conhecidos.
6. Faça uma avaliação independente antes de concluir que o modelo generaliza.

O parser não é CSV genérico, não oferece normalização automática nem embaralhamento integrado.