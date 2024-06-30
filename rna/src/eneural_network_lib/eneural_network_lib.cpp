//
// Created by Edumarek on 27/06/2021.
//

#include "eneural_network_lib.hpp"

#if notEmbedded
    #include <iostream>
    #include <fstream>
    #include <string>
    #include <sstream>
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

    ChangeMetricFunction(MetricFunction::_eMeanSquaredError);

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

void ENeuralNetwork::print()
{
    generic_print("\nNeural Network Structure( I | Wn | O )\n\n");
    generic_print("Nodes per layer\n\n");
    generic_print("|  X  | ");

    for (size_t i = 0; i < layersNumber - 2; i++)
    {
        generic_print("HL");
        generic_print_v(i);
        generic_print(" | ");
    }

    generic_print(" Y  |\n| ");
    for (size_t i = 0; i < layersNumber; i++)
    {
        generic_print(" ");
        generic_print_v(layersStructure[i]);
        generic_print("  | ");
    }
    generic_print("\n\n");

    generic_print("Structure | Inputs | Weigths | Outputs |\n\n");
    for (size_t i = 0; i < nodesLayerX; i++)
    {
        generic_print("X\n");
    }
    generic_print("\n");

    for (size_t i = 0; i < layersNumber - 1; i++)
    {
        weightsLayers[i]->print();
        generic_print("\n");
    }

    for (size_t i = 0; i < nodesLayerY; i++)
    {
        generic_print("Y\n");
    }
    generic_print("\n");

    generic_print("\nBias\n\n");
    for (size_t i = 0; i < layersNumber - 1; i++)
    {
        biasLayers[i]->print();
        generic_print("\n");
    }
    generic_print("\n");
}

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

#if notEmbedded
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
void ENeuralNetwork::eexport(_ponto *data)
{
    size_t dataLen = 0;
    for (size_t i = 0; i < layersNumber - 1; i++)
    {
        dataLen += weightsLayers[i]->lines * weightsLayers[i]->columns; 
        dataLen += biasLayers[i]->lines * biasLayers[i]->columns; 
    }

    free(data);
    data = (_ponto *) malloc(dataLen * sizeof(_ponto));

    size_t cont = 0;
    for (size_t i = 0; i < layersNumber - 1; i++)
        {
            for (size_t j = 0; j < weightsLayers[i]->lines; j++)
            {
                for (size_t k = 0; k < weightsLayers[i]->columns; k++)
                {
                    data[cont] = weightsLayers[i]->matrix[j][k];
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
                    data[cont] = biasLayers[i]->matrix[j][k];
                    cont++;
                }
            }
        }
}
#endif


unsigned long ENeuralNetwork::allocated_memory()
{
    unsigned long bytes = 0;

    bytes += sizeof(size_t) * layersNumber; 
    bytes += ((layersNumber - 1) * sizeof(funType) * 2);


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

#if notEmbedded
float ENeuralNetwork::Train(const std::string &pathInputs, const std::string &pathExpectedOutputs, const double testsPercentage, const size_t epochs)
{
    std::ifstream inputsStream;
    std::ifstream expectedOutputsStream;
    
    inputsStream.open(pathInputs);
    expectedOutputsStream.open(pathExpectedOutputs);

    unsigned long dataLength = get_stream_data_len(pathInputs);
    unsigned long testDataLength = (int)((dataLength * testsPercentage) / 100.0);
    unsigned long trainTestDataLength = (int)(dataLength * testDataInTrainPercentage);              
    unsigned long trainDataLength = dataLength - testDataLength - trainTestDataLength;

    size_t fileCursor = 0;

    double accuracy = 0.0;
    double loss = 0.0;
    double validationAccuracy = 0.0;
    double validationLoss = 0.0;

    std::string SInputs, SExpectedOutputs;

    //TRAIN ROUTINE
    if (!(inputsStream.is_open() && expectedOutputsStream.is_open()))
    {
        std::cout << "Erro ao abrir arquivos" << std::endl;
        return -1;
    }

    for(size_t e = 0; e < epochs; e++)
    {
        inputsStream.clear();
        expectedOutputsStream.clear();
        inputsStream.seekg(0);
        expectedOutputsStream.seekg(0);

        fileCursor = 0;

        //TRAIN

        accuracy = 0.0;        
        loss = 0.0;
        
        while (inputsStream && outputsLayers && fileCursor < trainDataLength)
        {
            std::getline(inputsStream, SInputs);
            std::getline(expectedOutputsStream, SExpectedOutputs);
            if (SInputs.size() != 0 || SExpectedOutputs.size() != 0)
            {
                fileCursor++;

                EMatrix *MInput(get_next_matrix_data_from_stream(SInputs));
                EMatrix *MexpectedOutput(get_next_matrix_data_from_stream(SExpectedOutputs));

                FeedFoward(*MInput);
                ComputeError(*MexpectedOutput);
                BackPropagation(*MInput);

                loss += metricFunction(MexpectedOutput, outputsLayers[outputLayerIndex]);
                accuracy += eAccuracy(MexpectedOutput, outputsLayers[outputLayerIndex]);

                delete MInput;
                delete MexpectedOutput;
            }
        }
        loss /= trainDataLength;
        accuracy /= trainDataLength;

        //TEST

        validationAccuracy = 0.0;
        validationLoss = 0.0;

        while (inputsStream && outputsLayers && fileCursor < trainDataLength + trainTestDataLength)
        {
            std::getline(inputsStream, SInputs);
            std::getline(expectedOutputsStream, SExpectedOutputs);
            if (SInputs.size() != 0 || SExpectedOutputs.size() != 0)
            {
                fileCursor++;

                EMatrix *MInput(get_next_matrix_data_from_stream(SInputs));
                EMatrix *MexpectedOutput(get_next_matrix_data_from_stream(SExpectedOutputs));

                FeedFoward(*MInput);

                validationLoss += metricFunction(MexpectedOutput, outputsLayers[outputLayerIndex]);
                validationAccuracy += eAccuracy(MexpectedOutput, outputsLayers[outputLayerIndex]);

                delete MInput;
                delete MexpectedOutput;
            }
        }

        validationAccuracy /= trainTestDataLength;
        validationLoss /= trainTestDataLength;

        std::cout << e + 1 << "/" << epochs << " | accuracy: " << accuracy << " | validation_accuracy: " << validationAccuracy << " | loss: " << loss << " | validation_loss: " << validationLoss << std::endl;
    }

    //TESTS ROUTINE

    accuracy = 0.0;
    loss = 0.0;

    if (testDataLength > 0)
    {
        while (inputsStream && outputsLayers && fileCursor < trainDataLength + trainTestDataLength + testDataLength)
        {
            std::getline(inputsStream, SInputs);
            std::getline(expectedOutputsStream, SExpectedOutputs);
            if (SInputs.size() != 0 || SExpectedOutputs.size() != 0)
            {
                fileCursor++;

                EMatrix *MInput(get_next_matrix_data_from_stream(SInputs));
                EMatrix *MexpectedOutput(get_next_matrix_data_from_stream(SExpectedOutputs));

                FeedFoward(*MInput);

                loss += metricFunction(MexpectedOutput, outputsLayers[outputLayerIndex]);
                accuracy += eAccuracy(MexpectedOutput, outputsLayers[outputLayerIndex]);

                delete MInput;
                delete MexpectedOutput;
            }
        }
        accuracy /= testDataLength;
        loss /= testDataLength;
        std::cout << "Accuracy testes: " << accuracy << " | loss: " << loss << std::endl;
    }

    inputsStream.close();
    expectedOutputsStream.close();

    return accuracy;
}

#else
float ENeuralNetwork::Train(const EMatrix *inputs, const EMatrix *expectedOutputs, const int dataLength, const double testsPercentage, const size_t epochs)
{
    int testDataLength = (int)((dataLength * testsPercentage) / 100.0);
    int trainTestDataLength = (int)(dataLength * testDataInTrainPercentage);
    int trainDataLength = dataLength - testDataLength - trainTestDataLength;

    size_t cursor = 0;

    double accuracy = 0.0;
    double loss = 0.0;
    double validationLoss = 0.0;
    double validationAccuracy = 0.0;

    //TRAIN ROUTINE
    for (size_t e = 0; e < epochs; e++)
    {
        cursor = 0;

        //TRAIN

        accuracy = 0.0;
        loss = 0.0;

        while (cursor < trainDataLength)
        {
            FeedFoward(inputs[cursor]);
            ComputeError(expectedOutputs[cursor]);
            BackPropagation(inputs[cursor]);

            loss += metricFunction(&expectedOutputs[cursor], outputsLayers[outputLayerIndex]);
            accuracy += eAccuracy(&expectedOutputs[cursor], outputsLayers[outputLayerIndex]);

            cursor++;
        }

        //TEST

        validationAccuracy = 0.0;
        validationLoss = 0.0;

        while (cursor < trainDataLength + trainTestDataLength)
        {
            FeedFoward(inputs[cursor]);

            validationLoss += metricFunction(&expectedOutputs[cursor], outputsLayers[outputLayerIndex]);
            validationAccuracy += eAccuracy(&expectedOutputs[cursor], outputsLayers[outputLayerIndex]);

            cursor++;
        }

        validationAccuracy /= trainTestDataLength;
        validationLoss /= trainTestDataLength;

        Serial.println(String(e + 1) + "/" + String(epochs) + " | accuracy: " + String(accuracy) + " | validation_accuracy: " + String(validationAccuracy) + " | loss: " + String(loss) + " | validation_loss: " + String(validationLoss));
    }

    //TEST ROUTINE
    
    accuracy = 0.0;
    loss = 0.0;

    if (testDataLength > 0)
    {
        while (cursor < trainDataLength + trainTestDataLength + testDataLength)
        {
            FeedFoward(inputs[cursor]);

            validationLoss += metricFunction(&expectedOutputs[cursor], outputsLayers[outputLayerIndex]);
            validationAccuracy += eAccuracy(&expectedOutputs[cursor], outputsLayers[outputLayerIndex]);

            cursor++;
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

void ENeuralNetwork::ChangeMetricFunction(int function)
{
    switch(function){
        case MetricFunction::_eMeanError:
            metricFunction = eMean_error;
            break;
        case MetricFunction::_eMeanSquaredError:
            metricFunction = eMean_squared_error;
            break;
        case MetricFunction::_eMeanAbsoluteError:
            metricFunction = eMean_absolute_error;
            break;
        case MetricFunction::_eMeanAbsolutePercentageError:
            metricFunction = eMean_absolute_percentage_error;
            break;
        case MetricFunction::_eAccuracy:
            metricFunction = eMean_squared_error;
            break;
        case MetricFunction::_eBinaryAccuracy:
            metricFunction = eMean_squared_error;
            break;
        case MetricFunction::_eSum:
            metricFunction = eSum;
            break;
        default:
            return;
    }
}

//----------------------FUNCOES AUXILIARES---------------------//
#if notEmbedded
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
#endif
