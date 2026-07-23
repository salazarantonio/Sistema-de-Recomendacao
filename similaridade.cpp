#include <vector>
#include <iostream>
#include "similaridade.h"
#include "lista_compras.h"
using namespace std;

MatrizSimilaridade matrizGlobal;

void construirMatrizCompras() {
    int n = vetorClientes.size();
    int m = nomesProdutos.size();

    matrizGlobal.numeroClientes = n;
    matrizGlobal.numeroProdutos = m;

    // Aloca a matriz densa n x m inicializada com 0
    matrizGlobal.A.assign(n, vector<int>(m, 0));

    // Para cada cliente, marca com 1 as posicoes dos produtos que ele comprou
    for (int i = 0; i < n; i++) {
        for (int idProduto : listaCompras[i]) {
            matrizGlobal.A[i][idProduto] = 1;
        }
    }
}

void construirMatrizSimilaridade() {
    int n = matrizGlobal.numeroClientes;
    int m = matrizGlobal.numeroProdutos;

    // Matriz de intersecao I = A x A^T (Algoritmo de Multiplicacao de Matrizes Padrao)
    vector<vector<int>> I(n, vector<int>(n, 0));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int soma = 0;
            for (int k = 0; k < m; k++) {
                // A^T[k][j] = A[j][k], entao I[i][j] = soma_k A[i][k] * A[j][k]
                soma += matrizGlobal.A[i][k] * matrizGlobal.A[j][k];
            }
            I[i][j] = soma;
        }
    }

    // Constroi a matriz de similaridade final: S[i][j] = 1 - I[i][j] / |P_i|
    matrizGlobal.S.assign(n, vector<double>(n, 0.0));

    for (int i = 0; i < n; i++) {
        int tamanhoPi = listaCompras[i].size(); // |P_i|
        for (int j = 0; j < n; j++) {
            if (i == j) {
                matrizGlobal.S[i][j] = 0.0; // cliente e identico a ele mesmo
            } else if (tamanhoPi == 0) {
                // Cliente sem compras: nao ha base de comparacao, considera-se
                // totalmente dissimilar para nao gerar divisao por zero.
                matrizGlobal.S[i][j] = 1.0;
            } else {
                matrizGlobal.S[i][j] = 1.0 - (double)I[i][j] / (double)tamanhoPi;
            }
        }
    }
}

double obterSimilaridade(int i, int j) {
    return matrizGlobal.S[i][j];
}

int clienteMaisSimilar(int i) {
    int n = matrizGlobal.numeroClientes;
    int melhorIndice = -1;
    double melhorValor = 2.0; // maior que qualquer valor possivel de s(i,j), que fica em [0,1]

    for (int j = 0; j < n; j++) {
        if (j == i) continue;
        // Quanto MENOR o valor de s(i,j), mais similar e o cliente j (distancia de Jaccard)
        if (matrizGlobal.S[i][j] < melhorValor) {
            melhorValor = matrizGlobal.S[i][j];
            melhorIndice = j;
        }
    }

    return melhorIndice;
}