#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/*

Se considera un vector N si P numarul de procese.
Sa se imparta vectorul in N/P si sa se calculeze valoarea minima si maxima din vector.
Fiecare proces calculeaza valorile necesare
si le trimite la lider
care calculeaza rezultatul final.
*/

#define N 16
#define MASTER 0

int minimum(int a, int b) {
	return (a < b) ? a : b;
}

void displayVector(int* v, int limit) {
	// DO NOT MODIFY
	int i;
	for(i = 0; i < limit; ++i)
		printf("%d ", v[i]);
	printf("\n");
}
 
int main(int argc, char * argv[]) {
	int rank, i, j;
	int P;

	int** m = (int**)malloc(N * sizeof(int*));
	for (i = 0; i < N; ++i)
        m[i] = (int*)calloc(N, sizeof(int));

    int** mRecv = (int**)malloc(N * sizeof(int*));
    for (i = 0; i < N; ++i)
        mRecv[i] = (int*)calloc(N, sizeof(int));    

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &P);

    srand(time(NULL));
    if (rank == MASTER) {
        for (i = 0; i < N; ++i)
            for (j = 0; j < N; ++j) {
        	   m[i][j] = i * N + j;
               mRecv[i][j] = m[i][j];
            }
    	
        for (i = 0; i < N; ++i)
            displayVector(m[i], N);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(mRecv, N * N, MPI_INT, MASTER, MPI_COMM_WORLD);


    printf("RANK #%d : ", rank);
    for (int i = 0; i < N; ++i)
        displayVector(m[i], N);


    // TODO : Cerinta pe fiecare proces

    // TODO : Procesarea rezultatelor
    if (rank != MASTER) {
    	// TODO : Send rezultat catre MASTER
    }
    else {
    	// TODO : Recv rezulatele celorlalte procese

    	// TODO : Modificarea rezultatului dorit daca este cazul
    }


	MPI_Finalize();
	free(vRecv);
	free(v);
	return 0;
}
