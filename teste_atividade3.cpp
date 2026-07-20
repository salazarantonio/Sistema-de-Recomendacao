/*
 * Testador da Atividade 3: Modulo Recomendacao
 * -----------------------------------------------
 * Entregavel: solicitar a recomendacao para 3 clientes, utilizando
 * seus codigos originais, e exibir os nomes dos k produtos melhor
 * recomendados para cada cliente.
 */

#include <iostream>
#include "lista_compras.h"
#include "similaridade.h"
#include "recomendacao.h"

static void recomendarParaCliente(const ListaCompras &lista, const Similaridade &sim,
                                   const std::string &codCliente, int k) {
    int indice = obterIndiceCliente(lista, codCliente);
    if (indice == -1) {
        std::cout << "Cliente com codigo \"" << codCliente << "\" nao encontrado.\n\n";
        return;
    }

    std::cout << "Recomendacoes para o cliente " << codCliente
              << " (indice interno " << indice << "):\n";

    std::vector<int> recomendados = recomendarProdutos(lista, sim, indice, k);

    if (recomendados.empty()) {
        std::cout << "  Nenhuma recomendacao disponivel para este cliente.\n\n";
        return;
    }

    for (size_t i = 0; i < recomendados.size(); i++) {
        int idxProduto = recomendados[i];
        std::cout << "  " << (i + 1) << ". " << lista.produtos[idxProduto] << "\n";
    }
    std::cout << "\n";
}

int main(int argc, char *argv[]) {
    std::string caminhoCSV = "dados/dados_clientes.csv";
    if (argc > 1) {
        caminhoCSV = argv[1];
    }

    ListaCompras lista;
    if (!carregarListaCompras(caminhoCSV, lista)) {
        return 1;
    }

    std::cout << "Construindo a matriz de similaridade para "
              << lista.clientes.size() << " clientes...\n\n";

    Similaridade sim;
    construirSimilaridade(lista, sim);

    const int k = 5; // quantidade de produtos a recomendar por cliente

    // Testa a recomendacao para os 3 primeiros clientes do arquivo,
    // utilizando seus codigos originais como entrada.
    int quantosClientesTestar = 3;
    if ((int)lista.clientes.size() < quantosClientesTestar) {
        quantosClientesTestar = (int)lista.clientes.size();
    }

    for (int i = 0; i < quantosClientesTestar; i++) {
        recomendarParaCliente(lista, sim, lista.clientes[i], k);
    }

    return 0;
}
