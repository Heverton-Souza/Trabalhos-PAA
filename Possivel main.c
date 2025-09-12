// possivel main 

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "brkga.h"

#define POP_SIZE 50       // tamanho da população
#define MAX_GEN  100      // número máximo de gerações
#define ELITE_PROB 0.7    // probabilidade de herança de genes da elite
#define MUTATION_RATE 0.1 // taxa de mutação
#define ELITE_SIZE 10     // número de indivíduos de elite
#define MUTANTS 5         // número de mutantes por geração

int main(int argc, char *argv[]) {
    if(argc < 2) {
        printf("Uso: %s <arquivo_de_itens>\n", argv[0]);
        return 1;
    }

    srand(time(NULL)); // inicializa gerador de números aleatórios

    int capacity, n;
    Item *items = load_items(argv[1], &capacity, &n);
    if(items == NULL) {
        printf("Erro ao carregar itens.\n");
        return 1;
    }

    // Populações atual e próxima
    Population current, next;
    initialize_population(&current, POP_SIZE, n);
    initialize_population(&next, POP_SIZE, n);

    // Decodifica população inicial
    for(int i = 0; i < POP_SIZE; i++) {
        decode(&current.individuals[i], items, n, capacity);
    }

    // Evolução
    for(int gen = 0; gen < MAX_GEN; gen++) {
        evolve(&current, &next, items, n, capacity,
               ELITE_PROB, MUTATION_RATE,
               ELITE_SIZE, MUTANTS);

        // Troca populações (swap)
        Population temp = current;
        current = next;
        next = temp;

        // Melhor indivíduo da geração
        qsort(current.individuals, POP_SIZE, sizeof(Chromosome), compare_fitness);
        printf("Geração %d | Melhor fitness: %d | Peso: %d\n",
               gen,
               current.individuals[0].fitness,
               current.individuals[0].total_weight);
    }

    // Melhor solução final
    qsort(current.individuals, POP_SIZE, sizeof(Chromosome), compare_fitness);
    Chromosome best = current.individuals[0];

    printf("\n=== Melhor solução encontrada ===\n");
    printf("Fitness total: %d\n", best.fitness);
    printf("Peso total: %d (capacidade: %d)\n", best.total_weight, capacity);
    printf("Itens escolhidos: ");
    for(int i = 0; i < n; i++) {
        if(best.solution[i] == 1) printf("%d ", i);
    }
    printf("\n");

    return 0;
}
