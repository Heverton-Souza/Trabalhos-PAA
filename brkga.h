#ifndef BRKGA_H
#define BRKGA_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ------------------ Structures ------------------ //

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
    double *keys;     // vector of random keys (DNA)
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

// ----------- Initialization ----------- //
void initialize_population(Population *pop, int population_size, int n);

// ----------- Decoding and Evaluation ----------- //
void decode(Chromosome *ind, Item *items, int n, int capacity);

// ----------- Genetic Operators ----------- //
void crossover(Chromosome *elite_parent, Chromosome *non_elite_parent,
               Chromosome *child, int n, double elite_prob);
void mutation(Chromosome *ind, int n);

// ----------- Evolution ----------- //
void evolve(Population *current, Population *next,
            Item *items, int n, int capacity,
            double elite_prob, double mutation_rate,
            int num_elite, int num_mutants);

// ----------- Utilities ----------- //
void copy_chromosome(Chromosome *dest, Chromosome *src, int n);
int compare_fitness(const void *a, const void *b);

#endif // BRKGA_H