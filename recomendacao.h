#ifndef RECOMENDACAO_H
#define RECOMENDACAO_H

#include <vector>

// Struct que representa uma entrada do vetor de ranqueamento de um cliente:
// o identificador interno do produto e o valor de ranqueamento calculado.
typedef struct {
    int idProduto;
    double ranking;
} ProdutoRank;

// Gera os k produtos mais bem ranqueados (recomendados) para o cliente de
// indice interno idCliente, seguindo o algoritmo descrito na Secao 2.3:
//   1) Identificacao de vizinhos (similaridade < 1, exceto o proprio cliente);
//   2) Inicializacao do vetor de ranqueamento com valor 1 para cada produto;
//   3) Atualizacao do ranqueamento multiplicando pela similaridade do vizinho;
//   4) Ordenacao nao-decrescente e selecao dos k primeiros.
// Produtos ja comprados pelo cliente sao excluidos do resultado.
std::vector<ProdutoRank> recomendarProdutos(int idCliente, int k);

#endif