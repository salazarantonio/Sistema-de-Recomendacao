#ifndef SIMILARIDADE_H
#define SIMILARIDADE_H

/*
 * Modulo: Similaridade  (Atividade 2 - versao inicial)
 * -----------------------------------------------------
 * Calcula a matriz de similaridade entre clientes usando a distancia de
 * Jaccard descrita na Secao 2.2 do projeto:
 *
 *      S[i][j] = 1 - |P_i inter P_j| / |P_i|
 *
 * Nesta primeira versao (Atividade 2), a matriz de intersecao e obtida
 * transformando a lista de compras em uma matriz de compras DENSA
 * (A, n x m) e multiplicando-a pela sua transposta (I = A x A^T),
 * seguindo o algoritmo padrao de multiplicacao de matrizes.
 *
 * Toda a identificacao de clientes/produtos aqui e feita pelos indices
 * internos (nao pelos codigos originais da base de compras).
 */

#include <vector>
#include "lista_compras.h"

struct Similaridade {
    int numClientes;                              // n
    std::vector<std::vector<double> > matrizS;    // matriz S, n x n (assimetrica)
};

/*
 * Constroi a matriz de similaridade "sim" a partir dos dados ja
 * carregados em "lista" (modulo ListaCompras).
 */
void construirSimilaridade(const ListaCompras &lista, Similaridade &sim);

/*
 * Retorna o valor S[i][j] da matriz de similaridade.
 */
double obterSimilaridade(const Similaridade &sim, int i, int j);

/*
 * Encontra o cliente mais similar ao cliente de indice "indiceCliente",
 * excluindo o proprio cliente. "Mais similar" = menor valor de S[i][j]
 * (S e uma distancia: quanto menor, mais parecido). Clientes sem
 * nenhum produto em comum (S == 1) nao sao considerados vizinhos.
 *
 * Retorna o indice do cliente mais similar, ou -1 caso nao exista
 * nenhum cliente com pelo menos um produto em comum.
 */
int clienteMaisSimilar(const Similaridade &sim, int indiceCliente);

#endif /* SIMILARIDADE_H */
