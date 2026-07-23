#include <vector>
#include <algorithm>
#include "recomendacao.h"
#include "lista_compras.h"
#include "similaridade.h"
using namespace std;

vector<ProdutoRank> recomendarProdutos(int idCliente, int k) {
    int n = matrizGlobal.numeroClientes;
    int m = matrizGlobal.numeroProdutos;

    // Marca quais produtos ja foram comprados pelo cliente c
    vector<bool> compradoPorC(m, false);
    for (int idProduto : listaCompras[idCliente]) {
        compradoPorC[idProduto] = true;
    }

    // Passo 2: Inicializacao do vetor de ranqueamento com valor 1 para cada produto
    vector<double> R(m, 1.0);

    // Passo 1 e 3: Identificacao de vizinhos e calculo do ranqueamento
    for (int s = 0; s < n; s++) {
        if (s == idCliente) continue;

        double similaridade = obterSimilaridade(idCliente, s);
        if (similaridade >= 1.0) continue; // vizinho totalmente dissimilar, ignora

        for (int p : listaCompras[s]) {
            if (!compradoPorC[p]) {
                R[p] *= similaridade;
            }
        }
    }

    // Monta o vetor de ranqueamento apenas com produtos que o cliente ainda nao comprou
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

    // Passo 4: Ordenacao nao-decrescente (menor valor = produto mais recomendado)
    sort(ranqueamento.begin(), ranqueamento.end(),
         [](const ProdutoRank &a, const ProdutoRank &b) {
             return a.ranking < b.ranking;
         });

    // Seleciona os k primeiros (ou menos, se nao houver k produtos disponiveis)
    if ((int)ranqueamento.size() > k) {
        ranqueamento.resize(k);
    }

    return ranqueamento;
}