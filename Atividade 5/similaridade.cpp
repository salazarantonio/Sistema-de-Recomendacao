#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include "similaridade.h"
#include "lista_compras.h"

void construirMatrizCompras(MatrizSimilaridade *matriz) {
    int i, j;
    int n = vetorClientes.size();
    int m = nomesProdutos.size();

    matriz->numeroClientes = n;
    matriz->numeroProdutos = m;

    matriz-> S = NULL;
    matriz->A = (int**) malloc(n * sizeof(int*));
    for (i = 0; i < n; i++) {
        matriz->A[i] = (int*) malloc(m * sizeof(int));
        for (j = 0; j < m; j++) {
            matriz->A[i][j] = 0;
        }
    }
    for (i = 0; i < n; i++) {
        for (int idProduto : listaCompras[i]) {
            matriz->A[i][idProduto] = 1;
        }
    }
}

int** construirTransposta(int **A, int n, int m) {
    int **At = (int**) malloc(m * sizeof(int*));
    for (int k = 0; k < m; k++) {
        At[k] = (int*) malloc(n * sizeof(int));
        for (int i = 0; i < n; i++) {
            At[k][i] = A[i][k];
        }
    }
    return At;
}

int** multiplicaTransposta(int **A, int n, int m) {
    int **At = construirTransposta(A, n, m);

    int **C = (int**) malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        C[i] = (int*) malloc(n * sizeof(int));
        for (int j = 0; j < n; j++) {
            int soma = 0;
            for (int k = 0; k < m; k++) {
                soma += A[i][k] * At[k][j];
            }
            C[i][j] = soma;
        }
    }

    for (int k = 0; k < m; k++) {free(At[k]);}
    free(At);

    return C;
}

int** multiplicaMatrizes(int **A, int n, int m) { // Atv 4
    int **C = (int**) malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        C[i] = (int*) malloc(n * sizeof(int));
    }

    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            int soma = 0;
            for (int k = 0; k < m; k++) {
                soma += A[i][k] * A[j][k];
            }
            C[i][j] = soma;
            C[j][i] = soma; 
        }
    }
    return C;
}

void calcularMatrizSimilaridade(MatrizSimilaridade *matriz, int **C) {
    int n = matriz->numeroClientes;

    if (matriz->S == NULL) {
        matriz->S = (double**) malloc(n * sizeof(double*));
        for (int i = 0; i < n; i++) {
            matriz->S[i] = (double*) malloc(n * sizeof(double));
        }
    }

    for (int i = 0; i < n; i++) {
        int tamanhoPi = listaCompras[i].size();
        for (int j = 0; j < n; j++) {
            if (i == j) {
                matriz->S[i][j] = 0.0; 
            } else if (tamanhoPi == 0) {
                matriz->S[i][j] = 1.0; 
            } else {
                matriz->S[i][j] = 1.0 - (double)C[i][j] / (double)tamanhoPi;
            }
        }
    }
}
    
void construirMatrizSimilaridade(MatrizSimilaridade *matriz, bool novoAlgoritmo) {
    int n = matriz->numeroClientes;
    int m = matriz->numeroProdutos;

    int **C = novoAlgoritmo ? multiplicaMatrizes(matriz->A, n, m) : multiplicaTransposta(matriz->A, n, m);

    calcularMatrizSimilaridade(matriz, C);

    for (int i = 0; i < n; i++) {free(C[i]);}
    free(C);
}

double obterSimilaridade(MatrizSimilaridade *matriz, int i, int j) {
    return matriz->S[i][j];
}

int clienteMaisSimilar(MatrizSimilaridade *matriz, int i) {
    int n = matriz->numeroClientes;
    int melhorIndice = -1;
    double melhorValor = 2.0;

    for (int j = 0; j < n; j++) {
        if (j == i) continue;
        if (matriz->S[i][j] < melhorValor) {
            melhorValor = matriz->S[i][j];
            melhorIndice = j;
        }
    }

    return melhorIndice;
}

void liberar(MatrizSimilaridade *matriz) {
    if (matriz->A != NULL) {
        for (int i = 0; i < matriz->numeroClientes; i++) {
            free(matriz->A[i]);
        }
        free(matriz->A);
        matriz->A = NULL;
    }
    if (matriz->S != NULL) {
        for (int i = 0; i < matriz->numeroClientes; i++) {
            free(matriz->S[i]);
        }
        free(matriz->S);
        matriz->S = NULL;
    }
}





void construirMatrizComprasCSR(MatrizSimilaridade *matriz, MatrizCSR *A) {
    int n = vetorClientes.size();
    int m = nomesProdutos.size();

    matriz->numeroClientes = n;
    matriz->numeroProdutos = m;
    matriz->A = NULL; 
    matriz->S = NULL;

    A->row_ptr.push_back(0);
    for (int i = 0; i < n; i++) {
        std::vector<int> colunas(listaCompras[i].begin(), listaCompras[i].end());
        sort(colunas.begin(), colunas.end());
        colunas.erase(unique(colunas.begin(), colunas.end()), colunas.end());

        for (int coluna : colunas) {
            A->values.push_back(1);
            A->col_index.push_back(coluna);
        }
        A->row_ptr.push_back((int) A->values.size());
    }
}

MatrizCSR multiplicaCSR(const MatrizCSR &A, int numeroClientes) {
    MatrizCSR C;
    int cont = 0; 
    C.row_ptr.push_back(0);

    for (int i = 0; i < numeroClientes; i++) {
        int inicioI = A.row_ptr[i];
        int fimI = A.row_ptr[i + 1];

        for (int j = 0; j < numeroClientes; j++) {
            int inicioJ = A.row_ptr[j];
            int fimJ = A.row_ptr[j + 1];

            int pi = inicioI, pj = inicioJ;
            int soma = 0;
            while (pi < fimI && pj < fimJ) {
                if (A.col_index[pi] < A.col_index[pj]) {
                    pi++;
                } else if (A.col_index[pi] > A.col_index[pj]) {
                    pj++;
                } else {
                    soma += A.values[pi] * A.values[pj];
                    pi++;
                    pj++;
                }
            }

            if (soma > 0) {
                C.values.push_back(soma);
                C.col_index.push_back(j);
                cont++;
            }
        }
        C.row_ptr.push_back(cont);
    }

    return C;
}

void construirMatrizSimilaridadeCSR(MatrizSimilaridade *matriz, const MatrizCSR &A) {
    int n = matriz->numeroClientes;

    MatrizCSR I = multiplicaCSR(A, n);

    if (matriz->S == NULL) {
        matriz->S = (double**) malloc(n * sizeof(double*));
        for (int i = 0; i < n; i++) {
            matriz->S[i] = (double*) malloc(n * sizeof(double));
        }
    }

    for (int i = 0; i < n; i++) {
        int tamanhoPi = listaCompras[i].size();

        for (int j = 0; j < n; j++) {
            matriz->S[i][j] = 1.0;
        }
        matriz->S[i][i] = 0.0; 

        if (tamanhoPi > 0) {
            for (int p = I.row_ptr[i]; p < I.row_ptr[i + 1]; p++) {
                int j = I.col_index[p];
                if (j == i) continue;
                matriz->S[i][j] = 1.0 - (double) I.values[p] / (double) tamanhoPi;
            }
        }
    }
}