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

int** construirTransposta(int **A, int n, int m) {
    // AT tem dimensao m x n
    int **At = (int**) malloc(m * sizeof(int*));
    for (int k = 0; k < m; k++) {
        At[k] = (int*) malloc(n * sizeof(int));
        for (int i = 0; i < n; i++) {
            At[k][i] = A[i][k];
        }
    }
    return At;
}

// Matriz de intersecao I = A x A^T (Algoritmo de Multiplicacao de Matrizes Padrao)
int** multiplicaMatrizes(int **A, int n, int m) {
    int **At = construirTransposta(A, n, m);

    int **C = (int**) malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        C[i] = (int*) malloc(n * sizeof(int));
        for (int j = 0; j < n; j++) {
            int soma = 0;
            for (int k = 0; k < m; k++) {
                // A^T[k][j] = A[j][k], entao I[i][j] = soma_k A[i][k] * A[j][k]
                soma += A[i][k] * At[k][j];
            }
            C[i][j] = soma;
        }
    }

    for (int k = 0; k < m; k++) {free(At[k]);}
    free(At);

    return C;
}

int** multiplica_por_transposta(int **A, int n, int m) { // Atv 4
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
            C[j][i] = soma; // espelhamento por conta da simetria i
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
                matriz->S[i][j] = 0.0; // cliente e' identico a ele mesmo
            } else if (tamanhoPi == 0) {
                matriz->S[i][j] = 1.0; // Cliente sem compras: nao ha base de comparacao, considera-se totalmente dissimilar para nao gerar divisao por zero.
            } else {
                matriz->S[i][j] = 1.0 - (double)C[i][j] / (double)tamanhoPi;
            }
        }
    }
}
    
void construirMatrizSimilaridade(MatrizSimilaridade *matriz, bool novoAlgoritmo) {
    int n = matriz->numeroClientes;
    int m = matriz->numeroProdutos;

    int **C = novoAlgoritmo ? multiplica_por_transposta(matriz->A, n, m) : multiplicaMatrizes(matriz->A, n, m);

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

// ---------------------- Atividade 5: Matrizes Esparsas (CSR) ----------------------

void construirMatrizComprasCSR(MatrizSimilaridade *matriz, MatrizCSR *A) {
    int n = vetorClientes.size();
    int m = nomesProdutos.size();

    matriz->numeroClientes = n;
    matriz->numeroProdutos = m;
    matriz->A = NULL; // nao ha matriz densa nesta abordagem
    matriz->S = NULL;

    A->row_ptr.push_back(0);
    for (int i = 0; i < n; i++) {
        // As colunas (produtos) precisam estar ordenadas para o algoritmo de
        // intersecao (multiplicaCSR) funcionar. Tambem removemos duplicatas,
        // ja que a matriz de compras so guarda se o produto foi comprado (0/1).
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
    int cont = 0; // contador de elementos nao nulos inseridos em C
    C.row_ptr.push_back(0);

    for (int i = 0; i < numeroClientes; i++) {
        int inicioI = A.row_ptr[i];
        int fimI = A.row_ptr[i + 1];

        for (int j = 0; j < numeroClientes; j++) {
            int inicioJ = A.row_ptr[j];
            int fimJ = A.row_ptr[j + 1];

            // Calcula o produto interno entre a linha i de A e a linha j de A
            // (simulando a coluna j de A^T), avancando os indices de coluna
            // de forma similar a uma intersecao de conjuntos ordenados.
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

        // Inicializa a linha inteira. Colunas ausentes em I tem intersecao 0,
        // ou seja, a maior dissimilaridade possivel (S = 1), tanto no caso
        // tamanhoPi == 0 (sem base de comparacao) quanto no caso geral.
        for (int j = 0; j < n; j++) {
            matriz->S[i][j] = 1.0;
        }
        matriz->S[i][i] = 0.0; // cliente e' identico a ele mesmo

        // Sobrescreve com o valor correto apenas as colunas que possuem
        // intersecao nao nula, armazenadas em I no formato CSR.
        if (tamanhoPi > 0) {
            for (int p = I.row_ptr[i]; p < I.row_ptr[i + 1]; p++) {
                int j = I.col_index[p];
                if (j == i) continue;
                matriz->S[i][j] = 1.0 - (double) I.values[p] / (double) tamanhoPi;
            }
        }
    }
}