#include "lista_compras.h"

#include <fstream>
#include <iostream>
#include <sstream>

/*
 * -----------------------------------------------------------------------
 * Funcoes auxiliares (uso interno deste arquivo apenas)
 * -----------------------------------------------------------------------
 */

// Remove espacos em branco (e '\r' de arquivos com final de linha do
// Windows) do inicio e do fim de uma string.
static std::string aparar(const std::string &texto) {
    size_t inicio = 0;
    size_t fim = texto.size();

    while (inicio < fim && (unsigned char)texto[inicio] <= ' ') {
        inicio++;
    }
    while (fim > inicio && (unsigned char)texto[fim - 1] <= ' ') {
        fim--;
    }
    return texto.substr(inicio, fim - inicio);
}

/*
 * Separa uma linha de CSV em campos, respeitando o padrao RFC4180:
 *  - campos sao separados por virgula;
 *  - um campo pode vir entre aspas duplas, e nesse caso pode conter
 *    virgulas literais;
 *  - dentro de um campo entre aspas, "" representa uma aspas literal.
 *
 * Essa funcao e necessaria porque nomes de produto como
 * TELEVISOR 32" ...  aparecem no arquivo como "TELEVISOR 32"" ..."
 */
static std::vector<std::string> separarLinhaCSV(const std::string &linha) {
    std::vector<std::string> campos;
    std::string atual;
    bool dentroDeAspas = false;

    for (size_t i = 0; i < linha.size(); i++) {
        char c = linha[i];

        if (dentroDeAspas) {
            if (c == '"') {
                // Verifica se e uma aspas escapada ("")
                if (i + 1 < linha.size() && linha[i + 1] == '"') {
                    atual += '"';
                    i++; // pula a segunda aspas
                } else {
                    dentroDeAspas = false;
                }
            } else {
                atual += c;
            }
        } else {
            if (c == '"') {
                dentroDeAspas = true;
            } else if (c == ',') {
                campos.push_back(atual);
                atual.clear();
            } else {
                atual += c;
            }
        }
    }
    campos.push_back(atual);
    return campos;
}

/*
 * -----------------------------------------------------------------------
 * Funcoes publicas do modulo (declaradas em lista_compras.h)
 * -----------------------------------------------------------------------
 */

bool carregarListaCompras(const std::string &caminhoArquivo, ListaCompras &lista) {
    std::ifstream arquivo(caminhoArquivo.c_str());
    if (!arquivo.is_open()) {
        std::cerr << "Erro: nao foi possivel abrir o arquivo \"" << caminhoArquivo << "\"\n";
        return false;
    }

    // Limpa o estado da estrutura, caso ja tenha sido usada antes.
    lista.clientes.clear();
    lista.mapaClientes.clear();
    lista.produtos.clear();
    lista.mapaProdutos.clear();
    lista.compras.clear();

    std::string linha;
    bool primeiraLinha = true;

    while (std::getline(arquivo, linha)) {
        if (primeiraLinha) {
            // Ignora o cabecalho: DATA_COMPRA,COD_CLIENTE,COD_PRODUTO,NOME_PRODUTO
            primeiraLinha = false;
            continue;
        }

        if (aparar(linha).empty()) {
            continue; // ignora linhas em branco
        }

        std::vector<std::string> campos = separarLinhaCSV(linha);
        if (campos.size() < 4) {
            // Linha mal formada; ignora e segue para a proxima.
            continue;
        }

        // campos[0] = DATA_COMPRA (nao utilizado neste modulo)
        std::string codCliente = aparar(campos[1]);
        std::string codProduto = aparar(campos[2]);
        std::string nomeProduto = aparar(campos[3]);

        // ---- Resolve (ou cria) o indice interno do cliente ----
        int indiceCliente;
        std::map<std::string, int>::iterator itCliente = lista.mapaClientes.find(codCliente);
        if (itCliente == lista.mapaClientes.end()) {
            indiceCliente = (int)lista.clientes.size();
            lista.clientes.push_back(codCliente);
            lista.mapaClientes[codCliente] = indiceCliente;
            lista.compras.push_back(std::vector<int>()); // nova lista de compras vazia
        } else {
            indiceCliente = itCliente->second;
        }

        // ---- Resolve (ou cria) o indice interno do produto ----
        int indiceProduto;
        std::map<std::string, int>::iterator itProduto = lista.mapaProdutos.find(codProduto);
        if (itProduto == lista.mapaProdutos.end()) {
            indiceProduto = (int)lista.produtos.size();
            lista.produtos.push_back(nomeProduto);
            lista.mapaProdutos[codProduto] = indiceProduto;
        } else {
            indiceProduto = itProduto->second;
        }

        // ---- Registra a compra (evita duplicar o mesmo produto para o mesmo cliente) ----
        std::vector<int> &produtosDoCliente = lista.compras[indiceCliente];
        bool jaComprou = false;
        for (size_t k = 0; k < produtosDoCliente.size(); k++) {
            if (produtosDoCliente[k] == indiceProduto) {
                jaComprou = true;
                break;
            }
        }
        if (!jaComprou) {
            produtosDoCliente.push_back(indiceProduto);
        }
    }

    arquivo.close();
    return true;
}

int obterIndiceCliente(const ListaCompras &lista, const std::string &codCliente) {
    std::map<std::string, int>::const_iterator it = lista.mapaClientes.find(codCliente);
    if (it == lista.mapaClientes.end()) {
        return -1;
    }
    return it->second;
}

int obterIndiceProduto(const ListaCompras &lista, const std::string &codProduto) {
    std::map<std::string, int>::const_iterator it = lista.mapaProdutos.find(codProduto);
    if (it == lista.mapaProdutos.end()) {
        return -1;
    }
    return it->second;
}

void imprimirComprasCliente(const ListaCompras &lista, const std::string &codCliente) {
    int indice = obterIndiceCliente(lista, codCliente);
    if (indice == -1) {
        std::cout << "Cliente com codigo \"" << codCliente << "\" nao encontrado.\n";
        return;
    }

    std::cout << "Produtos comprados pelo cliente " << codCliente
               << " (indice interno " << indice << "):\n";

    const std::vector<int> &produtosComprados = lista.compras[indice];
    for (size_t k = 0; k < produtosComprados.size(); k++) {
        int idxProduto = produtosComprados[k];
        std::cout << "  - " << lista.produtos[idxProduto] << "\n";
    }
}
