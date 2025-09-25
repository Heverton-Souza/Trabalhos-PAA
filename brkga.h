//Integrantes: Andryck Santiago, Danilo Taveira, Frederico de Souza, Heverton Souza, Julia Baptista e Matheus Rinaldi

#ifndef BRKGA_H
#define BRKGA_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ------------------ STRUCTURES ------------------ //
#pragma region STRUCTURES

typedef struct{
    int population_size, generations_number;
    double mutants_percent, elite_percent;
} BRGA_Params;

// Backpack item
typedef struct {
    int value;
    int weight;
} Item;

// Auxiliary gene for decoding (key sorting)
typedef struct {
    int index;      // index of the original item
    double key;     // value of the random key
} Gene;

// Chromosome (Individual of the population)
typedef struct {
    Gene *gen;     // vector of random keys (DNA)
    int *solution;    // decoded binary vector (phenotype)
    int total_value;      // total value of the backpack (fitness)
    int total_weight; // total weight of the backpack
} Chromosome;

// Population (set of chromosomes)
typedef struct { 
    Chromosome *individuals; //Uma solução (Conjunto de ítens da mochila)
    int size;
} Population;

#pragma endregion

// -------------- PARÂMETROS DO BRKGA ------------- //
#pragma region BRKGA
int POP_SIZE = 100;     // Tamanho da população
int STOP = 250;          // Número de gerações
float MUTANTS_PERCENT = 0.1;  // Percentual de mutantes
float ELITE_PERCENT = 0.2;    // Percentual de elite

// ----------- Initialization ---------- //
Population* initialize_population(int population_size, int n);

// ----------- Decoding and Evaluation ----------- //
void decode(Population *pop, Item *items, int n, int capacity);

// ----------- Evolution ----------- //
//void evolve(Population *pop, int n);
void evolve(Population *pop, int n);
// ----------- Genetic Operators ----------- //
void mutation(Chromosome *mutant, int n);
void crossover(Chromosome *child, Chromosome *elite, Chromosome *non_elite, int n);

#pragma endregion

// ----------- ORDENAÇÃO ----------- //
#pragma region 

void sort_genes_vector(Gene *genes, int ini, int fim);
void merge_genes(Gene *genes, int ini, int meio, int fim);
void sort_population(Population *pop, int ini, int fim);
#pragma endregion

// --------------------- DEBUG ---------------------//
#pragma region DEBUG
// Imprime os valores para cada geração
void print_individual(Chromosome ind, int id);

// Imprime as melhores soluções
void print_best_solution(Chromosome result, Item *itens, int n);

#pragma endregion

// ----------- Leitura dos itens ----------- //
#pragma region LEITURA

// Faz a leitura do arquivo e carrega os itens do problema da mochila
Item* load_items(const char* filename, int *capacity, int *n);

#pragma endregion

#endif // BRKGA_H