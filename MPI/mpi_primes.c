#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>   					/* For MPI functions, etc	*/

const int TRUE = 1;
const int FALSE = 0;

const int RANGE = 1000;

int main(void) {
	int			comm_sz;			/* Number of processes	*/
	int			my_rank;			/* My process rank	*/

	int			rnof = 0;			/* Required number of primes */
	int			interval[RANGE];	/* Interval to be tested */
	int			result[RANGE];		/* Result of the interval tested */
	int 		counter = 1;		/* Counter of primes found */
	int			sz = 0;				/* Consumer number after input */
	double		start = 0;			/* Clock start variable */
	double		finish = 0;			/* Clock finish variable */
	
	int c, i, n, q, r;				/* Loop variables */
	
	/* Start up MPI */
	/* MPI_Init(&argc, &argv); */
	MPI_Init(NULL, NULL);

	/* Get the number of processes */
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

	/* Get my rank among all the processes */
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	/* Check if comm_sz does not violate the minimum range required */
	if (comm_sz >= 2) {
		
		switch (my_rank) {
			case 0:			
				/* Sets the total amount of processes */
				sz = comm_sz;
				
				/* Asks for the number of primes required */
				fprintf(stderr, "Enter the number of prime numbers required: ");
				scanf("%d", &rnof);
				
				start = MPI_Wtime();
				
				/* Base values to produce numbers an print results */
				n = 3; r = 3;
				
				if (rnof >= 1)
					printf("2\n");	
				
				while (counter < rnof) {
					
					/* Produce and send an interval to be tested */
					for (q = 1; q < sz; q++) {
						for (i = 0; i < RANGE; i++) {
							interval[i] = n;
							//n += 2;
							n++;
						}
						MPI_Send(interval, RANGE, MPI_INT, q, 0, MPI_COMM_WORLD);
					}
					
					/* Check the return from the other processes and print when its prime */
					for (q = 1; q < sz; q++) {
						MPI_Recv(result, RANGE, MPI_INT, q, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
						for (i = 0; i < RANGE; i++) {
							if (result[i] && (counter < rnof)) {
								printf("%d\n", r);
								counter++;
							}
							//r += 2;
							r++;
						}
					}
					
				}
				
				for (i = 0; i < RANGE; i++)
					interval[i] = 0;
				/* Send an exit signal to everyone */
				for (q = 1; q < sz; q++)
					MPI_Send(interval, RANGE, MPI_INT, q, 0, MPI_COMM_WORLD);
					
				break;
				
			default:
				while (TRUE) {
					/* Receive an interval to be tested or the signal to exit*/
					MPI_Recv(interval, RANGE, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
					
					/* Test if should exit */
					if (interval[0] == 0)
						break;
					
					/* Prepare the result interval */
					for (i = 0; i < RANGE; i++)
						result[i] = TRUE;
					
					/* Test each number of the interval */
					for (i = 0; i < RANGE; i++) {
						for (c = 2; c < interval[i]; c++)
							if (interval[i]%c == 0) break;
					
						if (c != interval[i]) {
							//printf("XABU %d\n", interval[i]);
							result[i] = FALSE;
						}
					}
					
					/* Send an result about the number tested */
					MPI_Send(result, RANGE, MPI_INT, 0, 0, MPI_COMM_WORLD);
				}
		}
		
		if (my_rank == 0) {
			/* Print the execution time */
			finish = MPI_Wtime();
			printf("Elapsed time = %e seconds \n", finish-start);
		}
		
	}
	else
		printf("Minimum number of process should be 2\n");

	/* Shut down MPI */
	MPI_Finalize(); 

	return 0;
}
