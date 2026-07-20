#ifndef RECOMENDACAO_H
#define RECOMENDACAO_H

/*
 * Modulo: Recomendacao  (Atividade 3)
 * -------------------------------------
 * Implementa o algoritmo de ranqueamento de produtos descrito na
 * Secao 2.3 do documento do projeto:
 *
 *  1. Identificacao de Vizinhos: clientes com similaridade < 1 em
 *     relacao ao cliente c (excluindo o proprio c);
 *  2. Inicializacao do vetor de ranqueamento R, com 1 para cada produto;
 *  3. Para cada vizinho s e cada produto p comprado por s e nao
 *     comprado por c: R[p] = R[p] * S(c,s);
 *  4. Ordenacao crescente de R e selecao dos k primeiros produtos.
 */

#include <vector>
#include "lista_compras.h"
#include "similaridade.h"

// Um item do vetor de ranqueamento: um produto e seu valor de ranking.
struct ItemRanking {
    int indiceProduto;
    double valorRanking;
};

/*
 * Gera o vetor de ranqueamento completo (Passos 1 a 3 do algoritmo)
 * para o cliente de indice "indiceCliente". O vetor retornado contem
 * apenas os produtos que o cliente AINDA NAO comprou (candidatos a
 * recomendacao), ja ordenado de forma crescente pelo valor de ranking
 * (Passo 4), do melhor (menor valor) para o pior.
 */
std::vector<ItemRanking> gerarRanking(const ListaCompras &lista,
                                       const Similaridade &sim,
                                       int indiceCliente);

/*
 * Retorna os indices internos dos "k" produtos melhor ranqueados
 * (mais recomendados) para o cliente de indice "indiceCliente".
 * Se houver menos de k produtos candidatos, retorna todos os
 * disponiveis.
 */
std::vector<int> recomendarProdutos(const ListaCompras &lista,
                                     const Similaridade &sim,
                                     int indiceCliente,
                                     int k);

#endif /* RECOMENDACAO_H */
