#include <iostream>
#include <string>
#include "lista_compras.h"
using namespace std;

int main(void) {
    int count = 1, nClientes = 3; // Quantos clientes testar
    lerArquivo("dados/dados_venda_cluster_0.csv");
    
    cout << "Clientes carregados: " << v_CodigosClientes.size() << endl;
    cout << "Produtos carregados: " << v_NomesProdutos.size() << endl;
    cout << "\nDigite o codigo original de um cliente:\n";


    string codigoCliente;
    while(cin >> codigoCliente && count < nClientes) {
        
        if (i_Cliente.find(codigoCliente) == i_Cliente.end()) {
            cout << "Cliente " << codigoCliente << " nao encontrado.\n";
        } else {
            cout << "Produtos comprados pelo cliente " << codigoCliente << ":\n";
            for (int indiceProduto : compras[i_Cliente.find(codigoCliente)->second]) {
                cout << " - " << v_NomesProdutos[indiceProduto] << endl;
            }
        }
        cout << "\nProximo codigo: \n";
        count++;
    }
    return 0;
}