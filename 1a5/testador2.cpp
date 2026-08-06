#include <iostream>
#include <string>
#include "lista_compras.h"
#include "similaridade.h"
using namespace std;

// g++ lista_compras.cpp testador2.cpp similaridade.cpp -o testador2; ./testador2.exe dados/dados_venda_cluster_17.csv

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
    cout << "Digite o codigo original de um cliente: " << endl;

    string codigoCliente;
    while (count < 3 && cin >> codigoCliente /* Clientes a testar*/) {
        if (mapaCliente.find(codigoCliente) == mapaCliente.end()) {
            cout << "Cliente " << codigoCliente << " nao encontrado.\n" << endl;
        } else {
            int indiceCliente = mapaCliente.find(codigoCliente)->second;
            int maisSimilar = clienteMaisSimilar(&matriz, indiceCliente);

            if (maisSimilar == -1) {
                cout << "Nao ha outro cliente na base para comparar.\n" << endl;
            } else {
                cout << "Cliente " << codigoCliente
                     << " (indice interno " << indiceCliente << ")" << endl;
                cout << "Cliente mais similar: codigo " << vetorClientes[maisSimilar]
                     << " (indice interno " << maisSimilar << ")" << endl;
                cout << "Valor de similaridade (distancia de Jaccard): "
                     << obterSimilaridade(&matriz, indiceCliente, maisSimilar) << endl;
            }
        }
        count++;
        if (count < 3) {
            cout << "\nProximo codigo: " << endl;
        }
    }

    liberar(&matriz);
    cout << "\nEncerrando programa.\n" << endl;
    return 0;
}