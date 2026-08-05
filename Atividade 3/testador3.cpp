#include <iostream>
#include <string>
#include "lista_compras.h"
#include "similaridade.h"
#include "recomendacao.h"
using namespace std;

// g++ lista_compras.cpp testador3.cpp similaridade.cpp recomendacao.cpp -o testador; ./testador.exe dados/dados_venda_cluster_17.csv

int main(int argc, char **argv) {
    int count = 0;
    if (argc < 2) {
        cout << "Use o comando: " << argv[0] << " dados/dados_venda_cluster_X.csv" << endl;
        return 1;
    }
    lerArquivo(argv[1]);

    MatrizSimilaridade matriz;
    construirMatrizCompras(&matriz);
    construirMatrizSimilaridade(&matriz);

    cout << "\nClientes carregados: " << vetorClientes.size() << endl;
    cout << "Produtos carregados: " << nomesProdutos.size() << endl;

    int k = 5; // numero de produtos recomendados por cliente

    cout << "\nDigite o codigo original de um cliente: " << endl;

    string codigoCliente;
    while (count < 3 && cin >> codigoCliente) {
        if (mapaCliente.find(codigoCliente) == mapaCliente.end()) {
            cout << "Cliente " << codigoCliente << " nao encontrado.\n" << endl;
        } else {
            int idCliente = mapaCliente.find(codigoCliente)->second;
            vector<ProdutoRank> recomendados = recomendarProdutos(&matriz, idCliente, k);

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