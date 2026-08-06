#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <cstdlib>
#include "similaridade.h"
#include "recomendacao.h"

namespace py = pybind11;

static MatrizSimilaridade matrizPython;

// Recebe do Python a lista de compras ja processada (Atividade 1 em Python):
// comprasPorCliente[i] = indices internos dos produtos que o cliente i comprou.
void carregarDados(std::vector<std::vector<int>> comprasPorCliente, int numProdutos) {
    int n = comprasPorCliente.size();
    int m = numProdutos;

    matrizPython.numeroClientes = n;
    matrizPython.numeroProdutos = m;
    matrizPython.S = NULL; // mesmo contrato do similaridade.cpp: aloca S na 1a chamada

    matrizPython.A = (int**) malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        matrizPython.A[i] = (int*) malloc(m * sizeof(int));
        for (int k = 0; k < m; k++) matrizPython.A[i][k] = 0;
        for (int idProduto : comprasPorCliente[i]) {
            matrizPython.A[i][idProduto] = 1;
        }
    }
}

void calcularSimilaridade() {
    construirMatrizSimilaridade(&matrizPython); // usa o default (novoAlgoritmo = true, Atividade 4)
}

// pybind11 nao converte struct customizada (ProdutoRank) automaticamente --
// esta funcao converte para tupla, que ja tem conversao pronta com pybind11/stl.h.
std::vector<std::tuple<int, double>> recomendar(int idCliente, int k) {
    std::vector<ProdutoRank> resultado = recomendarProdutos(&matrizPython, idCliente, k);
    std::vector<std::tuple<int, double>> saida;
    for (const ProdutoRank &pr : resultado) {
        saida.push_back(std::make_tuple(pr.idProduto, pr.ranking));
    }
    return saida;
}

// Funcao nomeada (nao lambda) para expor liberar() ao Python
void liberarPython() {
    liberar(&matrizPython);
}

PYBIND11_MODULE(meu_solver, m) {
    m.doc() = "Modulo de recomendacao (Atividades 2, 3 e 4 em C++)";

    m.def("carregar_dados", &carregarDados,
          "Monta a matriz de compras a partir da lista de compras do Python",
          py::arg("compras_por_cliente"), py::arg("num_produtos"));

    m.def("calcular_similaridade", &calcularSimilaridade,
          "Calcula a matriz de similaridade");

    m.def("recomendar_produtos", &recomendar,
          "Retorna (indice_produto, ranking) dos k produtos recomendados",
          py::arg("cliente_id"), py::arg("k"));

    m.def("liberar", &liberarPython,
          "Libera a memoria alocada para a matriz");
}