#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define N 1000
#define MASTER 0

int min(int a, int b) {
	return (a < b) ? a : b;
}

void compareVectors(int* a, int* b) {
	// DO NOT MODIFY
	int i;
	for(i = 0; i < N; ++i)
		if(a[i] != b[i]) {
			printf("Sorted incorrectly : position %d\n", i);
			return;
		}
	printf("Sorted correctly\n");
}

void displayVector(int* v) {
	// DO NOT MODIFY
	int i;
	//int displayWidth = 2 + log10((double)v[N - 1]);
	for(i = 0; i < N; ++i)
	//	printf("%*i", displayWidth, v[i]);
		printf("%d ", v[i]);
	printf("\n");
}

int cmp(const void* a, const void* b) {
	// DO NOT MODIFY
	int A = *(int*)a;
	int B = *(int*)b;
	return A - B;
}
 
int main(int argc, char * argv[]) {
	int rank, i, j;
	int nProcesses;
	MPI_Init(&argc, &argv);

	int pos[N];
	
	int sorted = 0;
	
	int* v = (int*)malloc(sizeof(int) * N);
	int* vQSort = (int*)malloc(sizeof(int) * N);
	int* result = (int*)malloc(sizeof(int) * N);

	// initialize the positions
	for (i = 0; i < N; ++i)
		pos[i] = 0;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
	//printf("Hello from %i/%i\n", rank, nProcesses);

    srand(time(NULL));
    if (rank == MASTER)
        // generate random vector
        for (i = 0; i < N; ++i)
        	v[i] = rand() % 1000000;

    // send the vector to all processes
    MPI_Bcast(v, N, MPI_INT, MASTER, MPI_COMM_WORLD);

	if(rank == MASTER) {
		// DO NOT MODIFY

		// DO NOT MODIFY
		// make copy to check it against qsort
		for(i = 0; i < N; ++i)
			vQSort[i] = v[i];
		qsort(vQSort, N, sizeof(int), cmp);

		
        // receive the new positions
		for (int i = 0; i < N; ++i)
			MPI_Recv(&pos[i], 1, MPI_INT, MPI_ANY_SOURCE, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		// sort the vector v
		for (i = 0; i < N; ++i)
			result[pos[i]] = v[i];

		displayVector(vQSort);
		puts("");
		displayVector(result);
		compareVectors(result, vQSort);
	
	} else {	
        // compute the positions
        int start = (rank - 1) * N / (nProcesses - 1);
        int end = min(N, rank * N / (nProcesses - 1));

        for (i = start; i < end; ++i)
        	for (j = 0; j < N; ++j)
        		if (v[i] > v[j])
        			++pos[i];

        // send the new positions to process MASTER
		for (i = start; i < end; ++i)
			MPI_Send(&pos[i], 1, MPI_INT, MASTER, i, MPI_COMM_WORLD);
	}

	MPI_Finalize();
	return 0;
}
