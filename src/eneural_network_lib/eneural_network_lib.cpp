//
// Created by Edumarek on 27/06/2021.
//

#include "eneural_network_lib.hpp"

#if defined __linux__
    #include <iostream>
    #include <fstream>
    #include <string>
    #include <sstream>
    #include <assert.h>
#else
    #include<Arduino.h>
#endif

#include <math.h>

//------------------CONSTRUTORES&DESTRUTORES-------------------//
ENeuralNetwork::ENeuralNetwork(int inputNodesNumber, int outputNodesNumber, int *hiddenNodesStructure, int hiddenLayersNumber, _ponto alfaLearn)
{
    nodesLayerX = inputNodesNumber;
    nodesLayerY = outputNodesNumber;
    layersNumber = hiddenLayersNumber + 2;
    outputLayerIndex = layersNumber - 2;

    learningRate = alfaLearn;

    layersStructure = (size_t *)malloc(layersNumber * sizeof(size_t));
    layersStructure[0] = nodesLayerX;

    for (size_t i = 1; i < layersNumber - 1; i++)
    {
        layersStructure[i] = hiddenNodesStructure[i - 1];
    }
    layersStructure[layersNumber - 1] = nodesLayerY;

    funLayers = (funType *)malloc((layersNumber - 1) * sizeof(funType));
    DfunLayers = (funType *)malloc((layersNumber - 1) * sizeof(funType));

    for (size_t i = 0; i < layersNumber - 1; i++)
    {
        ChangeActivationFunction(ActivationFunction::_eSigmoid, i);
    }

    biasLayers = (EMatrix **)malloc((layersNumber - 1) * sizeof(EMatrix *));
    for (size_t i = 0; i < layersNumber - 1; i++)
    {
        biasLayers[i] = new EMatrix(layersStructure[i + 1], 1);
    }

    weightsLayers = (EMatrix **)malloc((layersNumber - 1) * sizeof(EMatrix *));
    for (size_t i = 0; i < layersNumber - 1; i++)
    {
        weightsLayers[i] = new EMatrix(layersStructure[i + 1], layersStructure[i]);
    }

    errorsLayers = (EMatrix **)malloc((layersNumber - 1) * sizeof(EMatrix *));
    for (size_t i = 0; i < layersNumber - 1; i++)
    {
        errorsLayers[i] = new EMatrix(layersStructure[i + 1], 1);
    }

    outputsLayers = (EMatrix **)malloc((layersNumber - 1) * sizeof(EMatrix *));
    for (size_t i = 0; i < layersNumber - 1; i++)
    {
        outputsLayers[i] = new EMatrix(layersStructure[i + 1], 1);
    }

    allocatedBytes = allocated_memory();
}

ENeuralNetwork::~ENeuralNetwork()
{
    for (size_t i = 0; i < layersNumber - 1; i++)
    {
        delete biasLayers[i];
        delete weightsLayers[i];
        delete errorsLayers[i];
        delete outputsLayers[i];
    }

    free(biasLayers);
    free(weightsLayers);
    free(errorsLayers);
    free(outputsLayers);
    free(layersStructure);
    free(funLayers);
    free(DfunLayers);
}

//---------------------------METODOS---------------------------//

#if defined __linux__
void ENeuralNetwork::print()
{
    std::cout << std::endl << "Neural Network Structure( I | Wn | O )" << std::endl << std::endl;
    std::cout << "Nodes per layer" << std::endl << std::endl;
    std::cout << "|  X  | ";

    for (size_t i = 0; i < layersNumber - 2; i++)
    {
        std::cout << "HL" << i << " | ";
    }

    std::cout << " Y  |" << std::endl << "| ";
    for (size_t i = 0; i < layersNumber; i++)
    {
        std::cout << " " << layersStructure[i] << "  | ";
    }
    std::cout << std::endl << std::endl;

    std::cout << "Structure | Inputs | Weigths | Outputs |" << std::endl << std::endl;
    for (size_t i = 0; i < nodesLayerX; i++)
    {
        std::cout << "X" << std::endl;
    }
    std::cout << std::endl;

    for (size_t i = 0; i < layersNumber - 1; i++)
    {
        weightsLayers[i]->print();
        std::cout << std::endl;
    }

    for (size_t i = 0; i < nodesLayerY; i++)
    {
        std::cout << "Y" << std::endl;
    }
    std::cout << std::endl;

    std::cout << std::endl << "Bias" << std::endl << std::endl;
    for (size_t i = 0; i < layersNumber - 1; i++)
    {
        biasLayers[i]->print();
        std::cout << std::endl;
    }
}

#else
void ENeuralNetwork::print()
{
    Serial.print("\nNeural Network Structure( I | Wn | O )\n\n");

    Serial.print("Nodes per layer\n\n");
    Serial.print("| X |");
    for (size_t i = 0; i < layersNumber - 2; i++)
    {
        Serial.print("HL");
        Serial.print(i);
        Serial.print(" | ");
    }

    Serial.print(" Y |\n| ");
    for (size_t i = 0; i < layersNumber; i++)
    {
        Serial.print(" ");
        Serial.print(layersStructure[i]);
        Serial.print("  | ");
    }
    Serial.print("\n\n");

    Serial.print("Structure | Inputs | Weigths | Outputs |\n\n");
    for (size_t i = 0; i < nodesLayerX; i++)
    {
        Serial.print("X\n");
    }
    Serial.print("\n");

    for (size_t i = 0; i < layersNumber - 1; i++)
    {
        weightsLayers[i]->print();
        Serial.print("\n");
    }

    for (size_t i = 0; i < nodesLayerY; i++)
    {
        Serial.print("Y\n");
    }
    Serial.print("\n");

    Serial.print("\nBias\n\n");
    for (size_t i = 0; i < layersNumber - 1; i++)
    {
        biasLayers[i]->print();
        Serial.print("\n");
    }
}
#endif

void ENeuralNetwork::eload(_ponto *data)
{
    long int cont = 0;

    for (size_t i = 0; i < layersNumber - 1; i++){
        for (size_t j = 0; j < weightsLayers[i]->lines; j++)
        {
            for (size_t k = 0; k < weightsLayers[i]->columns; k++)
            {
                weightsLayers[i]->matrix[j][k] = data[cont];
                cont++;
            }
        }
    }

    for (size_t i = 0; i < layersNumber - 1; i++)
    {
        for (size_t j = 0; j < biasLayers[i]->lines; j++)
        {
            for (size_t k = 0; k < biasLayers[i]->columns; k++)
            {
                biasLayers[i]->matrix[j][k] = data[cont];
                cont++;
            }
        }
    }
}

#if defined __linux__
void ENeuralNetwork::eexport(const char *path)
{
    std::ofstream file(path, std::ofstream::out);

    if (file.is_open())
    {
        file << "#ifndef EXPORTED_RNA_HPP\n#define EXPORTED_RNA_HPP\n\n";
        file << "#include \"src / matrix_lib / matrix_lib.hpp \"\n\n";
        file << "_ponto RNA_DATA[] = {";

        for (size_t i = 0; i < layersNumber - 1; i++)
        {
            for (size_t j = 0; j < weightsLayers[i]->lines; j++)
            {
                for (size_t k = 0; k < weightsLayers[i]->columns; k++)
                {
                    file << weightsLayers[i]->matrix[j][k] << ", ";
                }
            }
        }

        for (size_t i = 0; i < layersNumber - 1; i++)
        {
            for (size_t j = 0; j < biasLayers[i]->lines; j++)
            {
                for (size_t k = 0; k < biasLayers[i]->columns; k++)
                {
                    if (i == layersNumber - 2 && j == biasLayers[i]->lines - 1 && k == biasLayers[i]->columns - 1)
                    {
                        file << biasLayers[i]->matrix[j][k];
                    }
                    else
                    {
                        file << biasLayers[i]->matrix[j][k] << ", ";
                    }
                }
            }
        }

        file << "};";
        file << "#endif //EXPORTED_RNA_HPP";
    }
    
    file.close();
}
#else
void ENeuralNetwork::eexport(const char *s)
{
    
}
#endif


unsigned long ENeuralNetwork::allocated_memory()
{
    unsigned long bytes = 0;

    bytes += sizeof(size_t) * layersNumber; 

    bytes += (layersNumber - 1) * sizeof(funType);
    bytes += (layersNumber - 1) * sizeof(DfunType);


    // EMatrix **weightsLayers;
    // EMatrix **biasLayers;
    // EMatrix **errorsLayers;
    // EMatrix **outputsLayers;


    return bytes;
}

void ENeuralNetwork::FeedFoward(EMatrix inputs)
{
    // z_l=W_l.X+B
    // z_l=W_l.a_(l-1)+B
    // a_l=6z_l

    for (size_t i = 0; i < layersNumber - 1; i++)
    {
        delete outputsLayers[i];
    }

    outputsLayers[0] = new EMatrix(dot_multiplication(weightsLayers[0], &inputs));
    outputsLayers[0]->sum(biasLayers[0]);
    outputsLayers[0]->map(funLayers[0]); //"f"

    for (size_t i = 1; i < layersNumber - 1; i++)
    {
        outputsLayers[i] = new EMatrix(dot_multiplication(weightsLayers[i], outputsLayers[i - 1]));
        outputsLayers[i]->sum(biasLayers[i]);
        outputsLayers[i]->map(funLayers[i]); //"f"
    }
}

void ENeuralNetwork::ComputeError(EMatrix expectedOutputs)
{
    // e_L = (A_L - Y) 0 d6(z_L)
    // e_l = ((W_(l+1))T(erro_(l+1))) 0 d6(z_l)

    delete errorsLayers[layersNumber - 2];
    errorsLayers[layersNumber - 2] = new EMatrix(sub(&expectedOutputs, outputsLayers[layersNumber - 2]));
    EMatrix aux1(map(outputsLayers[layersNumber - 2], DfunLayers[layersNumber - 2])); //"Df"
    errorsLayers[layersNumber - 2]->hadamard_multiplication(&aux1);

    for (size_t i = layersNumber - 3; i >= 0; i--)
    {
        delete errorsLayers[i];
        EMatrix aux2(transpose(weightsLayers[i + 1]));
        errorsLayers[i] = new EMatrix(dot_multiplication(&aux2, errorsLayers[i + 1]));
        EMatrix aux3(map(outputsLayers[i], DfunLayers[i])); //"Df"
        errorsLayers[i]->hadamard_multiplication(&aux3);
    }
}

void ENeuralNetwork::BackPropagation(EMatrix inputs)
{
    // ∇Cw_l = a_(l-1) * e_l
    // ∇Cb_l = e_l

    for (size_t i = layersNumber - 2; i >= 1; i--)
    {
        EMatrix aux1(transpose(outputsLayers[i - 1]));
        EMatrix aux2(dot_multiplication(errorsLayers[i], &aux1));
        EMatrix aux3(scalar_multiplication(&aux2, learningRate));
        weightsLayers[i]->sum(&aux3);

        EMatrix aux4(scalar_multiplication(errorsLayers[i], learningRate));
        biasLayers[i]->sum(&aux4);
    }

    EMatrix aux1(transpose(&inputs));
    EMatrix aux2(dot_multiplication(errorsLayers[0], &aux1));
    EMatrix aux3(scalar_multiplication(&aux2, learningRate));
    weightsLayers[0]->sum(&aux3);

    EMatrix aux4(scalar_multiplication(errorsLayers[0], learningRate));
    biasLayers[0]->sum(&aux4);
}

#if defined __linux__
float ENeuralNetwork::Train(const std::string &pathInputs, const std::string &pathExpectedOutputs, const float testsPercentage, const int epochs, const _ponto coeficienteErro)
{
    std::ifstream inputsStream;
    std::ifstream expectedOutputsStream;
    inputsStream.open(pathInputs);
    expectedOutputsStream.open(pathExpectedOutputs);
    unsigned long dataLength = get_stream_data_len(pathInputs);
    unsigned long testDataLength = (int)((dataLength * testsPercentage) / 100);        // x% para testes ao final
    unsigned long trainTestDataLength = (int)(dataLength * 0.05);                                // 5% para testes durante treinamento
    unsigned long trainDataLength = dataLength - testDataLength - trainTestDataLength; // o restante para treinamento

    _ponto entradaMax = get_max_value_of_data_stream(pathExpectedOutputs);
    _ponto limiteLoss = pow(entradaMax * coeficienteErro, 2);

    size_t cont = 0;
    _ponto accuracy = 0;
    _ponto _loss = 1;
    _ponto _val_loss = 1;
    std::string SInputs, SExpectedOutputs;

    if (inputsStream.is_open() && expectedOutputsStream.is_open())
    {
        int e = 0;
        while (e < epochs || accuracy < 0.2)
        {
            if (accuracy < 1)
            {
                inputsStream.clear();
                expectedOutputsStream.clear();
                inputsStream.seekg(0);
                expectedOutputsStream.seekg(0);

                cont = 0;
                _loss = 0;
                while (inputsStream && outputsLayers && cont < trainDataLength)
                {
                    std::getline(inputsStream, SInputs);
                    std::getline(expectedOutputsStream, SExpectedOutputs);
                    if (SInputs.size() != 0 || SExpectedOutputs.size() != 0)
                    {
                        cont++;

                        EMatrix *MInput(get_next_matrix_data_from_stream(SInputs));
                        EMatrix *MexpectedOutput(get_next_matrix_data_from_stream(SExpectedOutputs));

                        FeedFoward(*MInput);
                        ComputeError(*MexpectedOutput);
                        BackPropagation(*MInput);
                        _loss += loss(MexpectedOutput, outputsLayers[outputLayerIndex]);

                        delete MInput;
                        delete MexpectedOutput;
                    }
                }
                _loss /= trainDataLength;

                accuracy = 0;
                _val_loss = 0;
                while (inputsStream && outputsLayers && cont < trainDataLength + trainTestDataLength)
                {
                    std::getline(inputsStream, SInputs);
                    std::getline(expectedOutputsStream, SExpectedOutputs);
                    if (SInputs.size() != 0 || SExpectedOutputs.size() != 0)
                    {
                        cont++;

                        EMatrix *MInput(get_next_matrix_data_from_stream(SInputs));
                        EMatrix *MexpectedOutput(get_next_matrix_data_from_stream(SExpectedOutputs));

                        FeedFoward(*MInput);

                        _ponto aux_loss = loss(MexpectedOutput, outputsLayers[outputLayerIndex]);
                        if (aux_loss < limiteLoss)
                            accuracy++;
                        _val_loss += aux_loss;

                        delete MInput;
                        delete MexpectedOutput;
                    }
                }
                accuracy /= trainTestDataLength;
                _val_loss /= trainTestDataLength;
                std::cout << e + 1 << "/" << epochs << " | accuracy: " << accuracy << " | loss: " << _loss << " | val_loss: " << _val_loss << std::endl;
            }
            e++;
        }
        accuracy = 0;
        _val_loss = 0;
        if (testDataLength > 0)
        {
            while (inputsStream && outputsLayers && cont < trainDataLength + trainTestDataLength + testDataLength)
            {
                std::getline(inputsStream, SInputs);
                std::getline(expectedOutputsStream, SExpectedOutputs);
                if (SInputs.size() != 0 || SExpectedOutputs.size() != 0)
                {
                    cont++;

                    EMatrix *MInput(get_next_matrix_data_from_stream(SInputs));
                    EMatrix *MexpectedOutput(get_next_matrix_data_from_stream(SExpectedOutputs));

                    FeedFoward(*MInput);

                    _ponto aux_loss = loss(MexpectedOutput, outputsLayers[outputLayerIndex]);
                    if (aux_loss < limiteLoss)
                        accuracy++;
                    _val_loss += aux_loss;

                    delete MInput;
                    delete MexpectedOutput;
                }
            }
            accuracy /= testDataLength;
            _val_loss /= testDataLength;
            std::cout << "Accuracy testes: " << accuracy << " | val_loss: " << _val_loss << std::endl;
        }

        inputsStream.close();
        expectedOutputsStream.close();
    }
    else
    {
        std::cout << "erro ao abrir arquivos" << std::endl;
        return -1;
    }

    return accuracy;
}

#else
float ENeuralNetwork::Train(const EMatrix *inputs, const EMatrix *expectedOutputs, const int dataLength, const float testsPercentage, const int epochs)
{
    int testDataLength = (int)((dataLength * testsPercentage) / 100);        // x% para testes ao final
    int trainTestDataLength = (int)(dataLength * 0.05);                                // 5% para testes durante treinamento
    int trainDataLength = dataLength - testDataLength - trainTestDataLength; // o restante para treinamento

    size_t cont = 0;
    float accuracy = 0;
    float loss = 1;
    for (size_t e = 0; e < epochs; e++)
    {
        if ((accuracy < 1 && loss > 0.001) || dataLength < 50)
        {
            cont = 0;
            accuracy = 0;
            loss = 0;
            while (cont < trainDataLength)
            {
                FeedFoward(inputs[cont]);
                ComputeError(expectedOutputs[cont]);
                BackPropagation(inputs[cont]);
                cont++;
            }

            while (cont < trainDataLength + trainTestDataLength)
            {
                    // if(RealizarTeste(inputs[cont], expectedOutputs[cont]))
                    // {
                        // accuracy++;
                    // }

                //     loss += loss(expectedOutputs[cont], outputsLayers[outputLayerIndex]);
                //     cont++;
            }
            accuracy /= trainTestDataLength;
            loss /= trainTestDataLength;
            Serial.println(String(e + 1) + "/" + String(epochs) + " | accuracy: " + String(accuracy) + " | val_loss: " + String(loss));
        }
    }
    accuracy = 0;
    loss = 0;
    if (testDataLength > 0)
    {
        while (cont < trainDataLength + trainTestDataLength + testDataLength)
        {
            // if(RealizarTeste(inputs[cont], expectedOutputs[cont]))
            // {
                // accuracy++;
            // }

            // loss+=loss(expectedOutputs[cont], outputsLayers[outputLayerIndex]);

            // cont++;
        }
        accuracy /= testDataLength;
        loss /= testDataLength;
        Serial.println("Accuracy testes: " + String(accuracy) + " | loss: " + String(loss));
    }
    return accuracy;
}
#endif

void ENeuralNetwork::ChangeActivationFunction(int function, int layer)
{
    switch(function)
    {
        case ActivationFunction::_eSigmoid:
            funLayers[layer] = eSigmoide;
            DfunLayers[layer] = eDsigmoide;
            break;

        case ActivationFunction::_eRelu:
            funLayers[layer] = eReLU;
            DfunLayers[layer] = eDReLU;
            break;

        case ActivationFunction::_eTahn:
            funLayers[layer] = eTanh;
            DfunLayers[layer] = eDTanh;
            break;

        case ActivationFunction::_eSin:
            funLayers[layer] = eSin;
            DfunLayers[layer] = eSin;
            break;

        default:
            funLayers[layer] = eSigmoide;
            DfunLayers[layer] = eDsigmoide;
            break;
    }
}

bool ENeuralNetwork::RealizarTeste(const EMatrix *entradas, const EMatrix *outputsLayersEsperadas)
{
    FeedFoward(*entradas);

    // outputsLayers[outputLayerIndex]->binary();
    double l = 0.01;

    if (loss(outputsLayersEsperadas, outputsLayers[outputLayerIndex]) < l){
        return true;
    }
    return false;
}

//----------------------FUNCOES AUXILIARES---------------------//
#if defined __linux__
unsigned long get_stream_data_len(const std::string caminho)
{
    unsigned long cont = 0;
    std::fstream arquivo;
    std::string linha;

    arquivo.open(caminho);

    do
    {
        getline(arquivo, linha);
        cont++;
    } while (arquivo);

    return cont - 1;
}

EMatrix *get_next_matrix_data_from_stream(const std::string linha)
{
    std::istringstream iss(linha);
    std::string valor;
    _ponto *dados = (_ponto *)malloc(sizeof(_ponto));
    size_t i = 0;

    while (std::getline(iss, valor, ' '))
    {
        if (i != 0)
            dados = (_ponto *)realloc(dados, (i + 1) * sizeof(_ponto));
        dados[i] = (std::stof(valor));
        i++;
    }
    EMatrix *coleta = new EMatrix(i, 1);

    for (size_t j = 0; j < i; j++)
        coleta->matrix[j][0] = dados[j];

    free(dados);

    return coleta;
}

_ponto get_max_value_of_data_stream(const std::string caminho)
{
    _ponto max = -1;
    std::fstream arquivo;
    std::string linha;
    std::string valor;

    arquivo.open(caminho);

    do
    {
        getline(arquivo, linha);
        std::istringstream iss(linha);
        while (std::getline(iss, valor, ' '))
        {
            _ponto novoValor = std::stof(valor);
            if (novoValor > max)
                max = novoValor;
        }

    } while (arquivo);

    return max;
}
#endif
