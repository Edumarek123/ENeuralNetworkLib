//
// Created by Edumarek on 21/06/2021.
//

#ifndef MATRIX_LIB_HPP
#define MATRIX_LIB_HPP

#define notEmbedded defined(__linux__) || defined(__APPLE__) || defined(_WIN32)

#if notEmbedded
    #include <iostream>
#else
    #include <Arduino.h>
#endif

typedef double _ponto;
#define ACCURACY_TOLERANCE 0.02 //2%

typedef _ponto (*funType)(_ponto);


struct EMatrix{
    size_t lines;
    size_t columns;
    _ponto **matrix;

    EMatrix();
    EMatrix(size_t lin, size_t col);
    EMatrix(const EMatrix &m);
    ~EMatrix();

    void initilize_values(const _ponto n, bool isRand);
    void print();

    void sum(const EMatrix *m);
    void hadamard_multiplication(const EMatrix *m);
    void scalar_multiplication(const _ponto n);
    void map(const funType f);
};

EMatrix sub(const EMatrix *A, const EMatrix *B);
EMatrix scalar_multiplication(const EMatrix *m,const _ponto n);
EMatrix dot_multiplication(const EMatrix *A, const EMatrix *B);
EMatrix transpose(const EMatrix *L);
EMatrix map(const EMatrix *m, const funType f);


bool is_equal(const EMatrix *A, const EMatrix *B, double tolerance);
EMatrix convert_to_binary(const EMatrix *m);
EMatrix mse_softmax(const EMatrix *m);


namespace ActivationFunction
{
    enum type
    {
        _eSigmoid,
        _eRelu,
        _eTahn,
        _eSin,
        LAST
    };
}
_ponto eSigmoide(_ponto x);
_ponto eDsigmoide(_ponto x);
_ponto eReLU(_ponto x);
_ponto eDReLU(_ponto x);
_ponto eTanh(_ponto x);
_ponto eDTanh(_ponto x);
_ponto eSin(_ponto x);


namespace MetricFunction
{
    enum type
    {
        _eMeanError,
        _eMeanSquaredError,
        _eMeanAbsoluteError,
        _eMeanAbsolutePercentageError,
        _eAccuracy,
        _eBinaryAccuracy,
        _eCategoricalAccuracy,
        _eSum,
        LAST
    };
}
typedef double (*metricFun)(const EMatrix *A, const EMatrix *B);

double eMean_error(const EMatrix *A, const EMatrix *B);
double eMean_squared_error(const EMatrix *A, const EMatrix *B);
double eMean_absolute_error(const EMatrix *A, const EMatrix *B);
double eMean_absolute_percentage_error(const EMatrix *A, const EMatrix *B);
double eAccuracy(const EMatrix *A, const EMatrix *B);
double eBinary_accuracy(const EMatrix *A, const EMatrix *B);
double eSum(const EMatrix *A, const EMatrix *B);


void generic_print(const char* s);
void generic_print_v(const _ponto &v);
    
#endif //MATRIX_LIB_HPP
