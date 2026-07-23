#include <iostream>
#include <string>
#include "lista_compras.h"
#include "similaridade.h"
#include "recomendacao.h"
using namespace std;

int main(int argc, char **argv) {
    lerArquivo(argv[1]);

    construirMatrizCompras();
    construirMatrizSimilaridade();

    cout << "\nClientes carregados: " << vetorClientes.size() << endl;
    cout << "Produtos carregados: " << nomesProdutos.size() << endl;

    int k = 5; // numero de produtos recomendados por cliente
    int count = 0;

    cout << "\nDigite o codigo original de um cliente: " << endl;

    string codigoCliente;
    while (count < 3 && cin >> codigoCliente) {
        if (mapaCliente.find(codigoCliente) == mapaCliente.end()) {
            cout << "Cliente " << codigoCliente << " nao encontrado.\n" << endl;
        } else {
            int idCliente = mapaCliente.find(codigoCliente)->second;
            vector<ProdutoRank> recomendados = recomendarProdutos(idCliente, k);

            cout << "Top " << k << " produtos recomendados para o cliente "
                 << codigoCliente << ":\n" << endl;

            if (recomendados.empty()) {
                cout << " (nenhum produto disponivel para recomendar)" << endl;
            } else {
                for (const ProdutoRank &pr : recomendados) {
                    cout << " - " << nomesProdutos[pr.idProduto]
                         << " (ranking: " << pr.ranking << ")" << endl;
                }
            }
        }
        count++;
        if (count < 3) {
            cout << "\nProximo codigo: " << endl;
        }
    }

    cout << "\nEncerrando programa.\n" << endl;
    return 0;
}