#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <string>
using namespace std;

typedef struct {
    int date;
    char codClient[9];
    char codProduct[7];
    char nameProduct[100];
} Compra;   

int main(void) {
    FILE *arquivo = fopen("../dados/dados_venda_cluster_0.csv", "r");
    Compra compra;
    
    vector<string> v_CodigosClientes;
    vector<string> v_NomesProdutos;
    vector<list<int>> compras;
    map<string, int> i_Cliente;
    map<string, int> i_Produto;

    if(arquivo == NULL) {perror("Erro ao abrir arquivo"); return 1;}

    // Descartar cabeçalho
    char header[51];
    fscanf(arquivo, "%50[^\n]\n", header);

    while(fscanf(arquivo, "%d;%8[^;];%6[^;];%99[^\n]\n", &compra.date, compra.codClient, compra.codProduct, compra.nameProduct) == 4) {
        string codigoCliente = compra.codClient;
        string codigoProduto = compra.codProduct;
        string nomeProduto = compra.nameProduct;

        // Verfica se indice do cliente NAO existe no local (adiciona no vetor e no map. Existindo, acessa o valor dentro de map). Se dá o mesmo para os produtos
        int indiceCliente;
        if(i_Cliente.find(codigoCliente) == i_Cliente.end()) {
            indiceCliente = v_CodigosClientes.size();
            v_CodigosClientes.push_back(codigoCliente);
            i_Cliente[codigoCliente] = indiceCliente;
            compras.push_back(list<int>()); // Como está "criando o cliente", cria-se o vetor de suas listas de produtos (um vetor de listas). Lista de Compras fig 4.
        } else {
            indiceCliente = i_Cliente.find(codigoCliente)->second; //ponteiro acessando um valor -nesse caso "int"- em map<string, int>
        }

        int indiceProduto;
        if(i_Produto.find(codigoProduto) == i_Produto.end()) {
            indiceProduto = v_NomesProdutos.size();
            v_NomesProdutos.push_back(nomeProduto);
            i_Produto[codigoProduto] = indiceProduto;
        } else {
            indiceProduto = i_Produto.find(codigoProduto)->second;
        }

        // Adiciona a lista de compras a cada indice do cliente (codigo local do cliente)
        compras[indiceCliente].push_back(indiceProduto);
    }
    
    // TESTADOR AQUI

    fclose(arquivo);
    return 0;
}