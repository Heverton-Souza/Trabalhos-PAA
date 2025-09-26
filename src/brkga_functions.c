//Integrantes: Andryck Santiago, Danilo Taveira, Frederico de Souza, Heverton Souza, Julia Baptista e Matheus Rinaldi

#include "brkga.h"

/*------------------------- Funções do BRKGA ---------------------------*/

// Inicializa a população de forma aleatória
Population* initialize_population(int population_size, int num_items){

    Population *population = (Population*)malloc(sizeof(Population));
    population->size = population_size;
    population->individuals = (Chromosome*)malloc(population_size * sizeof(Chromosome));

    for (int i = 0; i < population_size; i++){
        population->individuals[i].gen = (Gene*)malloc(num_items * sizeof(Gene));
        population->individuals[i].solution = (int*)malloc(num_items * sizeof(int));

        for (int j = 0; j < num_items; j++){
            population->individuals[i].gen[j].key = rand()%RAND_MAX;
            population->individuals[i].gen[j].index = j;
            population->individuals[i].solution[j] = 0;
        }
    }
    return population;
}

// Decodifica os cromossomos
void decode(Population *pop, Item *items, int num_items, int capacity){
    for (int i = 0; i < pop->size; i++){
        pop->individuals[i].total_value = 0;
        pop->individuals[i].total_weight = 0;
        
        Gene *temp = (Gene*)malloc(num_items * sizeof(Gene));
        memcpy(temp, pop->individuals[i].gen, num_items * sizeof(Gene));
        // Ordena genes (chaves) -> define prioridade dos itens
        sort_genes_vector(pop->individuals[i].gen, 0, num_items-1);

        // Constrói solução
        for(int j = 0; j < num_items; j++){
            int idx = pop->individuals[i].gen[j].index;
            if((pop->individuals[i].total_weight + items[idx].weight) <= capacity){
                pop->individuals[i].total_weight += items[idx].weight;
                pop->individuals[i].total_value += items[idx].value;
                pop->individuals[i].solution[idx] = 1;
            }
            else{
                break;
            }
        }
        // Restaura os genes originais (se precisar)
        memcpy(pop->individuals[i].gen, temp, num_items * sizeof(Gene));
        // Libera memória
        free(temp);
    }
}

// Evolui a população
void evolve(Population *pop, int num_items){
    int elite_size = pop->size * ELITE_PERCENT; // número de elite
    int mutants = pop->size * MUTANTS_PERCENT;  // número de mutantes

    Population new_pop;
    new_pop.size = pop->size;
    new_pop.individuals = (Chromosome*)malloc(pop->size * sizeof(Chromosome));

    // 1. Copia elite direto
    for(int i = 0; i < elite_size; i++){
        new_pop.individuals[i].gen = (Gene*)malloc(num_items * sizeof(Gene));
        new_pop.individuals[i].solution = (int*)malloc(num_items * sizeof(int));
        memcpy(new_pop.individuals[i].gen, pop->individuals[i].gen, num_items * sizeof(Gene));
        memcpy(new_pop.individuals[i].solution, pop->individuals[i].solution, num_items * sizeof(int));
        new_pop.individuals[i].total_value = pop->individuals[i].total_value;
        new_pop.individuals[i].total_weight = pop->individuals[i].total_weight;
    }

    // 2. Gera mutantes
    for(int i = elite_size; i < elite_size + mutants; i++){
        new_pop.individuals[i].gen = (Gene*)malloc(num_items * sizeof(Gene));
        new_pop.individuals[i].solution = (int*)malloc(num_items * sizeof(int));
        mutation(&new_pop.individuals[i], num_items);
    }

    // 3. Gera descendentes (crossover)
    for(int i = elite_size + mutants; i < pop->size; i++){
        int elite_parent = rand() % elite_size;
        int non_elite_parent = elite_size + rand() % (pop->size - elite_size);

        new_pop.individuals[i].gen = (Gene*)malloc(num_items * sizeof(Gene));
        new_pop.individuals[i].solution = (int*)malloc(num_items * sizeof(int));

        crossover(&new_pop.individuals[i], &pop->individuals[elite_parent], &pop->individuals[non_elite_parent], num_items);
    }

    // Substitui população antiga
    free_population(pop);
    pop->individuals = new_pop.individuals;
    
}

// Gera um mutante completamente novo para aumentar a variedade da população
void mutation(Chromosome *mutant, int num_items){
    for (int i = 0; i < num_items; i++){
        mutant->gen[i].key = rand()%RAND_MAX; //Gera uma chave aleatória de 0.000 a 0.999
        mutant->gen[i].index = i; //Armazena a qual ítem essa chave se refere
    }
}

//  Faz o crossover entre os membros Elite e Não Elite
void crossover(Chromosome *child, Chromosome *elite, Chromosome *non_elite, int num_items){
    for (int i = 0; i < num_items; i++){
        int p = rand() % 10 + 1;

        if (p > 3){ // 70% chance vem do elite
            child->gen[i].key = elite->gen[i].key;
        } else {    // 30% chance vem do não-elite
            child->gen[i].key = non_elite->gen[i].key;;
        }

        child->solution[i] = 0;
        child->gen[i].index = i;
    }
}