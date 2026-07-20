/*
 * Testador da Atividade 2: Modulo Similaridade
 * -----------------------------------------------
 * Entregavel: para dois clientes quaisquer (identificados por seus
 * indices internos), encontrar e exibir o cliente mais similar que
 * nao seja ele mesmo.
 */

#include <iostream>
#include "lista_compras.h"
#include "similaridade.h"

static void testarClienteMaisSimilar(const ListaCompras &lista, const Similaridade &sim, int indice) {
    std::cout << "Cliente de indice " << indice
              << " (codigo original " << lista.clientes[indice] << "):\n";

    int maisSimilar = clienteMaisSimilar(sim, indice);
    if (maisSimilar == -1) {
        std::cout << "  Nenhum cliente com produtos em comum foi encontrado.\n\n";
        return;
    }

    double valorS = obterSimilaridade(sim, indice, maisSimilar);
    std::cout << "  Cliente mais similar -> indice " << maisSimilar
              << " (codigo original " << lista.clientes[maisSimilar] << ")\n";
    std::cout << "  Valor de similaridade S = " << valorS << "\n\n";
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

    // Testa com dois clientes quaisquer, identificados por indice interno.
    int indiceA = 0;
    int indiceB = (int)lista.clientes.size() > 1 ? 1 : 0;

    testarClienteMaisSimilar(lista, sim, indiceA);
    testarClienteMaisSimilar(lista, sim, indiceB);

    return 0;
}
