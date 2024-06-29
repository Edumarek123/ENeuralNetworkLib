//
// Created by Edumarek on 27/06/2021.
//

#ifndef ENEURAL_NETWORK_LIB_HPP
#define ENEURAL_NETWORK_LIB_HPP

#include "../ematrix_lib/ematrix_lib.hpp"

struct ENeuralNetwork{
    _ponto learningRate;
    size_t layersNumber;
    size_t nodesLayerX;
    size_t nodesLayerY;
    size_t *layersStructure;
    funType *funLayers;
    funType *DfunLayers;
    size_t outputLayerIndex;
    unsigned long allocatedBytes;

    EMatrix **weightsLayers;
    EMatrix **biasLayers;
    EMatrix **errorsLayers;
    EMatrix **outputsLayers;

    ENeuralNetwork(int inputNodesNumber, int outputNodesNumber, int *hiddenNodesStructure, int hiddenLayersNumber, _ponto alfaLearn);
    ENeuralNetwork()=delete;
    ~ENeuralNetwork();

    void print();
    void eload(_ponto *data);
    void eexport(const char *path);
    unsigned long allocated_memory();
    void FeedFoward(EMatrix inputs); 
    void ComputeError(EMatrix expectedOutputs);
    void BackPropagation(EMatrix inputs);  
    bool RealizarTeste(const EMatrix *entradas,const EMatrix *saidasEsperadas);
    void ChangeActivationFunction(int function, int layer);

    #if defined __linux__
    float Train(const std::string &pathInputs, const std::string &pathExpectedOutputs, const float testsPercentage, const int epochs, const _ponto coeficienteErro);
    #else
    float Train(const EMatrix *inputs, const EMatrix *outputs, const int dataLength, const float testsPercentage, const int epochs);
    #endif
};

#if defined __linux__
EMatrix *get_next_matrix_data_from_stream(const std::string linha);
unsigned long get_stream_data_len(const std::string caminho);
_ponto get_max_value_of_data_stream(const std::string caminho);
#endif

#endif //ENEURAL_NETWORK_LIB_HPP
