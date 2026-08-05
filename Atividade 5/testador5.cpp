#include <iostream>
#include <ctime>
#include "lista_compras.h"
#include "similaridade.h"
using namespace std;

// g++ lista_compras.cpp testador5.cpp similaridade.cpp -o testador5; ./testador5 dados/dados_venda_cluster_2.csv

int main(int argc, char **argv) {
    if (argc < 2) {
        cout << "Use o comando: " << argv[0] << " dados/dados_venda_cluster_X.csv" << endl;
        return 1;
    }

    lerArquivo(argv[1]);

    int n = vetorClientes.size();
    int m = nomesProdutos.size();

    cout << "Clientes carregados: " << n << endl;
    cout << "Produtos carregados: " << m << endl;

    // matriz densa (Atividades 2/3/4) 
    MatrizSimilaridade matrizDensa;
    clock_t inicioDensa = clock();
    construirMatrizCompras(&matrizDensa);
    construirMatrizSimilaridade(&matrizDensa);
    clock_t fimDensa = clock();
    double tempoDensa = double(fimDensa - inicioDensa) / CLOCKS_PER_SEC;

    long memoriaDensaA = (long) n * m * sizeof(int);

    // matriz esparsa CSR (atividade 5)
    MatrizSimilaridade matrizCSR;
    MatrizCSR A;
    clock_t inicioCSR = clock();
    construirMatrizComprasCSR(&matrizCSR, &A);
    construirMatrizSimilaridadeCSR(&matrizCSR, A);
    clock_t fimCSR = clock();
    double tempoCSR = double(fimCSR - inicioCSR) / CLOCKS_PER_SEC;

    long memoriaCSR_A = (long) (A.values.size() + A.col_index.size() + A.row_ptr.size()) * sizeof(int);

    cout << "\nComparacao de memoria: matriz de compras (A)" << endl;
    cout << "Densa (n x m): " << memoriaDensaA << " bytes" << endl;
    cout << "CSR:           " << memoriaCSR_A << " bytes" << endl;
    cout << "Economia de memoria: " << (100.0 - (100.0 * memoriaCSR_A / memoriaDensaA)) << " %" << endl;

    cout << "\nComparacao de tempo: construcao da matriz de similaridade" << endl;
    cout << "Abordagem densa: " << tempoDensa << " segundos" << endl;
    cout << "Abordagem CSR:   " << tempoCSR << " segundos" << endl;

    liberar(&matrizDensa);
    liberar(&matrizCSR);

    cout << "\nEncerrando programa.\n" << endl;
    return 0;
}
