#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "brkga.h"

Item* load_items(const char* filename,  int *capacity, int *n)
{
    FILE  *file;
    Item *items;

    file = fopen(filename, "r");

    if (file == NULL){
        printf("Não foi possível abrir o arquivo");
        exit(1);
    }

    fscanf(file, "%d", capacity);
    fscanf(file, "%d", n);

    items = (Item*)malloc((*n)*sizeof(Item));
    for (int i = 0; i < *n; i++){
        fscanf(file, "%d %d", &items[i].value, &items[i].weight);
    }

    fclose(file);

    return items;
}

Population* initialize_population(int population_size, int n){

    srand(time(NULL));

    Population *population = (Population*)malloc(sizeof(Population));

    if (population == NULL) {
        printf("Erro ao alocar memória para Population\n");
        exit(1);
    }
    population->size = population_size;
    population->individuals = (Chromosome*)malloc(population_size*sizeof(Chromosome));

    for (int i = 0; i < population_size; i++){
        population->individuals[i].fitness = -1;
        population->individuals[i].total_weight = -1;
        population->individuals[i].gen = (Gene*)malloc(n*sizeof(Gene));
        population->individuals[i].solution = (int*)malloc(n*sizeof(int));

        for (int j = 0; j < n; j++){
            population->individuals[i].gen[j].key= rand()%1000/1000.0;
            population->individuals[i].gen[j].index = i;
            population->individuals[i].solution[j] = 0;
        }
    }

    return population;
}

void sort_genes_vector(Gene *genes, int n){
    Gene aux;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n -1; j++)
        {
            if (genes[j+1].key < genes[j].key)
            {
                aux = genes[j+1];
                genes[j+1] = genes[j];
                genes[j] = aux;
            }
        }
    }
}

void sort_genes_vector(Population *pop){
    Chromosome aux;
    for (int i = 0; i < pop->size; i++)
    {
        for (int j = 0; j < pop->size -1; j++)
        {
            if (pop->individuals[j+1].fitness < pop->individuals[j].fitness)
            {
                aux = pop->individuals[j+1];
                pop->individuals[j+1] = pop->individuals[j];
                pop->individuals[j] = aux;
            }
        }
    }
}

void decode(Population *pop, Item *items, int n, int capacity){
    int idx;
    for (int i = 0; i < pop->size; i++){
        sort_vector(pop->individuals[i].gen, n);
        for(int j=0; j < n; j++){
            idx = pop->individuals[i].gen[j].index;
            if((pop->individuals[i].total_weight + items[idx].weight) < capacity){
                pop->individuals[i].total_weight += items[idx].weight;
                pop->individuals[i].fitness += items[idx].value;
                pop->individuals[i].solution[idx] = 1;
            }
            else{
                return;
            }   
        }
    }
}

int main(){
    char filename[] = "/home/julia/Documents/Engenharia de Computação/PAA/t2_paa/texte.txt";
    Item *items;
    Population *population;
    int capacity, n; 

    items = load_items(filename, &capacity, &n);
    population = initialize_population(5, n);

    printf("Tamanho da população: %d\n", population->size);

    for (int i = 0; i < population->size; i++){
        printf("Indivíduo %d\n", i+1);
        printf("Fitness: %d\n", population->individuals[i].fitness);
        printf("Total_weight: %d\n", population->individuals[i].total_weight);
        printf("Keys: ");
        for(int j = 0; j < n; j++){
            printf("%.3f ", population->individuals[i].gen[j].key);
        }
        printf("\n\n");

    }
    decode(population, items, n, capacity);

    sort_population(population);

    return 0; 
}