#ifndef SIMILARIDADE_H
#define SIMILARIDADE_H

#include <vector>

// Struct que agrupa toda a informação da matriz de compras/similaridade.
// Mantendo tudo dentro de uma única struct facilita passar/expor o "estado"
// do módulo Similaridade sem espalhar variáveis soltas.
typedef struct {
    int nClientes;                              // n: numero de clientes
    int nProdutos;                              // m: numero de produtos
    std::vector<std::vector<int>> A;            // matriz de compras densa (n x m)
    std::vector<std::vector<double>> S;         // matriz de similaridade (n x n)
} MatrizSimilaridade;

// Estado global do módulo (unico, assim como as estruturas de ListaCompras)
extern MatrizSimilaridade matrizGlobal;

// Constroi a matriz de compras densa A (n x m) a partir das estruturas
// preenchidas pelo módulo ListaCompras (v_CodigosClientes, v_NomesProdutos, compras).
void construirMatrizCompras();

// Constroi a matriz de similaridade S (n x n) a partir da matriz de compras A,
// calculando I = A x A^T e depois S[i][j] = 1 - I[i][j] / |P_i| (distancia de Jaccard).
void construirMatrizSimilaridade();

// Retorna o valor de similaridade (distancia de Jaccard) entre os clientes de
// indices internos i e j.
double obterSimilaridade(int i, int j);

// Dado um cliente de indice interno i, retorna o indice interno do cliente
// mais similar a ele (excluindo ele mesmo). Retorna -1 se nao houver outro cliente.
int clienteMaisSimilar(int i);

#endif
