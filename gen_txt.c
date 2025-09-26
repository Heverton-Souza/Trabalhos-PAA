//Integrantes: Andryck Santiago, Danilo Taveira, Frederico de Souza, Heverton Souza, Julia Baptista e Matheus Rinaldi

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int n = 10000;          // número de itens
    int capacity = 5000;    // capacidade da mochila
    FILE *f = fopen("backpack.txt", "w");
    if (!f) {
        printf("Erro ao criar arquivo!\n");
        return 1;
    }

    srand(time(NULL));

    // Primeira linha: capacidade
    fprintf(f, "%d\n", capacity);
    // Segunda linha: número de itens
    fprintf(f, "%d\n", n);

    // Cada item: valor e peso
    for (int i = 0; i < n; i++) {
        int value = rand() % 500 + 1;   // valor entre 1 e 100
        int weight = rand() % 100 + 1;  // peso entre 1 e 50
        fprintf(f, "%d %d\n", value, weight);
    }

    fclose(f);
    printf("Arquivo teste1000.txt gerado com sucesso!\n");
    return 0;
}
