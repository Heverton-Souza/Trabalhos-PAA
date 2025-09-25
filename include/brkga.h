//Integrantes: Andryck Santiago, Danilo Taveira, Frederico de Souza, Heverton Souza, Julia Baptista e Matheus Rinaldi

#ifndef BRKGA_H
#define BRKGA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ------------------ STRUCTURES ------------------ //
#pragma region STRUCTURES

typedef struct{
    int population_size, generations_number;
    double mutants_percent, elite_percent;
} BRGA_Params;

// Item na mochila
typedef struct {
    int value;
    int weight;
} Item;

// Gene auxiliar para decodificação (ordenação de chave)
typedef struct {
    int index;      // index do item original
    double key;     // valor da chave aleatória
} Gene;

// Cromossomo (Indivíduo de uma população)
typedef struct {
    Gene *gen;        // vetor de chaves aleatórias (DNA)
    int *solution;    // vetor de binario decodificado (fenótipo)
    int total_value;  // valor total da mochila (fitness)
    int total_weight; // peso total da mochila
} Chromosome;

// População (Conjunto de cromossomos)
typedef struct { 
    Chromosome *individuals; // Uma solução (Conjunto de ítens da mochila)
    int size;
} Population;

#pragma endregion

// -------------- BRKGA ------------- //
#pragma region BRKGA
extern int POP_SIZE;           // Tamanho da população
extern int STOP_BY_GEN;        // Número de gerações
extern int STOP_BY_ELITE;      // Parâmetro de vício das gerações
extern float MUTANTS_PERCENT;  // Percentual de mutantes
extern float ELITE_PERCENT;    // Percentual de elite

// ----------- Inicialização ---------- //
Population* initialize_population(int population_size, int num_items);

// ----------- Decodificação e avaliação ----------- //
void decode(Population *pop, Item *items, int num_items, int capacity);

// ----------- Avaliação ----------- //
void evolve(Population *pop, int num_items);
void free_population(Population *pop);

// ----------- Operadores genéticos ----------- //
void mutation(Chromosome *mutant, int num_items);
void crossover(Chromosome *child, Chromosome *elite, Chromosome *non_elite, int num_items);

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
void print_individual(Chromosome ind, int gen);

// Imprime as melhores soluções
void print_best_solution(Chromosome result, Item *itens, int num_items);

#pragma endregion

// ----------- Leitura dos itens ----------- //
#pragma region LEITURA

// Faz a leitura do arquivo e carrega os itens do problema da mochila
Item* load_items(const char* filename, int *capacity, int *num_items);

#pragma endregion

#endif // BRKGA_H