#include <iostream>
#include <string>
#include "lista_compras.h"
#include "similaridade.h"
using namespace std;

int main(void) {
    int count = 0;
    lerArquivo("dados/dados_venda_cluster_17.csv");

    construirMatrizCompras();
    construirMatrizSimilaridade();

    cout << "\nClientes carregados: " << v_CodigosClientes.size() << endl;
    cout << "Produtos carregados: " << v_NomesProdutos.size() << endl;
    cout << "Digite o codigo original de um cliente: " << endl;

    string codigoCliente;
    while (count < 3 && cin >> codigoCliente /* Clientes a testar*/) {
        if (i_Cliente.find(codigoCliente) == i_Cliente.end()) {
            cout << "Cliente " << codigoCliente << " nao encontrado.\n" << endl;
        } else {
            int indiceCliente = i_Cliente.find(codigoCliente)->second;
            int maisSimilar = clienteMaisSimilar(indiceCliente);

            if (maisSimilar == -1) {
                cout << "Nao ha outro cliente na base para comparar.\n" << endl;
            } else {
                cout << "Cliente " << codigoCliente
                     << " (indice interno " << indiceCliente << ")" << endl;
                cout << "Cliente mais similar: codigo " << v_CodigosClientes[maisSimilar]
                     << " (indice interno " << maisSimilar << ")" << endl;
                cout << "Valor de similaridade (distancia de Jaccard): "
                     << obterSimilaridade(indiceCliente, maisSimilar) << endl;
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