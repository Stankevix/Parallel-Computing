/* ==========================================================================
 * Universidade Federal de São Carlos - Campus Sorocaba
 * Disciplina: Computação Paralela
 * Prof. Dr.ª Tiemi Christine Sakata
 *
 * Trabalho 1 - Os N Numeros Primos
 *
 * Aluno: Gabriel Stankevix Soares | RA: 511340
 * Aluno: Mauricio Spinardi        | RA: 408174
 * Aluno: Erico Matthiesen         | RA: 400556
 * ========================================================================== */

/* Bibliotecas */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

/* Variaveis Globais */
int thread_count; //número de threads
int *primes_found; //vetor de resultado, contendo os primos encontrados
int global_iterator; //iterador do vetor de resultados
int number = 2; //número que será testado;
int primes; //número total de primos efetivamente buscados
pthread_mutex_t lock; //bloqueador de acesso concorrente

/* Função de comparação, utilizada por qsort.*/
int cmpfunc (const void * a, const void * b) {
	return ( *(int*)a - *(int*)b );
}

void* is_prime() {	
	while (global_iterator < primes) {

		pthread_mutex_lock(&lock);
			number += 1;
			int my_number = number;
		pthread_mutex_unlock(&lock);

		int c;
		for ( c = 2 ; c < my_number; c++ ) {
			if ( my_number%c == 0 )	break;
		}
		if ( c == my_number ) {
			pthread_mutex_lock(&lock);
				int i = global_iterator;
				global_iterator++;
			pthread_mutex_unlock(&lock);			
			
			primes_found[i] = my_number;
		}
		my_number++;
	}
	return (void**)0;
}

int main(int argc, char* argv[]) {
	long thread; //long para sistemas 64 bits
	pthread_t* thread_handles; //vetor de threads
	int n_primes; //número de primos requisitados pelo usuário
	
	int start = 0; //tempo de início
	int finish = 0; //tempo de encerramento
	int finish_qsort = 0; // tempo de encerramento com ordenação 
	int elapsed = 0; //diferença entre início e encerramento
	int elapsed_qsort = 0; //diferença entre início e encerramento com ordenação
	global_iterator = 1; //inicialização do iterador global do vetor de resposta

	printf("Enter the number of prime numbers required\n");
	scanf("%d",&n_primes);

	primes = n_primes;

	primes_found = (int*)malloc(sizeof(int)*primes);
	if ( n_primes >= 1 ) {
		printf("First %d prime numbers are:\n", n_primes);
		primes_found[0] = 2;
	}
	
	thread_count = strtol(argv[1], NULL, 10);
	thread_handles = malloc(thread_count*sizeof(pthread_t));
	
	GET_TIME(start);
	
	for ( thread = 0; thread < thread_count; thread++ ) {
		pthread_create(&thread_handles[thread], NULL, is_prime, NULL);
	}
	
	for ( thread = 0; thread < thread_count; thread++ ) {
		pthread_join(thread_handles[thread], NULL);
	}

	free(thread_handles);
	
	GET_TIME(finish);
	elapsed = finish - start;
	
	qsort(primes_found, primes, sizeof(int), cmpfunc);

	GET_TIME(finish_qsort);
	elapsed_qsort = finish_qsort - start;

	int i = 0;
	for ( i = 0; i < n_primes; i++ ) {
		printf("%d\n", primes_found[i]);
	}
	
	printf("Elapsed time (without sorting): %d\n", elapsed);
	printf("Elapsed time (with sorting):  %d\n", elapsed_qsort);
	return 0;
}


