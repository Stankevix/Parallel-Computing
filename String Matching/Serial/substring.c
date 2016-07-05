/*	
 * 	UNIVERSIDADE FEDERAL DE SÃO CARLOS - CAMPUS SOROCABA
 * 	Novembro de 2015
 * 	Orientação: Tiemi C. Sakata
 *  
 *	Busca ocorrências de um padrão em um ou mais textos.
 * 
 *	Erico A. N. Matthiesen
 *	Gabriel Stankevix Soares
 *	Maurício Spinardi
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"timer.h"

// Função de busca. Avalia somente um padrão em um texto.
int find(int M, int N, char *pat, char* txt) {
	int end = 0;
	int i, j;
	
	for( i = 0; i <= N-M; i++) {
		for (j = 0; j < M; j++) {
			if (txt[i+j] != pat[j])
				break;
			if( j+1 == M) {
				printf("Found at %d\n", i);
				end = 1;
				break;
			}
		}
	}
	return end;
}

// Função principal. Recebe um padrão e um ou mais textos.
// Também é responsável por exibir o tempo de execução da(s) busca(s).
int main() {
	char *pat, *txt;
	int M, N = -1;
	double start, finish;
	double elapsed = 0;
	
	scanf("%d", &M);	// size of pattern
	pat = malloc(sizeof(char)*M);
	scanf("%s", pat);	// pattern
	
	while (N != 0) {
		scanf("%d", &N);	// size of text
		if (N == 0)
			break;
		
		txt = malloc(sizeof(char)*N);
		scanf("%s", txt);	// text
	
		GET_TIME(start);
		if (!find(M, N, pat, txt))
			printf("Not found!\n");
		GET_TIME(finish);	
		elapsed += finish-start;
		
		free(txt);
	}
	
	printf("Elapsed time: %.2f\n", elapsed);
	return 0;
}
