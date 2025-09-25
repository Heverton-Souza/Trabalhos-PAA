//Integrantes: Andryck Santiago, Danilo Taveira, Frederico de Souza, Heverton Souza, Julia Baptista e Matheus Rinaldi

#include "brkga.h"

/*------------------------- Funções Auxiliares ---------------------------*/

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

//Função de liberação da memória alocada para as populações anteriores
void free_population(Population *pop){
    for (int i = 0; i < pop->size; i++){
        free(pop->individuals[i].gen);
        free(pop->individuals[i].solution);
    }
    free(pop->individuals);
}