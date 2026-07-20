/*
 * Testador da Atividade 1: Modulo ListaCompras
 * ----------------------------------------------
 * Entregavel: exibir os nomes dos produtos comprados por pelo menos
 * 3 clientes, utilizando seus codigos originais como entrada.
 */

#include <iostream>
#include "lista_compras.h"

int main(int argc, char *argv[]) {
    std::string caminhoCSV = "dados/dados_clientes.csv";
    if (argc > 1) {
        caminhoCSV = argv[1];
    }

    ListaCompras lista;
    if (!carregarListaCompras(caminhoCSV, lista)) {
        return 1;
    }

    std::cout << "Arquivo carregado com sucesso.\n";
    std::cout << "Total de clientes: " << lista.clientes.size() << "\n";
    std::cout << "Total de produtos: " << lista.produtos.size() << "\n\n";

    // Utiliza os codigos originais dos 3 primeiros clientes encontrados
    // no arquivo como entrada de teste.
    int quantosClientesTestar = 3;
    if ((int)lista.clientes.size() < quantosClientesTestar) {
        quantosClientesTestar = (int)lista.clientes.size();
    }

    for (int i = 0; i < quantosClientesTestar; i++) {
        std::string codOriginal = lista.clientes[i];
        imprimirComprasCliente(lista, codOriginal);
        std::cout << "\n";
    }

    // Demonstra tambem a busca por um codigo que nao existe, para
    // mostrar o tratamento de erro do modulo.
    imprimirComprasCliente(lista, "CODIGO_INEXISTENTE_999");

    return 0;
}
