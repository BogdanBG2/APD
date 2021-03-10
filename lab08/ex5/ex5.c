#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROOT 0

int main (int argc, char *argv[]) {
    int  numtasks, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    // Checks the number of processes allowed.
    if (numtasks != 2) {
        printf("Only 2 processes allowed!\n");
        MPI_Finalize();
        return 0;
    }

    // How many numbers will be sent.
    int send_numbers = 10;
    int tag, value;
    MPI_Status status;

    if (rank == ROOT) {
        srand(time(0));
        for (int i = 0; i < send_numbers; ++i) {
            // Generate the random numbers.
            value = rand() % 500;
            // Generate the random tags.
            tag = rand() % 100;

            // Sends the numbers with the tags to the second process.
            printf("ROOT has sent %d with tag %d.\n", value, tag);
            MPI_Send(
                &value, 1, MPI_INT,
                1, tag,
                MPI_COMM_WORLD);
        }
        
    } else {

        for (int i = 0; i < send_numbers; ++i) {
            // Receives the information from the first process.            
            MPI_Recv(
                &value, 1, MPI_INT,
                ROOT, MPI_ANY_TAG,
                MPI_COMM_WORLD, &status);


            // Prints the numbers with their corresponding tags.
            printf("DEST has recieved %d with tag %d.\n", value, status.MPI_TAG);
        }

        //printf("The recieved number is %d with tag %d.\n", value, status.MPI_TAG);
    }

    MPI_Finalize();

}

