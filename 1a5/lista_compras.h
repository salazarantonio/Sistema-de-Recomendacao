#ifndef LISTA_COMPRAS_H
#define LISTA_COMPRAS_H

#include <string>
#include <list>
#include <vector>
#include <map>

extern std::vector<std::string> vetorClientes;
extern std::vector<std::string> nomesProdutos;
extern std::vector<std::list<int>> listaCompras;
extern std::map<std::string, int> mapaCliente;
extern std::map<std::string, int> mapaProduto;

void lerArquivo(const char *caminho);

#endif
