//Integrantes: Andryck Santiago, Danilo Taveira, Frederico de Souza, Heverton Souza, Julia Baptista e Matheus Rinaldi

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "brkga.h"

// Coloque o caminho para o seu arquivo aqui
char path_to_backpack[] = "C:\\Users\\Oca\\Downloads\\Trabalhos-PAA\\output\\teste.txt";

/*------------------------- Parâmetros do BRKGA ---------------------------*/
int POP_SIZE = 100;           // Tamanho da população
int STOP_BY_GEN = 250;        // Número de gerações
int STOP_BY_ELITE = 100;      // Parâmetro de vício das gerações
float MUTANTS_PERCENT = 0.1;  // Percentual de mutantes
float ELITE_PERCENT = 0.2;    // Percentual de elite

/*-------------------- Leitura dos itens --------------------*/
// Faz a leitura do arquivo e carrega os itens do problema da mochila
Item* load_items(const char* filename, int *capacity, int *num_items){
    FILE *file;
    Item *items;

    file = fopen(filename, "r");
    if (file == NULL){
        printf("Erro: Nao foi possivel abrir o arquivo!\n");
        exit(1);
    }

    fscanf(file, "%d", capacity);  // capacidade da mochila
    fscanf(file, "%d", num_items); // número de itens

    items = (Item*)malloc((*num_items) * sizeof(Item));
    for (int i = 0; i < *num_items; i++){
        fscanf(file, "%d %d", &items[i].value, &items[i].weight);
    }

    fclose(file);
    return items;
}

/*------------------------- Main ---------------------------*/
int main(){
    srand(time(NULL));
    Item *items;
    Population *population;
    int capacity, num_items; 
    int previous_elite = 0;
    int cont = 0, gen=0;
    // Carrega itens
    items = load_items(path_to_backpack, &capacity, &num_items);

    // Inicializa população
    population = initialize_population(POP_SIZE, num_items);

    // Loop de gerações
    while(cont < STOP_BY_ELITE && gen < STOP_BY_GEN){

        // Decodifica
        decode(population, items, num_items, capacity);

        // Ordena população
        sort_population(population, 0, population->size -1);
        if (previous_elite != population->individuals[0].total_value){
            print_individual(population->individuals[0], gen);
            previous_elite = population->individuals[0].total_value;
            cont=0;
        }

        evolve(population, num_items);  
        gen++;
        cont++;

    }

    print_best_solution(population->individuals[0], items, num_items);

    return 0; 
}