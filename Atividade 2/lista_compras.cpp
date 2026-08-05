#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <string>
#include "lista_compras.h"
using namespace std;

typedef struct {
    int date;
    char codClient[9];
    char codProduct[7];
    char nameProduct[100];
} Compra;

vector<string> vetorClientes;
vector<string> nomesProdutos;
vector<list<int>> listaCompras;
map<string, int> mapaCliente;
map<string, int> mapaProduto;

void lerArquivo(const char *caminho) {
    FILE *arquivo = fopen(caminho, "r");
    Compra compra;

    if(arquivo == NULL) {perror("Erro ao abrir arquivo"); exit(1);}

    // Descartar cabeçalho
    char header[51];
    fscanf(arquivo, "%50[^\n]\n", header);

    while(fscanf(arquivo, "%d,%8[^,],%6[^,],%99[^\n]\n", &compra.date, compra.codClient, compra.codProduct, compra.nameProduct) == 4) {

        string codigoCliente = compra.codClient;
        string codigoProduto = compra.codProduct;
        string nomeProduto = compra.nameProduct;

        // Verfica se indice do cliente NAO existe no local (adiciona no vetor e no map. Existindo, acessa o valor dentro de map). Se dá o mesmo para os produtos
        int indiceCliente;
        if(mapaCliente.find(codigoCliente) == mapaCliente.end()) {
            indiceCliente = vetorClientes.size();
            vetorClientes.push_back(codigoCliente);
            mapaCliente[codigoCliente] = indiceCliente;
            listaCompras.push_back(list<int>()); // Como está "criando o cliente", cria-se o vetor de suas listas de produtos (um vetor de listas). Vito lista de compras fig 4.
        } else {
            indiceCliente = mapaCliente.find(codigoCliente)->second;
        }

        int indiceProduto;
        if(mapaProduto.find(codigoProduto) == mapaProduto.end()) {
            indiceProduto = nomesProdutos.size();
            nomesProdutos.push_back(nomeProduto);
            mapaProduto[codigoProduto] = indiceProduto;
        } else {
            indiceProduto = mapaProduto.find(codigoProduto)->second;
        }

        // Adiciona a lista de compras a cada indice do cliente (codigo local do cliente)
        listaCompras[indiceCliente].push_back(indiceProduto);
    }

    fclose(arquivo);
}
