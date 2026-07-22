#include <iostream>
#include <string>
#include "lista_compras.h"
using namespace std;

int main(void) {
    int count = 0;
    lerArquivo("dados/dados_venda_cluster_17.csv");
    
    cout << "\nClientes carregados: " << vetorClientes.size() << endl;
    cout << "Produtos carregados: " << nomesProdutos.size() << endl;
    cout << "Digite o codigo original de um cliente:\n" << endl;

    string codigoCliente;
    while(count < 3 && cin >> codigoCliente /* Clientes a testar */) {
        if (mapaCliente.find(codigoCliente) == mapaCliente.end()) {
            cout << "Cliente " << codigoCliente << " nao encontrado.\n" << endl;
        } else {
            cout << "Produtos comprados pelo cliente " << codigoCliente << ":\n" << endl;
            for (int indiceProduto : listaCompras[mapaCliente.find(codigoCliente)->second]) {
                cout << " - " << nomesProdutos[indiceProduto] << endl;
            }
        }
        count++;
        if(count < 3) {
            cout << "\nProximo codigo: \n" << endl;
        }
    }

    cout << "\nEncerrando programa.\n" << endl;
    return 0;
}
