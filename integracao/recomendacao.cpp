#include <stdlib.h>
#include <vector>
#include <algorithm>
#include "recomendacao.h"
#include "similaridade.h"
using namespace std;

// Funcao de comparacao usada pelo sort (nao pode ser lambda): retorna true se
// 'a' deve vir antes de 'b', ou seja, se 'a' tem o menor ranking (mais recomendado).
bool comparacao(const ProdutoRank &a, const ProdutoRank &b);

vector<ProdutoRank> recomendarProdutos(MatrizSimilaridade *matriz, int idCliente, int k) {
    int n = matriz->numeroClientes;
    int m = matriz->numeroProdutos;

    // Marca quais produtos ja foram comprados pelo cliente idCliente, lendo
    // direto da matriz de compras (matriz->A) -- nao mais de listaCompras,
    // que fica vazia no caminho da integracao Python (Atividade 6).
    bool *compradoPorC = (bool*) malloc(m * sizeof(bool));
    for (int p = 0; p < m; p++) {
        compradoPorC[p] = (matriz->A[idCliente][p] == 1);
    }

    // Passo 2: Inicializacao do vetor de ranqueamento com valor 1 para cada produto
    double *R = (double*) malloc(m * sizeof(double));
    for (int p = 0; p < m; p++) {
        R[p] = 1.0;
    }

    // Passo 1 e 3: Identificacao de vizinhos e calculo do ranqueamento
    for (int s = 0; s < n; s++) {
        if (s == idCliente) continue;
        double similaridade = obterSimilaridade(matriz, idCliente, s);
        if (similaridade >= 1.0) continue;

        for (int p = 0; p < m; p++) {
            if (matriz->A[s][p] == 1 && !compradoPorC[p]) {
                R[p] *= similaridade;
            }
        }
    }

    vector<ProdutoRank> ranqueamento;
    ranqueamento.reserve(m);
    for (int p = 0; p < m; p++) {
        if (!compradoPorC[p]) {
            ProdutoRank pr;
            pr.idProduto = p;
            pr.ranking = R[p];
            ranqueamento.push_back(pr);
        }
    }

    free(compradoPorC);
    free(R);

    sort(ranqueamento.begin(), ranqueamento.end(), comparacao);

    if ((int)ranqueamento.size() > k) {
        ranqueamento.resize(k);
    }

    return ranqueamento;
}

bool comparacao(const ProdutoRank &a, const ProdutoRank &b) {
    return a.ranking < b.ranking;
}