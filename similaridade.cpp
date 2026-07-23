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

    // Aloca a matriz densa n x m inicializada com 0
    matriz->A = (int**) malloc(n * sizeof(int*));
    for (i = 0; i < n; i++) {
        matriz->A[i] = (int*) malloc(m * sizeof(int));
        for (j = 0; j < m; j++) {
            matriz->A[i][j] = 0;
        }
    }


    // Para cada cliente, marca com 1 as posicoes dos produtos que ele comprou
    for (i = 0; i < n; i++) {
        for (int idProduto : listaCompras[i]) {
            matriz->A[i][idProduto] = 1;
        }
    }
}

void construirMatrizSimilaridade(MatrizSimilaridade *matriz) {
    int i, j, k;
    int n = matriz->numeroClientes;
    int m = matriz->numeroProdutos;

    int **I = (int**) malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        I[i] = (int*) malloc(n * sizeof(int));
    }

    // Matriz de intersecao I = A x A^T (Algoritmo de Multiplicacao de Matrizes Padrao)
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            int soma = 0;
            for (k = 0; k < m; k++) {
                // A^T[k][j] = A[j][k], entao I[i][j] = soma_k A[i][k] * A[j][k]
                soma += matriz->A[i][k] * matriz->A[j][k];
            }
            I[i][j] = soma;
        }
    }

    // Constroi a matriz de similaridade final: S[i][j] = 1 - I[i][j] / |P_i|
    matriz->S = (double**) malloc(n * sizeof(double*));
    for (i = 0; i < n; i++) {
        matriz->S[i] = (double*) malloc(n * sizeof(double));
    }

    for (int i = 0; i < n; i++) {
        int tamanhoPi = listaCompras[i].size();
        for (int j = 0; j < n; j++) {
            int tamanhoPj = listaCompras[j].size();
            int uniao = tamanhoPi + tamanhoPj - I[i][j]; // |A U B| = |A| + |B| - |A inter B|
            if (i == j) {
                matriz->S[i][j] = 0.0; // cliente e identico a ele mesmo
            } else if (uniao == 0) {
                // Cliente sem compras: nao ha base de comparacao, considera-se
                // totalmente dissimilar para nao gerar divisao por zero.
                matriz->S[i][j] = 1.0;
            } else {
                matriz->S[i][j] = 1.0 - (double)I[i][j] / (double)uniao;
            }
        }
    }

    for (int i = 0; i < n; i++) free(I[i]);
    free(I);
}

double obterSimilaridade(MatrizSimilaridade *matriz, int i, int j) {
    return matriz->S[i][j];
}

int clienteMaisSimilar(MatrizSimilaridade *matriz, int i) {
    int n = matriz->numeroClientes;
    int melhorIndice = -1;
    double melhorValor = 2.0; // maior que qualquer valor possivel de s(i,j), que fica em [0,1]

    for (int j = 0; j < n; j++) {
        if (j == i) continue;
        // Quanto MENOR o valor de s(i,j), mais similar e o cliente j (distancia de Jaccard)
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