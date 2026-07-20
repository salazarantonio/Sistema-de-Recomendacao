#include "similaridade.h"

/*
 * -----------------------------------------------------------------------
 * Funcoes auxiliares (uso interno deste arquivo apenas)
 * -----------------------------------------------------------------------
 */

// Constroi a matriz de compras densa A (n x m), onde A[i][k] = 1 se o
// cliente i comprou o produto k, e 0 caso contrario.
static std::vector<std::vector<int> > construirMatrizCompras(const ListaCompras &lista) {
    int n = (int)lista.clientes.size();
    int m = (int)lista.produtos.size();

    std::vector<std::vector<int> > A(n, std::vector<int>(m, 0));

    for (int i = 0; i < n; i++) {
        const std::vector<int> &produtosDoCliente = lista.compras[i];
        for (size_t k = 0; k < produtosDoCliente.size(); k++) {
            int produto = produtosDoCliente[k];
            A[i][produto] = 1;
        }
    }
    return A;
}

// Calcula a transposta de uma matriz (algoritmo padrao, conforme Secao 2.3
// "Matriz Transposta" do documento do projeto).
static std::vector<std::vector<int> > transporMatriz(const std::vector<std::vector<int> > &A) {
    int linhas = (int)A.size();
    int colunas = linhas > 0 ? (int)A[0].size() : 0;

    std::vector<std::vector<int> > T(colunas, std::vector<int>(linhas, 0));

    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            T[j][i] = A[i][j];
        }
    }
    return T;
}

// Multiplicacao padrao de matrizes: C (m x p) = A (m x n) * B (n x p).
// Implementacao direta, seguindo o pseudocodigo "MultiplicaMatrizes" da
// Secao 2.3 do documento do projeto (esta e a versao NAO otimizada;
// a versao que evita construir a transposta e trata da Atividade 4,
// que nao faz parte deste escopo).
static std::vector<std::vector<int> > multiplicarMatrizes(const std::vector<std::vector<int> > &A,
                                                           const std::vector<std::vector<int> > &B) {
    int m = (int)A.size();
    int n = m > 0 ? (int)A[0].size() : 0;
    int p = n > 0 ? (int)B[0].size() : 0;

    std::vector<std::vector<int> > C(m, std::vector<int>(p, 0));

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < p; j++) {
            int soma = 0;
            for (int k = 0; k < n; k++) {
                soma += A[i][k] * B[k][j];
            }
            C[i][j] = soma;
        }
    }
    return C;
}

/*
 * -----------------------------------------------------------------------
 * Funcoes publicas do modulo (declaradas em similaridade.h)
 * -----------------------------------------------------------------------
 */

void construirSimilaridade(const ListaCompras &lista, Similaridade &sim) {
    int n = (int)lista.clientes.size();
    sim.numClientes = n;

    // 1) Matriz de compras densa A (n x m)
    std::vector<std::vector<int> > A = construirMatrizCompras(lista);

    // 2) Transposta de A (m x n)
    std::vector<std::vector<int> > At = transporMatriz(A);

    // 3) Matriz de intersecao I = A x A^T  (n x n)
    std::vector<std::vector<int> > I = multiplicarMatrizes(A, At);

    // 4) Matriz de similaridade final: S[i][j] = 1 - I[i][j] / |P_i|
    sim.matrizS.assign(n, std::vector<double>(n, 0.0));
    for (int i = 0; i < n; i++) {
        int tamanhoPi = (int)lista.compras[i].size();
        for (int j = 0; j < n; j++) {
            if (tamanhoPi == 0) {
                // Cliente sem nenhuma compra: nao ha como calcular a
                // proporcao; tratamos como totalmente dissimilar.
                sim.matrizS[i][j] = 1.0;
            } else {
                sim.matrizS[i][j] = 1.0 - (double)I[i][j] / (double)tamanhoPi;
            }
        }
    }
}

double obterSimilaridade(const Similaridade &sim, int i, int j) {
    return sim.matrizS[i][j];
}

int clienteMaisSimilar(const Similaridade &sim, int indiceCliente) {
    int melhorIndice = -1;
    double melhorValor = 1.0; // s == 1 significa nenhum produto em comum: nao conta como vizinho

    for (int j = 0; j < sim.numClientes; j++) {
        if (j == indiceCliente) {
            continue;
        }
        double valor = sim.matrizS[indiceCliente][j];
        if (valor < melhorValor) {
            melhorValor = valor;
            melhorIndice = j;
        }
    }
    return melhorIndice;
}
