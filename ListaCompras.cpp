#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int date;
    char codClient[9];
    int codProduct;
    char nameProduct[100];
} Compra;

int main(void) {
    FILE *arquivo = fopen("../dados/dados_venda_cluster_0.csv", "r");
    Compra compra;

    if(arquivo == NULL) {perror("Erro ao abrir arquivo"); return 1;}

    // Descartar cabeçalho
    char header[51];
    fscanf(arquivo, "%50[^\n]\n", header);

    while (fscanf(arquivo, "%d;%8[^;];%d;%99[^\n]\n",
                            &compra.date, compra.codClient,
                            &compra.codProduct, compra.nameProduct) == 4) {
        printf("Data: %d, Cliente: %-8s, Produto: %d, Nome: %s\n",
            compra.date, compra.codClient, compra.codProduct, compra.nameProduct);
    }
    fclose(arquivo);
    return 0;
}