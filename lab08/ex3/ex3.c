#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define MULTI 5 // chunk dimension
#define ROOT 0

int main (int argc, char *argv[]) {
    int numtasks, rank, len;
    char hostname[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Get_processor_name(hostname, &len);

    int num_elements = MULTI * numtasks; // total elements
    int *v_send = NULL; // full vector
    int *v_recv = (int *)malloc(MULTI * sizeof(int)); // partial vector

    if (rank == ROOT) {
        // ROOT process generates the values for the full vector.
        v_send = calloc(num_elements, sizeof(int));

        // Scatter the vector to all processes.
        MPI_Scatter(
            v_send, MULTI, MPI_INT,
            v_recv, MULTI, MPI_INT,
            ROOT,
            MPI_COMM_WORLD);

    }

    // Each process increments the values of the partial vector received.
    for (int i = 0; i < MULTI; ++i)
        v_recv[i] = rank * MULTI + i;
    /*
     * Prints the values received after scatter and modified.
     * NOTE: If MULTI changed, also change this line.
     */
    printf("Process [%d]: have elements %d %d %d %d %d.\n", rank, v_recv[0],
            v_recv[1], v_recv[2], v_recv[3], v_recv[4]);

    // Gathers the values from all the processes.
    MPI_Gather(
        v_recv, MULTI, MPI_INT,
        v_send, MULTI, MPI_INT, 
        ROOT, 
        MPI_COMM_WORLD);

    // The ROOT process prints the elements received.
    if(rank == ROOT) {
        for (int i = 0; i < num_elements; ++i)
            printf("%d ", v_send[i]);
        printf("\n");    
    }

    free(v_recv);
    free(v_send);
    MPI_Finalize();

    return 0;
}

