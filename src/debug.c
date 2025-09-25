//Integrantes: Andryck Santiago, Danilo Taveira, Frederico de Souza, Heverton Souza, Julia Baptista e Matheus Rinaldi

#include "brkga.h"

/*------------------------- DEBUG ---------------------------*/

// Imprime os valores para cada geração
void print_individual(Chromosome ind, int id){
    printf(" Individuo (solucao) %d | Valor Total: %d | Peso Total: %d\n", id, ind.total_value, ind.total_weight);
}

// Imprime o resultado final
void print_best_solution(Chromosome result, Item *itens, int num_items){
    int weight = 0, value = 0;
    printf("\nA melhor solucao eh incluir os items: \n");
    for (int i = 0; i < num_items; i++){
        if (result.solution[i] == 1){
            printf("%d, ", i);
            weight+= itens[i].weight;
            value += itens[i].value;
        }
    }
    printf("\nO peso total sera: %d, e o valor total sera: %d", weight, value);

}
