/* =====================================================================
 * Universidade Federal de São Carlos - Campus Sorocaba
 * Computação Paralela - 2015/2
 * Orientação: Prof.ª Dr.ª Tiemi C. Sakata
 * 
 * Uma nova versão do código original para encontrar números primos,
 * dessa vez, implementada com OpenMP.
 * 
 * Setembro de 2015
 * 
 * Erico N. Matthiesen			|	400556
 * Gabriel Stankevix Soares		|	511340
 * Maurício Spinardi			|	408174
===================================================================== */

/* Bibliotecas */
#include <stdio.h>
#include <stdlib.h>

#ifdef _OPENMP	// checagem de erro para suporte à OpenMP
	#include <omp.h>
	int omp = 1;
#else
	int omp = 0;
#endif

#include <omp.h>

#include <semaphore.h>
#include "timer.h"

/* Definições */
#define N_ITENS 12

/* Variaveis Globais */
sem_t bloqueio, pos_vazia, pos_ocupada;	// semáforos
int buffer[N_ITENS];	// fila produtor-consumidor
int inicio = 0, fim = 0;	// índices de fila
int n, count = 2;	// count: primos encontrados
int thread_count = 2;	// número mínimo de threads
double start = 0, finish = 0, elapsed = 0;	// variáveis de relógio

/* Cabeçalhos */
void produtor_consumidor();
void tester(int i);
void breaker();

/*
 * Função principal
 */
int main(int argc, char* argv[]) {

	if (!omp) {
		printf("OpenMP not supported\n");
		exit(0);
	}

	printf("Enter the number of prime numbers required\n");
   	scanf("%d",&n);

	/* 
	 * Caso o número de threads desejado pelo usuário satisfaça a
	 * condição de ser maior do que 1, temos a atribuição. Caso
	 * contrário, mantém-se o número mínimo 2 (um produtor e um
	 * consumidor).
	 */
	if (strtol(argv[1], NULL, 10) > 1) {
		thread_count = strtol(argv[1], NULL, 10);
	}
	
	/* 
	 * Inicialização de semáforos
	 */
	sem_init(&bloqueio, 0, 1);
	sem_init(&pos_vazia, 0, N_ITENS);
	sem_init(&pos_ocupada, 0, 0);

	if ( n > 1 ){
		printf("First %d prime numbers are:\n",n);
    	printf("2\n");
		
		GET_TIME(start);
		
		/* 
		 * Paralelização através de OpenMP
		 */ 
		# pragma omp parallel num_threads(thread_count)
	 	produtor_consumidor();
	}
	else {
		if (n == 1) printf("First prime number is: 2\n");
	}

	return 0;
}

/*
 * Thread produtor/consumidor, de acordo com o identificador recuperado
 * pela função omp_get_thread_num() da biblioteca OMP.H
 */
void produtor_consumidor() {
	int tid = omp_get_thread_num();	
	
	/*
	 * @tid Thread ID
	 * Se tid == 0, então produtor. Caso contrário, consumidor.
	 */
	
	if (tid == 0) {
		int i = 3;
		while(1) {
			sem_wait(&pos_vazia);
		    sem_wait(&bloqueio);
				
			buffer[fim] = i;	// acrescenta à fila um novo número
			fim = (fim + 1) % N_ITENS; // atualiza o índice
			if (count <= n) i++;	// produz um novo número
			else breaker();
			
			sem_post(&bloqueio);
   			sem_post(&pos_ocupada);
	  	}
	}
	else {
		int numero_a_ser_testado;
		while (1) {
			sem_wait(&pos_ocupada);
		    sem_wait(&bloqueio);		
				
			numero_a_ser_testado = buffer[inicio];	// consome um número
			inicio = (inicio + 1) % N_ITENS; // atualiza o índice

			sem_post(&bloqueio);
		    sem_post(&pos_vazia);

			if (count <= n) tester(numero_a_ser_testado);
			else breaker();
		}
	}
}

/* 
 * Método verificador. Testa, de fato, se um número é primo.
 * Não ignora os pares e nem efetua maiores otimizações para se manter
 * fiel à proposta do código base.
 */
void tester(int i) {

	int c = 2;

	for (c = 2; c <= i-1; c++) {
		if ( i%c == 0 ) {		
			break;
		}
	}
	
	if (c == i) {
		
		/*
		 * Bloqueia uma região crítica para acesso serial, através de
		 * OpenMP.
		 */
		# pragma omp critical
		{
			if (count <= n) {
				printf("%d\n",i);
				count++;
			}
		}
		
	}
}

/*
 * Método de encerramento. Destrói as execuções em paralelo caso
 * satisfeito.
 */
void breaker() {
	# pragma omp critical
	{
		GET_TIME(finish);
		elapsed = finish - start;
		printf("Elapsed time: %f\n", elapsed);
		exit(0);	//caso satisfeito, encerra produção e consumo
	}
}