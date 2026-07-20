#ifndef LISTA_COMPRAS_H
#define LISTA_COMPRAS_H

/*
 * Modulo: ListaCompras  (Atividade 1)
 * -----------------------------------
 * Responsavel por ler o historico de compras (arquivo CSV) e organizar
 * as estruturas de dados que representam clientes, produtos e as compras
 * de cada cliente.
 *
 * Escrito em estilo procedural (structs + funcoes livres), sem classes
 * nem metodos. As unicas facilidades de C++ utilizadas sao os
 * containers da STL (std::vector, std::map) e std::string, conforme
 * solicitado. Nao ha heranca, polimorfismo ou sobrecarga de operadores.
 */

#include <string>
#include <vector>
#include <map>

/*
 * Struct principal do modulo.
 *
 *  clientes[i]        -> codigo original (string) do cliente de indice interno i
 *  mapaClientes[cod]  -> indice interno do cliente cujo codigo original e "cod"
 *  produtos[i]        -> nome descritivo do produto de indice interno i
 *  mapaProdutos[cod]  -> indice interno do produto cujo codigo original e "cod"
 *  compras[i]         -> lista (vetor) com os indices internos dos produtos
 *                         comprados pelo cliente de indice interno i
 */
struct ListaCompras {
    std::vector<std::string> clientes;
    std::map<std::string, int> mapaClientes;

    std::vector<std::string> produtos;
    std::map<std::string, int> mapaProdutos;

    std::vector<std::vector<int> > compras;
};

/*
 * Le o arquivo CSV indicado por "caminhoArquivo" e preenche todas as
 * estruturas de "lista". O CSV deve ter a estrutura:
 *
 *      DATA_COMPRA,COD_CLIENTE,COD_PRODUTO,NOME_PRODUTO
 *
 * com a primeira linha sendo o cabecalho (ignorada). Campos podem vir
 * entre aspas duplas (padrao CSV/RFC4180), o que e necessario pois
 * alguns nomes de produto contem aspas (ex.: TELEVISOR 32"").
 *
 * Retorna true se o arquivo foi aberto e lido com sucesso, false caso
 * contrario (ex.: arquivo inexistente).
 */
bool carregarListaCompras(const std::string &caminhoArquivo, ListaCompras &lista);

/*
 * Retorna o indice interno do cliente cujo codigo original e "codCliente",
 * ou -1 caso o codigo nao exista na lista.
 */
int obterIndiceCliente(const ListaCompras &lista, const std::string &codCliente);

/*
 * Retorna o indice interno do produto cujo codigo original e "codProduto",
 * ou -1 caso o codigo nao exista na lista.
 */
int obterIndiceProduto(const ListaCompras &lista, const std::string &codProduto);

/*
 * Imprime no console os nomes dos produtos comprados pelo cliente cujo
 * codigo original e "codCliente". Caso o cliente nao exista, imprime
 * uma mensagem de aviso.
 */
void imprimirComprasCliente(const ListaCompras &lista, const std::string &codCliente);

#endif /* LISTA_COMPRAS_H */
