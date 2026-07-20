#ifndef LISTA_COMPRAS_H
#define LISTA_COMPRAS_H

#include <string>
#include <list>
#include <vector>
#include <map>

extern std::vector<std::string> v_CodigosClientes;
extern std::vector<std::string> v_NomesProdutos;
extern std::vector<std::list<int>> compras;
extern std::map<std::string, int> i_Cliente;
extern std::map<std::string, int> i_Produto;

void lerArquivo(const char *caminho);

#endif