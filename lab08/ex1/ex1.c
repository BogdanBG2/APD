#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main (int argc, char *argv[]) {
    int  numtasks, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    int recv_num;
    MPI_Status status;

    // First process starts the circle.
    if (rank == 0) { 
                
        // Generate a random number.
        srand(time(0));
        recv_num = rand() % 50 + 1;

        // Send the number to the next process.
        MPI_Send(
            &recv_num, 1, MPI_INT,
            1, 0,
            MPI_COMM_WORLD);
        printf("[PRC-%d] Has sent %d\n", rank, recv_num);

        MPI_Recv(
            &recv_num, 1, MPI_INT,
            numtasks - 1, 0,
            MPI_COMM_WORLD, &status);
        printf("[PRC-%d] Has recieved %d\n", rank, recv_num);

    // Last process close the circle.
    } /*else if (rank == numtasks - 1) {
    
        // Receives the number from the previous process.
        MPI_Recv(&recv_num, 1, MPI_INT, numtasks - 2, 0, MPI_COMM_WORLD, &status);
        printf("[PRC-%d] Has recieved %d\n", rank, recv_num);
        
        // Increments the number.
        recv_num++;

        // Sends the number to the first process.
        MPI_Send(&recv_num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        printf("[PRC-%d] Has sent %d\n", rank, recv_num);

    } */
    // Middle process.
    else {
        // Receives the number from the previous process.
        MPI_Recv(
            &recv_num, 1, MPI_INT,
            (rank - 1) % numtasks, 0, 
            MPI_COMM_WORLD, &status);
        printf("[PRC-%d] Has recieved %d\n", rank, recv_num);

        // Increments the number.
        recv_num++;

        // Sends the number to the next process.
        MPI_Send(
            &recv_num, 1, MPI_INT,
            (rank + 1) % numtasks, 0,
            MPI_COMM_WORLD);
        printf("[PRC-%d] Has sent %d\n", rank, recv_num);
    }

    MPI_Finalize();
}

