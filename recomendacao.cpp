#include "recomendacao.h"

#include <algorithm>

/*
 * -----------------------------------------------------------------------
 * Funcoes auxiliares (uso interno deste arquivo apenas)
 * -----------------------------------------------------------------------
 */

// Passo 1 do algoritmo: lista de indices de clientes vizinhos de "c"
// (similaridade estritamente menor que 1, excluindo o proprio c).
static std::vector<int> identificarVizinhos(const Similaridade &sim, int c) {
    std::vector<int> vizinhos;
    for (int s = 0; s < sim.numClientes; s++) {
        if (s == c) {
            continue;
        }
        if (sim.matrizS[c][s] < 1.0) {
            vizinhos.push_back(s);
        }
    }
    return vizinhos;
}

// Funcao de comparacao usada pelo std::sort para ordenar o vetor de
// ranqueamento de forma NAO DECRESCENTE pelo valor de ranking (Passo 4).
static bool comparaPorRanking(const ItemRanking &a, const ItemRanking &b) {
    return a.valorRanking < b.valorRanking;
}

/*
 * -----------------------------------------------------------------------
 * Funcoes publicas do modulo (declaradas em recomendacao.h)
 * -----------------------------------------------------------------------
 */

std::vector<ItemRanking> gerarRanking(const ListaCompras &lista,
                                       const Similaridade &sim,
                                       int indiceCliente) {
    int numProdutos = (int)lista.produtos.size();

    // Passo 2: vetor de ranqueamento R, valor inicial 1 para cada produto.
    std::vector<double> R(numProdutos, 1.0);

    // Marca quais produtos o cliente "c" ja comprou, para poder
    // consultar em tempo O(1) durante o Passo 3.
    std::vector<bool> jaComprou(numProdutos, false);
    const std::vector<int> &comprasCliente = lista.compras[indiceCliente];
    for (size_t i = 0; i < comprasCliente.size(); i++) {
        jaComprou[comprasCliente[i]] = true;
    }

    // Passo 1: identifica os vizinhos de "c".
    std::vector<int> vizinhos = identificarVizinhos(sim, indiceCliente);

    // Passo 3: para cada vizinho s e cada produto p comprado por s e
    // nao comprado por c, atualiza R[p] = R[p] * S(c,s).
    for (size_t v = 0; v < vizinhos.size(); v++) {
        int s = vizinhos[v];
        double similaridadeCS = sim.matrizS[indiceCliente][s];

        const std::vector<int> &comprasVizinho = lista.compras[s];
        for (size_t k = 0; k < comprasVizinho.size(); k++) {
            int p = comprasVizinho[k];
            if (!jaComprou[p]) {
                R[p] *= similaridadeCS;
            }
        }
    }

    // Monta o vetor de ItemRanking apenas com os produtos que o
    // cliente ainda nao comprou (candidatos a recomendacao).
    std::vector<ItemRanking> ranking;
    for (int p = 0; p < numProdutos; p++) {
        if (!jaComprou[p]) {
            ItemRanking item;
            item.indiceProduto = p;
            item.valorRanking = R[p];
            ranking.push_back(item);
        }
    }

    // Passo 4: ordena de forma nao decrescente pelo valor de ranking.
    std::sort(ranking.begin(), ranking.end(), comparaPorRanking);

    return ranking;
}

std::vector<int> recomendarProdutos(const ListaCompras &lista,
                                     const Similaridade &sim,
                                     int indiceCliente,
                                     int k) {
    std::vector<ItemRanking> ranking = gerarRanking(lista, sim, indiceCliente);

    int quantidade = std::min((int)ranking.size(), k);
    std::vector<int> produtosRecomendados;
    for (int i = 0; i < quantidade; i++) {
        produtosRecomendados.push_back(ranking[i].indiceProduto);
    }
    return produtosRecomendados;
}
