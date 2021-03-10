#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>

int N;

void compareVectors(int* a, int* b) {
	// DO NOT MODIFY
	int i;
	for(i = 0; i < N; ++i)
		if(a[i] != b[i]) {
			printf("Sorted incorrectly\n");
			return;
		}
	printf("Sorted correctly\n");
}

void displayVector(int* v) {
	// DO NOT MODIFY
	int i;
	for(i = 0; i < N; ++i)
		printf("%d ", v[i]);
	printf("\n");
}

int cmp(const void *a, const void *b) {
	// DO NOT MODIFY
	int A = *(int*)a;
	int B = *(int*)b;
	return A - B;
}

// Use 'mpirun -np 20 --oversubscribe ./pipeline_sort' to run the application with more processes
int main(int argc, char * argv[]) {
	int rank;
	int nProcesses;
	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
	//printf("Hello from %i/%i\n", rank, nProcesses);

	N = nProcesses - 1;
	int actual_value = -1;

	if (rank == 0) { // This code is run by a single process
		int intialValue = -1;
		int sorted = 0;
		int aux;
		int *v = (int*)malloc(sizeof(int) * N);
		int *vQSort = (int*)malloc(sizeof(int) * N);
		int i, val;

		// generate the vector v with random values
		// DO NOT MODIFY
		srandom(42);
		for(i = 0; i < N; i++)
			v[i] = random() % 200;
		//displayVector(v);

		// make copy to check it against qsort
		// DO NOT MODIFY
		for(i = 0; i < N; i++)
			vQSort[i] = v[i];
		qsort(vQSort, N, sizeof(int), cmp);

		// sort the vector v
		for (int i = 0; i < N; ++i)
			MPI_Send(&v[i], 1, MPI_INT, 1, i, MPI_COMM_WORLD);

		// get the sorted version of v   
		for (int i = 0; i < N; ++i)
			MPI_Recv(&v[i], 1, MPI_INT, i + 1, i + 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		
		displayVector(v);
		compareVectors(v, vQSort);

		free(v);
		free(vQSort);

	} else {
		// sort the vector v
		int size = N - (rank - 1);
		
		int *v = (int*)calloc(size, sizeof(int));

		// receive the current sub-vector
		for (int i = 0; i < size; ++i)
			MPI_Recv(&v[i], 1, MPI_INT, rank - 1, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		// once we receive our entire sub-vector,
		// we can initialize the actual value with the first element
		actual_value = v[0];
		for (int i = 1; i < size; ++i)
			if (actual_value <= v[i])
				MPI_Send(&v[i], 1, MPI_INT, rank + 1, i - 1, MPI_COMM_WORLD);
			else {
				MPI_Send(&actual_value, 1, MPI_INT, rank + 1, i - 1, MPI_COMM_WORLD);
				actual_value = v[i];
			}

		// send the current minimum back to the master
		MPI_Send(&actual_value, 1, MPI_INT, 0, rank, MPI_COMM_WORLD);
		free(v);
	}

	MPI_Finalize();
	return 0;
}
