#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define CONVERGENCE_COEF 50

/*

	Se considera o topologie de tip Inel.
Se implementeaza un algoritm de alegere a liderului
(rank-ul cel mai mare devine lider).
Acesta o sa trimita un numar X la procesul cu rank-ul cel mai mic,
care o sa o trimita mai departe la procesul cu rank-ul imediat urmator,
practic, procesul n trimite la procesul n+1.

	Fiecare proces o sa execute o anumita operatie pe numarul primit,
procesele cu rank impar o sa incrementeze valoarea primita,
iar cele cu rank par o sa o inmulteasca cu 2.

 */

static int num_neigh;
static int neigh[2];

// Setarea topologiei de tip inel
void set_neighbours(int rank, int nProcesses) {
	num_neigh = 2;
	neigh[0] = (rank != 0) ? (rank - 1) % nProcesses : nProcesses - 1;
	neigh[1] = (rank + 1) % nProcesses;
}

int leader_chosing(int rank, int nProcesses) {
	int leader = -1;
	int q;
	leader = rank;
	int leader_neigh = -1;
	MPI_Status s;
	// Executam acest pas pana ajungem la convergenta
	for (int k = 0; k < CONVERGENCE_COEF; ++k) {
		for (int i = 0; i < num_neigh; ++i) {
			MPI_Send(&leader, 1, MPI_INT, neigh[i], 0, MPI_COMM_WORLD);
			MPI_Recv(&leader_neigh, 1, MPI_INT,
				MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &s);

			if (leader_neigh > leader)
				leader = leader_neigh;
		}
	}

	printf("[RANK #%d] Liderul este %d\n", rank, leader);

	return leader;
}


int main(int argc, char * argv[]) {
	int rank, nProcesses, num_procs, leader;
	int *parents, **topology;

	MPI_Init(&argc, &argv);
	MPI_Status status;
	MPI_Request request;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
	 

	// Initializarea topologiei de tip inel si determinarea liderului
	set_neighbours(rank, nProcesses);
	leader = leader_chosing(rank, nProcesses);

	MPI_Barrier(MPI_COMM_WORLD);

	int X = -1;
	int XRecv = -1;

	MPI_Status s;
	/*
	Se implementeaza un algoritm de alegere a liderului
	(rank-ul cel mai mare devine lider). 
	ACESTA o sa trimita[...]
	
	Din cerinta, am inteles ca algoritmul de inel incepe si se termina cu
	liderul determinat anterior. Astfel, am ales sa plec de la lider,
	sa trimit X-ul initial catre vecinul cu rang mai mic
	si sa primesc rezultatul final de la vecinul cu rang mai mare.
	*/
	if (rank == leader) {

		// Initializam X
		X = 0;
		printf("X (INITIAL) = %d.\n", X);

		// Vom trimite X catre vecinul cu rangul mai mic
		// si vom primi XRecv de la vecinul cu rangul mai mare
		int send_rank = (neigh[0] < neigh[1]) ? neigh[0] : neigh[1];
		int recv_rank = (neigh[0] < neigh[1]) ? neigh[1] : neigh[0];

		// Modificam X in functie de paritatea rangului curent
		if (rank % 2 == 1)
			X += 1;
		else
			X *= 2;

		// Transmiterea si receptarea lui X
		MPI_Send(&X, 1, MPI_INT, send_rank, 0, MPI_COMM_WORLD);
		printf("[RANK #%d] Am trimis X = %d catre %d\n",
			rank, X, send_rank);
		
		MPI_Recv(&XRecv, 1, MPI_INT, recv_rank, 0, MPI_COMM_WORLD, &s);
		printf("[RANK #%d] Am primit X = %d de la %d\n",
			rank, XRecv, recv_rank);
	}

	else {
		// De la cine primim XRecv
		int send_rank = neigh[1];
		int recv_rank = neigh[0];

		// Primim X de la vecinul cu valoarea rank-1
		MPI_Recv(&XRecv, 1, MPI_INT, recv_rank, 0, MPI_COMM_WORLD, &s);
		printf("[RANK #%d] Am primit X = %d de la %d\n",
			rank, XRecv, recv_rank);
		
		// Modificam X in functie de paritatea rangului curent
		if (rank % 2 == 1)
			X = XRecv + 1;
		else
			X = XRecv * 2;

		// Trimitem X catre vecinul cu valoarea rank+1
		MPI_Send(&X, 1, MPI_INT, send_rank, 0, MPI_COMM_WORLD);
		printf("[RANK #%d] Am trimis X = %d catre %d\n",
			rank, X, send_rank);
	}

	MPI_Barrier(MPI_COMM_WORLD);

	// Afisarea rezultatului dorit
	if (rank == leader) {
		puts("----------------------------------------");
		printf("Rezultatul final este %d.\n", XRecv);
		puts("----------------------------------------");
	}

	MPI_Finalize();
	return 0;
}