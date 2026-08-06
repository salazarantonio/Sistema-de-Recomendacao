#ifndef SIMILARIDADE_H
#define SIMILARIDADE_H

#include <vector>

// Struct que agrupa toda a informação da matriz de compras/similaridade.
// Mantendo tudo dentro de uma única struct facilita passar/expor o "estado"
// do módulo Similaridade sem espalhar variáveis soltas.
typedef struct {
    int numeroClientes;                              // n: numero de clientes
    int numeroProdutos;                              // m: numero de produtos
    int **A;            // matriz de compras densa (n x m)
    double **S;            // matriz de similaridade (n x n)
} MatrizSimilaridade;

// Estrutura CSR (Compressed Sparse Row): guarda apenas os elementos nao nulos
// de uma matriz esparsa, junto com suas colunas e o inicio de cada linha.
typedef struct {
    std::vector<int> values;       // valores nao nulos da matriz, linha por linha
    std::vector<int> col_index;    // indice da coluna correspondente a cada valor em values
    std::vector<int> row_ptr;      // indice de inicio de cada linha em values (tamanho n_linhas + 1)
} MatrizCSR;

// Constroi a matriz de compras densa A (n x m) a partir das estruturas
// preenchidas pelo módulo ListaCompras (v_CodigosClientes, v_NomesProdutos, compras).
void construirMatrizCompras(MatrizSimilaridade *matriz);

// Constroi a matriz de similaridade S (n x n) a partir da matriz de compras A,
// calculando I = A x A^T e depois S[i][j] = 1 - I[i][j] / |P_i| (distancia de Jaccard).
void construirMatrizSimilaridade(MatrizSimilaridade *matriz, bool novoAlgoritmo = true);

// Retorna o valor de similaridade (distancia de Jaccard) entre os clientes de
// indices internos i e j.
double obterSimilaridade(MatrizSimilaridade *matriz, int i, int j);

// Dado um cliente de indice interno i, retorna o indice interno do cliente
// mais similar a ele (excluindo ele mesmo). Retorna -1 se nao houver outro cliente.
int clienteMaisSimilar(MatrizSimilaridade *matriz, int i);

void liberar(MatrizSimilaridade *matriz);

// ---------------------- Atividade 5: Matrizes Esparsas (CSR) ----------------------

// Constroi a matriz de compras A (n x m) diretamente no formato CSR, a partir
// da lista de compras (evita alocar a matriz densa n x m). Preenche tambem
// numeroClientes e numeroProdutos em matriz (matriz->A fica NULL).
void construirMatrizComprasCSR(MatrizSimilaridade *matriz, MatrizCSR *A);

// Calcula a matriz de intersecao I = A x A^T utilizando a representacao CSR
// de A, atraves do produto interno entre as linhas de A (simulando a coluna
// de A^T sem construi-la), conforme o algoritmo apresentado na Seção 5.
// O resultado I tambem e armazenado no formato CSR.
MatrizCSR multiplicaCSR(const MatrizCSR &A, int numeroClientes);

// Constroi a matriz de similaridade S (distancia de Jaccard) a partir da
// matriz de compras A e da matriz de intersecao I, ambas no formato CSR.
void construirMatrizSimilaridadeCSR(MatrizSimilaridade *matriz, const MatrizCSR &A);

#endif