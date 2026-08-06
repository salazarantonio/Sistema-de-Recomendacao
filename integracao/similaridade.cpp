#include <stdlib.h>
#include <iostream>
#include "similaridade.h"
#include "lista_compras.h"

void construirMatrizCompras(MatrizSimilaridade *matriz) {
    int i, j;
    int n = vetorClientes.size();
    int m = nomesProdutos.size();

    matriz->numeroClientes = n;
    matriz->numeroProdutos = m;

    matriz->S = NULL;
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

// Matriz de intersecao (padrao)
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

int** multiplicaMatrizes(int **A, int n, int m) { // 4
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
    int m = matriz->numeroProdutos;

    if (matriz->S == NULL) {
        matriz->S = (double**) malloc(n * sizeof(double*));
        for (int i = 0; i < n; i++) {
            matriz->S[i] = (double*) malloc(n * sizeof(double));
        }
    }

    for (int i = 0; i < n; i++) {
        // |P_i| calculado somando a propria linha de A, nao mais lendo
        // listaCompras[i].size() -- essa dependencia do modulo ListaCompras
        // quebrava no caminho da integracao Python (Atividade 6), onde quem
        // preenche a matriz e' bindings.cpp, nao lerArquivo().
        int tamanhoPi = 0;
        for (int k = 0; k < m; k++) {
            tamanhoPi += matriz->A[i][k];
        }
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
    for (int i = 0; i < matriz->numeroClientes; i++) {
        free(matriz->A[i]);
        free(matriz->S[i]);
    }
    free(matriz->A);
    free(matriz->S);
}