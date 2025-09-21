#ifndef BRKGA_H
#define BRKGA_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ------------------ Structures ------------------ //
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
    int fitness;      // total value of the backpack
    int total_weight; // total weight of the backpack
} Chromosome;

// Population (set of chromosomes)
typedef struct {
    Chromosome *individuals;
    int size;
} Population;


// ----------- Data input ----------- //
Item* load_items(const char* filename, int *capacity, int *n);

// ----------- Initialization ---------- //
Population* initialize_population(int population_size, int n);

// ----------- Decoding and Evaluation ----------- //
void decode(Population *pop, Item *items, int n, int capacity);

// ----------- Genetic Operators ----------- //
void crossover(Chromosome *child, Chromosome *elite, Chromosome *non_elite, int n);

void mutation(Chromosome *mutant, int n);

// ----------- Evolution ----------- //
void evolve(Population *pop, int n);

// ----------- Utilities ----------- //
void sort_genes_vector(Gene *genes, int n);
void sort_population(Population *pop);

#endif // BRKGA_H