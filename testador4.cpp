#include <iostream>
#include <string>
#include <ctime>
#include <iomanip> // Necessário para setprecision e fixed
#include "lista_compras.h"
#include "similaridade.h"

using namespace std;

// g++ lista_compras.cpp testador4_completo.cpp similaridade.cpp -o testador4; /testador4 dados/dados_venda_cluster_17.csv

int main(int argc, char **argv) {
    if (argc < 2) {
        cout << "Uso correto: " << argv[0] << " <caminho_para_arquivo_csv>" << endl;
        return 1;
    }

    cout << "--- Carregando Base de Dados ---" << endl;
    lerArquivo(argv[1]);

    MatrizSimilaridade matriz;
    matriz.S = NULL;
    construirMatrizCompras(&matriz);

    int n = matriz.numeroClientes;
    int m = matriz.numeroProdutos;

    cout << "Clientes carregados: " << n << endl;
    cout << "Produtos carregados: " << m << endl;

    // Configura a saída padrão para exibir números de ponto flutuante com 2 casas decimais
    cout << fixed << setprecision(2);

    // ---------------------------------------------------------
    // 1. Teste de Desempenho (Tempo de Execução)
    // ---------------------------------------------------------
    cout << "\nDesempenho" << endl;
    
    clock_t inicio_padrao = clock();
    construirMatrizSimilaridade(&matriz, false); 
    clock_t fim_padrao = clock();
    double tempo_padrao = double(fim_padrao - inicio_padrao) / CLOCKS_PER_SEC;
    cout << "Tempo (Padrao): " << tempo_padrao << " segundos" << endl;

    clock_t inicio_adaptado = clock();
    construirMatrizSimilaridade(&matriz, true); 
    clock_t fim_adaptado = clock();
    double tempo_adaptado = double(fim_adaptado - inicio_adaptado) / CLOCKS_PER_SEC;
    cout << "Tempo (Adaptado): " << tempo_adaptado << " segundos" << endl;

    if (tempo_adaptado > 0) {
        double ganho = tempo_padrao / tempo_adaptado;
        cout << "O algoritmo adaptado foi aproximadamente " << ganho << "x mais rapido.\n" << endl;
    }

    // ---------------------------------------------------------
    // 2. Validação Prática (Busca de Clientes Similares)
    // ---------------------------------------------------------
    cout << "Digite o codigo original de um cliente:" << endl;

    string codigoCliente;
    int count = 0;
    
    while (count < 3 && cin >> codigoCliente) {
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
                     << obterSimilaridade(&matriz, indiceCliente, maisSimilar) << "\n" << endl;
            }
        }
        count++;
        if (count < 3) {
            cout << "Proximo codigo:" << endl;
        }
    }

    liberar(&matriz);
    cout << "\nEncerrando programa.\n" << endl;
    return 0;
}