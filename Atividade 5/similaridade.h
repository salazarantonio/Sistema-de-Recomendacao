#ifndef SIMILARIDADE_H
#define SIMILARIDADE_H
#include <vector>

typedef struct {
    int numeroClientes;
    int numeroProdutos;                              
    int **A;
    double **S;            
} MatrizSimilaridade;
typedef struct {
    std::vector<int> values;       
    std::vector<int> col_index;    
    std::vector<int> row_ptr;      
} MatrizCSR;

void construirMatrizCompras(MatrizSimilaridade *matriz);

void construirMatrizSimilaridade(MatrizSimilaridade *matriz, bool novoAlgoritmo = true);

double obterSimilaridade(MatrizSimilaridade *matriz, int i, int j);
int clienteMaisSimilar(MatrizSimilaridade *matriz, int i);

void liberar(MatrizSimilaridade *matriz);

// ------- atividade 5: matrizes esparsas (CSR) --------------
void construirMatrizComprasCSR(MatrizSimilaridade *matriz, MatrizCSR *A);


MatrizCSR multiplicaCSR(const MatrizCSR &A, int numeroClientes);
void construirMatrizSimilaridadeCSR(MatrizSimilaridade *matriz, const MatrizCSR &A);

#endif