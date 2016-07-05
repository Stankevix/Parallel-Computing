/*	
 * 	UNIVERSIDADE FEDERAL DE SÃO CARLOS - CAMPUS SOROCABA
 * 	Novembro de 2015
 * 	Orientação: Tiemi C. Sakata
 *  
 *	Busca ocorrências de um padrão em uma ou mais entradas.
 * 
 *	Erico A. N. Matthiesen
 *	Gabriel Stankevix Soares
 *	Maurício Spinardi
 */

#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include<pthread.h> // habilita implementações com PThreads
#include<omp.h> // habilita implementações com OpenMP
#include"timer.h"

// argumentos para find().
struct arg_struct {
    int M, N, ID, OMP;
    char *pat, *txt;
};

/* 
 * função de busca.
 * avalia somente um padrão em uma entrada.
 */
void *find(void *arguments) {
	struct arg_struct *args = arguments;
	
	int end = 0;
	int i, j;
	
	/* 
	 * paraleliza a busca do algoritmo inocente, computando um ganho que
	 * pode facilmente ultrapassar 100% do tempo de esforço original.
	 */
	# pragma omp parallel for private(j) num_threads(args->OMP)
	for( i = 0; i <= args->N - args->M; i++) {
		for (j = 0; j < args->M; j++) {
			if (args->txt[i+j] != args->pat[j])
				break;
			if( j+1 == args->M) {
				printf("Found at %d in text '%d'\n", i, args->ID);
				end = 1;
				break;
			}
		}
	}
	if (!end)
		printf("Not found in text '%d'!\n", args->ID);
		
	free(args);
	return EXIT_SUCCESS;
}

/* 
 * função principal.
 * recebe um padrão e uma ou mais entradas.
 * também é responsável por exibir o tempo de execução da(s) busca(s).
 */
int main(int argc, char* argv[]) {
	char *pat, *txt;
	int M = -1, N = -1, ID = -1;
	
	double start, finish;
	double elapsed = 0;
	double input_start, input_finish;
	double input_elapsed = 0;

	int thread_count = 0;
	struct arg_struct *args;
	long thread = -1; // long para sistemas 64 bits
	pthread_t* thread_handles; // variável de threads
	thread_handles = malloc(sizeof(pthread_t));
	
	scanf("%d", &M); // tamanho do padrão
	pat = malloc(sizeof(char)*M);
	scanf("%s", pat); // padrão
	
	GET_TIME(start);
	while (N != 0) {
		GET_TIME(input_start);
		
		scanf("%d", &N); // tamanho da entrada
		if (N == 0)
			break;
			
		thread_count += 1;
		thread_handles = realloc(thread_handles, thread_count*sizeof(pthread_t));
		ID += 1;
		
		txt = malloc(sizeof(char)*N);
		scanf("%s", txt);	// text
	
		// preenche a estrutura de argumentos para uma nova thread
		args = malloc(sizeof(struct arg_struct));
		(*args).OMP = strtol(argv[1], NULL, 10);
		(*args).ID = ID;
		(*args).M = M;	(*args).N = N;
		(*args).pat = pat;
		(*args).txt = malloc(sizeof(char)*N); strcpy((*args).txt, txt);
		
		GET_TIME(input_finish);
		input_elapsed += input_finish - input_start;
			
		// cria uma nova thread
		thread++;
		pthread_create(&thread_handles[thread], NULL, &find, (void *) args);
			
		free(txt);
	}
	
	for ( thread = 0; thread < thread_count; thread++ )
		pthread_join(thread_handles[thread], NULL);
		
	GET_TIME(finish);
	elapsed = finish - start;

	/*
	 * exibe o total de tempo de esforço para encontrar um padrão,
	 * desconsiderando o que foi gasto com a entrada de dados, de forma
	 * similar ao que ocorre na versão serial do mesmo algoritmo.
	 */
	printf("Elapsed time: %.2f\n", elapsed - input_elapsed);
	return EXIT_SUCCESS;
}
