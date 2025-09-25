//Integrantes: Andryck Santiago, Danilo Taveira, Frederico de Souza, Heverton Souza, Julia Baptista e Matheus Rinaldi

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "brkga.h"

/*------------------------- Funções Auxiliares de Ordenação ---------------------------*/

//Ordena os genes, da menor para a maior chave
void sort_genes_vector(Gene *genes, int ini, int fim){

   int meio;  

   if (ini < fim) {                  

      meio = (ini+fim)/2;

      sort_genes_vector(genes, ini, meio);      
      sort_genes_vector(genes, meio+1, fim);

	  merge_genes(genes, ini, meio, fim); 

   }

}

void merge_genes(Gene *genes, int ini, int meio, int fim){

	Gene *temp;
    int tamanho, inicio1, inicio2;

	tamanho = fim - ini + 1;
	inicio1 = ini;
	inicio2 = meio + 1;
	
	temp = (Gene *) malloc(tamanho*sizeof(Gene));

	for(int i = 0; i < tamanho; i++){

	    if((inicio1 <= meio) && (inicio2 <= fim)){

			if(genes[inicio1].key < genes[inicio2].key)
				temp[i] = genes[inicio1++];
			else
				temp[i] = genes[inicio2++];

	    }
		else{
			
			if(inicio1 <= meio)
				temp[i] = genes[inicio1++];
			else
				temp[i] = genes[inicio2++];

		}
	}

	for(int i = 0, j = ini; i < tamanho; i++, j++)
		genes[j] = temp[i];
	

	free(temp);
}

//Ordena a população de acordo com os maiores fitness value
void merge_population(Population *pop, int ini, int meio, int fim){
    int tamanho = fim - ini + 1;
    int i1 = ini;
    int i2 = meio + 1;

    Chromosome *temp = (Chromosome *) malloc(tamanho * sizeof(Chromosome));
    if (!temp) {
        fprintf(stderr, "Erro: malloc falhou em merge_population\n");
        exit(1);
    }

    for (int k = 0; k < tamanho; k++){
        if ((i1 <= meio) && (i2 <= fim)) {
            // queremos ordenar do maior para o menor (melhor fitness primeiro)
            if (pop->individuals[i1].total_value >= pop->individuals[i2].total_value) {
                temp[k] = pop->individuals[i1++];
            } else {
                temp[k] = pop->individuals[i2++];
            }
        } else {
            if (i1 <= meio)
                temp[k] = pop->individuals[i1++];
            else
                temp[k] = pop->individuals[i2++];
        }
    }

    for (int k = 0, j = ini; k < tamanho; k++, j++){
        pop->individuals[j] = temp[k];
    }

    free(temp);
}

void sort_population(Population *pop, int ini, int fim){
    if (ini < fim) {
        int meio = (ini + fim) / 2;
        sort_population(pop, ini, meio);
        sort_population(pop, meio + 1, fim);
        merge_population(pop, ini, meio, fim);
    }
}

/*------------------------- Funções do BRKGA ---------------------------*/

// Inicializa a população de forma aleatória
Population* initialize_population(int population_size, int n){

    Population *population = (Population*)malloc(sizeof(Population));
    population->size = population_size;
    population->individuals = (Chromosome*)malloc(population_size * sizeof(Chromosome));

    for (int i = 0; i < population_size; i++){
        population->individuals[i].gen = (Gene*)malloc(n * sizeof(Gene));
        population->individuals[i].solution = (int*)malloc(n * sizeof(int));

        for (int j = 0; j < n; j++){
            population->individuals[i].gen[j].key = rand()%RAND_MAX;
            population->individuals[i].gen[j].index = j;
            population->individuals[i].solution[j] = 0;
        }
    }
    return population;
}

// Decodifica os cromossomos
void decode(Population *pop, Item *items, int n, int capacity){
    for (int i = 0; i < pop->size; i++){
        pop->individuals[i].total_value = 0;
        pop->individuals[i].total_weight = 0;
        
        Gene *temp = (Gene*)malloc(n * sizeof(Gene));
        memcpy(temp, pop->individuals[i].gen, n * sizeof(Gene));
        // Ordena genes (chaves) -> define prioridade dos itens
        sort_genes_vector(pop->individuals[i].gen, 0, n-1);

        // Constrói solução
        for(int j = 0; j < n; j++){
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
        memcpy(pop->individuals[i].gen, temp, n * sizeof(Gene));
        // Libera memória
        free(temp);
    }
}

void evolve(Population *pop, int n){
    int elite_size = pop->size * ELITE_PERCENT; // número de elite
    int mutants = pop->size * MUTANTS_PERCENT;  // número de mutantes

    Population new_pop;
    new_pop.size = pop->size;
    new_pop.individuals = (Chromosome*)malloc(pop->size * sizeof(Chromosome));

    // 1. Copia elite direto
    for(int i = 0; i < elite_size; i++){
        new_pop.individuals[i] = pop->individuals[i];
    }

    // 2. Gera mutantes
    for(int i = elite_size; i < elite_size + mutants; i++){
        new_pop.individuals[i].gen = (Gene*)malloc(n * sizeof(Gene));
        new_pop.individuals[i].solution = (int*)malloc(n * sizeof(int));
        mutation(&new_pop.individuals[i], n);
    }

    // 3. Gera descendentes (crossover)
    for(int i = elite_size + mutants; i < pop->size; i++){
        int elite_parent = rand() % elite_size;
        int non_elite_parent = elite_size + rand() % (pop->size - elite_size);

        new_pop.individuals[i].gen = (Gene*)malloc(n * sizeof(Gene));
        new_pop.individuals[i].solution = (int*)malloc(n * sizeof(int));

        crossover(&new_pop.individuals[i], &pop->individuals[elite_parent], &pop->individuals[non_elite_parent], n);
    }

    // Substitui população antiga
    free(pop->individuals);
    pop->individuals = new_pop.individuals;
    
}

// Gera um mutante completamente novo para aumentar a variedade da população
void mutation(Chromosome *mutant, int n){
    for (int i = 0; i < n; i++){
        mutant->gen[i].key = rand()%RAND_MAX; //Gera uma chave aleatória de 0.000 a 0.999
        mutant->gen[i].index = i; //Armazena a qual ítem essa chave se refere
    }
}


//Faz o crossover entre os membros Elite e Não Elite
void crossover(Chromosome *child, Chromosome *elite, Chromosome *non_elite, int n){
    for (int i = 0; i < n; i++){
        int p = rand() % 10 + 1;

        if (p > 3){ // 60% chance vem do elite
            child->gen[i].key = elite->gen[i].key;
        } else {    // 40% chance vem do não-elite
            child->gen[i].key = non_elite->gen[i].key;;
        }

        child->solution[i] = 0;
        child->gen[i].index = i;
    }
}

/*------------------------- DEBUG ---------------------------*/

// Imprime os valores para cada geração
void print_individual(Chromosome ind, int id){
    printf(" Individuo (solucao) %d | Valor Total: %d | Peso Total: %d\n", id, ind.total_value, ind.total_weight);
}

void print_best_solution(Chromosome result, Item *itens, int n){
    int weight = 0, value = 0;
    printf("\nA melhor solucao eh incluir os items: \n");
    for (int i = 0; i < n; i++){
        if (result.solution[i] == 1){
            printf("%d, ", i);
            weight+= itens[i].weight;
            value += itens[i].value;
        }
    }
    printf("\nO peso total sera: %d, e o valor total sera: %d", weight, value);

}

/*-------------------- Leitura dos itens --------------------*/
// Faz a leitura do arquivo e carrega os itens do problema da mochila
Item* load_items(const char* filename, int *capacity, int *n){
    FILE *file;
    Item *items;

    file = fopen(filename, "r");
    if (file == NULL){
        printf("Erro: Nao foi possivel abrir o arquivo!\n");
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

/*------------------------- Main ---------------------------*/
int main(){
    srand(time(NULL));
    char filename[] = "/home/julia/Documents/Engenharia de Computação/PAA/t2_paa/teste.txt"; // coloque seu arquivo
    Item *items;
    Population *population;
    int capacity, n; 
    int previous_elite = 0;
    int cont = 0, gen=0;
    // Carrega itens
    items = load_items(filename, &capacity, &n);

    // Inicializa população
    population = initialize_population(POP_SIZE, n);

    // Loop de gerações
    while(cont < 100){

        // Decodifica
        decode(population, items, n, capacity);

        // Ordena população
        sort_population(population, 0, population->size -1);
        if (previous_elite != population->individuals[0].total_value){
            printf("============================\n");
            printf(" Geracao %d\n", gen+1);
            printf("============================\n");
            print_individual(population->individuals[0], 1);
            previous_elite = population->individuals[0].total_value;
            cont=0;
        }

        evolve(population, n);  
        gen++;
        cont++;

    }

    print_best_solution(population->individuals[0], items, n);

    return 0; 
}