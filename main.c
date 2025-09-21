#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "brkga.h"

/*------------------------- Parâmetros do BRKGA ---------------------------*/

int POP_SIZE = 10;     // Tamanho da população
int STOP = 5;          // Número de gerações
float MUTANTS_PERCENT = 0.1;  // Percentual de mutantes
float ELITE_PERCENT = 0.3;    // Percentual de elite

/*------------------------- Funções Auxiliares de Ordenação ---------------------------*/

//Ordena os genes, da menor para a maior chave
void sort_genes_vector(Gene *genes, int n){
    Gene aux;
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n - 1; j++){
            if (genes[j+1].key < genes[j].key){
                aux = genes[j+1];
                genes[j+1] = genes[j];
                genes[j] = aux;
            }
        }
    }
}

//Ordena a população de acordo com os maiores fitness value
void sort_population(Population *pop){
    Chromosome aux;
    for (int i = 0; i < pop->size; i++){
        for (int j = 0; j < pop->size - 1; j++){
            if (pop->individuals[j+1].fitness > pop->individuals[j].fitness){
                aux = pop->individuals[j+1];
                pop->individuals[j+1] = pop->individuals[j];
                pop->individuals[j] = aux;
            }
        }
    }
}

/*------------------------- Funções do BRKGA ---------------------------*/

//Faz o crossover entre os membros Elite e Não Elite
void crossover(Chromosome *child, Chromosome *elite, Chromosome *non_elite, int n){
    for (int i = 0; i < n; i++){
        int p = rand() % 10 + 1;
        if (p > 3){ // 70% chance vem do elite
            child->gen[i].key = elite->gen[i].key;
        } else {    // 30% chance vem do não-elite
            child->gen[i].key = non_elite->gen[i].key;
        }
        child->gen[i].index = i;
    }
}

// Gera um mutante completamente novo para aumentar a variedade da população
void mutation(Chromosome *mutant, int n){
    for (int i = 0; i < n; i++){
        mutant->gen[i].key = rand()%1000/1000.0; //Gera uma chave aleatória de 0.000 a 0.999
        mutant->gen[i].index = i; //Armazena a qual ítem essa chave se refere
    }
}

// Faz a leitura do arquivo e carrega os itens do problema da mochila
Item* load_items(const char* filename, int *capacity, int *n){
    FILE *file;
    Item *items;

    file = fopen(filename, "r");
    if (file == NULL){
        printf("Erro: Não foi possível abrir o arquivo!\n");
        exit(1);
    }

    fscanf(file, "%d", capacity); // capacidade da mochila
    fscanf(file, "%d", n);        // número de itens

    items = (Item*)malloc((*n) * sizeof(Item));
    for (int i = 0; i < *n; i++){
        fscanf(file, "%d %d", &items[i].value, &items[i].weight);
    }

    fclose(file);
    return items;
}

// Inicializa a população de forma aleatória
Population* initialize_population(int population_size, int n){
    srand(time(NULL));

    Population *population = (Population*)malloc(sizeof(Population));
    population->size = population_size;
    population->individuals = (Chromosome*)malloc(population_size * sizeof(Chromosome));

    for (int i = 0; i < population_size; i++){
        population->individuals[i].fitness = 0;
        population->individuals[i].total_weight = 0;
        population->individuals[i].gen = (Gene*)malloc(n * sizeof(Gene));
        population->individuals[i].solution = (int*)malloc(n * sizeof(int));

        for (int j = 0; j < n; j++){
            population->individuals[i].gen[j].key = rand()%1000/1000.0;
            population->individuals[i].gen[j].index = j;
            population->individuals[i].solution[j] = 0;
        }
    }
    return population;
}

// Decodifica os cromossomos
void decode(Population *pop, Item *items, int n, int capacity){
    for (int i = 0; i < pop->size; i++){
        pop->individuals[i].fitness = 0;
        pop->individuals[i].total_weight = 0;

        for (int j = 0; j < n; j++)
            pop->individuals[i].solution[j] = 0;

        // Ordena genes (chaves) -> define prioridade dos itens
        sort_genes_vector(pop->individuals[i].gen, n);

        // Constrói solução
        for(int j = 0; j < n; j++){
            int idx = pop->individuals[i].gen[j].index;
            if((pop->individuals[i].total_weight + items[idx].weight) <= capacity){
                pop->individuals[i].total_weight += items[idx].weight;
                pop->individuals[i].fitness += items[idx].value;
                pop->individuals[i].solution[idx] = 1;
            }
        }
    }
}

// Atualiza população (elitismo + mutantes + descendentes)
void update_population(Population *pop, int n){
    int elite_size = pop->size * ELITE_PERCENT; // número de elite
    int mutants = pop->size * MUTANTS_PERCENT;  // número de mutantes

    Population *new_pop = (Population*)malloc(sizeof(Population));
    new_pop->size = pop->size;
    new_pop->individuals = (Chromosome*)malloc(pop->size * sizeof(Chromosome));

    // 1. Copia elite direto
    for(int i = 0; i < elite_size; i++){
        new_pop->individuals[i] = pop->individuals[i];
    }

    // 2. Gera mutantes
    for(int i = elite_size; i < elite_size + mutants; i++){
        new_pop->individuals[i].fitness = 0;
        new_pop->individuals[i].total_weight = 0;
        new_pop->individuals[i].gen = (Gene*)malloc(n * sizeof(Gene));
        new_pop->individuals[i].solution = (int*)malloc(n * sizeof(int));
        mutation(&new_pop->individuals[i], n);
    }

    // 3. Gera descendentes (crossover)
    for(int i = elite_size + mutants; i < pop->size; i++){
        int elite_parent = rand() % elite_size;
        int non_elite_parent = elite_size + rand() % (pop->size - elite_size);

        new_pop->individuals[i].fitness = 0;
        new_pop->individuals[i].total_weight = 0;
        new_pop->individuals[i].gen = (Gene*)malloc(n * sizeof(Gene));
        new_pop->individuals[i].solution = (int*)malloc(n * sizeof(int));

        crossover(&new_pop->individuals[i], &pop->individuals[elite_parent], &pop->individuals[non_elite_parent], n);
    }

    // Substitui população antiga
    free(pop->individuals);
    pop->individuals = new_pop->individuals;
    free(new_pop);
}

/*------------------------- DEBUG ---------------------------*/

// Imprime os valores para cada geração
void print_individual(Chromosome ind, int id){
    printf(" \nIndivíduo %d | Fitness: %d | Peso: %d\n", id, ind.fitness, ind.total_weight);
    printf("\n");
}

void verify_solution(Chromosome result, Item *itens, int n){
    int weight = 0, value = 0;
    for (int i = 0; i < n; i++){
        if (result.solution[i] == 1){
            weight+= itens[i].weight;
            value += itens[i].value;
        }
    }
    printf("Weight: %d\n Value: %d", weight, value);

}
/*------------------------- Main ---------------------------*/

int main(){
    char filename[] = "/home/julia/Documents/Engenharia de Computação/PAA/t2_paa/teste.txt"; // coloque seu arquivo
    Item *items;
    Population *population;
    int capacity, n; 
    
    // Carrega itens
    items = load_items(filename, &capacity, &n);

    // Inicializa população
    population = initialize_population(POP_SIZE, n);

    // Loop de gerações
    for (int gen = 0; gen < STOP; gen++){
        printf("\n============================\n");
        printf(" Geração %d\n", gen+1);
        printf("============================\n");

        // Decodifica
        decode(population, items, n, capacity);

        // Ordena população
        sort_population(population);

        // Mostra população
        for (int i = 0; i < population->size; i++){
            print_individual(population->individuals[i], i+1);
        }

        // Atualiza população
        if (gen < STOP - 1) update_population(population, n);
    }

    verify_solution(population->individuals[0], items, n);

    return 0; 
}