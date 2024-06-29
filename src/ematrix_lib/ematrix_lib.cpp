//
// Created by Edumarek on 26/06/2021.
//
#include "ematrix_lib.hpp"

#include <math.h>

//------------------CONSTRUTORES&DESTRUTORES-------------------//
EMatrix::EMatrix()
{
    lines = 0;
    columns = 0;
    matrix = NULL;
}

EMatrix::EMatrix(size_t lin, size_t col)
{
    lines = lin;
    columns = col;

    matrix = (_ponto**)malloc(lines * sizeof (_ponto*));

    for (size_t i = 0; i < lines; i++)
    {
        matrix[i] = (_ponto *) malloc(columns * sizeof(_ponto));
    }
    initilize_values(1, true);
}

EMatrix::EMatrix(const EMatrix &m)
{
    lines = m.lines;
    columns = m.columns;
    matrix = (_ponto**)malloc(lines * sizeof (_ponto*));

    for (size_t i = 0; i < lines; i++)
    {
        matrix[i] = (_ponto *) malloc(columns * sizeof(_ponto));
    }
    
    for(size_t i = 0 ; i < lines; i++)
    {
        for(size_t j = 0; j < columns; j++)
        {
            matrix[i][j] = m.matrix[i][j];
        }
    }

}

EMatrix::~EMatrix()
{
    for(size_t i = 0; i < lines; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
}

//---------------------------METODOS---------------------------//

void EMatrix::initilize_values(const _ponto n, bool isRand)
{
    if(isRand)
    {
        for(size_t i = 0 ; i < lines ; i++)
        {
            for(size_t j = 0; j < columns; j++)
            {
                do
                {
                    matrix[i][j] = ((_ponto)(rand()%21-10)/10);
                }while(matrix[i][j] == 0);
            }
        }
    }
    else
    {
        for(size_t i = 0; i < lines; i++)
        {
            for(size_t j = 0; j < columns; j++)
            {       
                matrix[i][j] = n;
            }
        }
    }
}

#if defined __linux__
void EMatrix::print()
{
    for(size_t i = 0; i < lines; i++)
    {
        for(size_t j = 0; j < columns; j++)
        {
            std::cout<<matrix[i][j]<< " ";
        }
        std::cout<<std::endl;
    }
}
#else
void EMatrix::print()
{
    for(size_t i = 0; i < lines; i++)
    {
        for(size_t j = 0; j < columns; j++)
        {
            Serial.print(String(matrix[i][j]) + " ");
        }
        Serial.println("");
    }
}
#endif

void EMatrix::sum(const EMatrix *m)
{
    for(size_t i = 0; i < lines; i++)
    {
        for(size_t j = 0; j < columns; j++)
        {
            matrix[i][j] += m->matrix[i][j];
        }
    }
}

void EMatrix::hadamard_multiplication(const EMatrix *m)
{
    for(size_t i = 0; i < lines; i++)
    {
        for(size_t j = 0; j<columns; j++)
        {
            matrix[i][j] *= m->matrix[i][j];
        }
    }
}

void EMatrix::scalar_multiplication(const _ponto n)
{
    for(size_t i = 0; i < lines; i++)
    {
        for(size_t j = 0; j < columns; j++)
        {
            matrix[i][j] *= n;
        }
    }
}

void EMatrix::map(const funType f)
{
    _ponto value;

    for(size_t i = 0; i < lines; i++)
    {
        for(size_t j = 0; j < columns; j++)
        {
            value = matrix[i][j];
            matrix[i][j] = f(value);
        }

    }
}

bool EMatrix::verificar_igualdade(const EMatrix *m)
{
    for(size_t i = 0; i < lines; i++)
    {
        for(size_t j = 0; j < columns; j++)
        {
            if(matrix[i][j] != m->matrix[i][j])
            {
              return false;
            }
        }
    }

    return true;
}

void EMatrix::softmax()
{
    _ponto sum = 0;

    if(lines == 1){
        for(size_t i = 0; i < columns; i++)
        {
            sum += exp(matrix[0][i]);
        }
        
        for(size_t i = 0; i < columns; i++)
        {
            matrix[0][i] = exp(matrix[0][i]) / sum;
        }
    }

    else if(columns == 1)
    {
        for(size_t i = 0; i < lines; i++)
        {
            sum += exp(matrix[i][0]);
        }

        for(size_t i = 0; i < lines; i++)
        {
            matrix[i][0] = exp(matrix[i][0]) / sum;
        }
    }

    else
    {
        //erro
        return;
    }
}

void EMatrix::binary()
{   
    for(size_t i = 0; i < lines; i++)
    {
        for(size_t j = 0; j < columns; j++)
        {
            if(matrix[i][j]>0.5)
            {
                matrix[i][j] = 1;
            }
            else
            {
                matrix[i][j] = 0;
            }
        }

    }
}

//---------------------------FUNCOES---------------------------//

EMatrix sub(const EMatrix *A, const EMatrix *B)
{
    EMatrix result(A->lines, A->columns);

    for(size_t i = 0; i < A->lines; i++)
    {
        for(size_t j = 0; j < A->columns; j++)
        {
            result.matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
        }
    }

    return result;
}

EMatrix scalar_multiplication(const EMatrix *m, const _ponto n)
{
    EMatrix result(m->lines, m->columns);

    for(size_t i = 0; i < m->lines; i++)
    {
        for(size_t j = 0; j < m->columns; j++)
        {
            result.matrix[i][j] = m->matrix[i][j] * n;
        }
    }

    return result;
}

EMatrix dot_multiplication(const EMatrix *A, const EMatrix *B)
{
    EMatrix result(A->lines, B->columns);
    _ponto value;

    for(size_t i = 0; i < A->lines; i++) 
    {
        for (size_t j = 0; j < B->columns; j++) 
        {
            value = 0;
        
            for(size_t k = 0; k < A->columns; k++)
            {
                value += (A->matrix[i][k] * B->matrix[k][j]);
            }        

            result.matrix[i][j] = value;
        }
    }

    return result;
}

EMatrix transpose(const EMatrix *L)
{
    EMatrix result(L->columns, L->lines);

    for(size_t i = 0; i < L->columns; i++)
    {
        for(size_t j = 0; j < L->lines; j++)
        {
            result.matrix[i][j] = L->matrix[j][i];
        }
    }

    return result;
}

EMatrix map(const EMatrix *m , const funType f)
{
    EMatrix result(m->lines, m->columns);

    _ponto value;
    for(size_t i = 0; i < result.lines; i++)
    {
        for(size_t j = 0; j < result.columns; j++)
        {
            value = m->matrix[i][j];
            result.matrix[i][j] = f(value);
        }
    }

    return result;
}

bool verificar_igualdade(const EMatrix *A, const EMatrix *B)
{
    for(size_t i = 0; i < A->lines; i++)
    {
        for(size_t j = 0; j < A->columns; j++)
        {
            if(A->matrix[i][j] != B->matrix[i][j])
            {
                return false;
            }
        }
    }

    return true;
}

EMatrix softmax(const EMatrix *m)
{
    EMatrix result(m->lines, m->columns);
    
    _ponto sum = 0;
    if(m->lines == 1)
    {
        for(size_t i = 0; i < m->columns; i++)
        {
            sum += exp(m->matrix[0][i]);
        }

        for(size_t i = 0; i < m->columns; i++)
        {
            result.matrix[0][i] = exp(m->matrix[0][i]) / sum;
        }
    }
    
    else if(m->columns ==1 )
    {
        for(size_t i = 0; i < m->lines; i++)
        {
            sum += exp(m->matrix[i][0]);
        }

        for(size_t i = 0; i < m->lines; i++)
        {
            result.matrix[i][0] = exp(m->matrix[i][0]) / sum;
        }
    }
    else
    {
        //erro
    }

    return result;
}

EMatrix binary(const EMatrix *m)
{   
    EMatrix result(m->lines, m->columns);

    for(size_t i = 0; i < m->lines; i++)
    {
        for(size_t j = 0; j < m->columns; j++)
        {
            if(m->matrix[i][j] > 0.5)
            {
                result.matrix[i][j] = 1;
            }
            else
            {
                result.matrix[i][j] = 0;
            }
        }

    }

    return result;
}

_ponto loss(const EMatrix *A, const EMatrix *B)
{
    _ponto result=0;

    EMatrix L(sub(A, B));

    for(size_t i = 0; i < L.lines; i++)
    {
        for(size_t j = 0; j < L.columns; j++)
        {
            result += L.matrix[i][j];
        }
    }
    result /= (L.lines * L.columns);

    return result * result;
}

//----------------------FUNCOES DE ATIVACAO--------------------//
_ponto eSigmoide(_ponto x)
{
    return 1 / (1 + exp(-x));
}

_ponto eDsigmoide(_ponto x)
{
    return x * (1 - x);
}

_ponto eReLU(_ponto x)
{
    if(x > 0)
    {
        return x;
    }

    return 0;
}

_ponto eDReLU(_ponto x)
{
    return (_ponto)(x > 0);
}

_ponto eTanh(_ponto x)
{
    return tanh(x);
}

_ponto eDTanh(_ponto x)
{
    return 1 - (tanh(x) * tanh(x));
}

_ponto eSin(_ponto x)
{
    return sin(x);
}
//----------------------FUNCOES AUXILIARES---------------------//





