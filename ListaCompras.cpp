#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int date;
    char codClient[50];
    char codProduct[50];
    char nameProduct[50];
} Compra;

int main(void) {
    FILE *arquivo = fopen("../dados/dados_venda_cluster_0.csv", "r");
    Compra compra;

    if(arquivo == NULL) {perror("Erro ao abrir arquivo"); return 1;}

    // Erro para corrigir nao entrada no loop
    char header[52];
    fscanf(arquivo, "%51[^\n]\n", header);

    printf ("Iniciando leitura de arquivos CSV:\n");
    printf("-----------------------------------------------------\n");

    while (fscanf(arquivo, "%d, %19[^,], %19[^,], %49[^\n]\n", 
                  &compra.date,
                  compra.codClient,
                  compra.codProduct,
                  compra.nameProduct) == 4) {
        printf("Data: %d, Cliente: %-10s, Produto: %-8s, Nome: %s\n",
               compra.date, compra.codClient, compra.codProduct, compra.nameProduct);
    }
    fclose(arquivo);
    return 0;
}