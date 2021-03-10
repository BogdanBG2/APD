#include "mpi.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "string_operations.h"

#define MASTER 0
#define HORROR 1
#define COMEDY 2
#define FNTASY 3
#define SCI_FI 4

#define MAX_PROC sysconf(_SC_NPROCESSORS_CONF)

#define MAX_LINE_LENGTH 2000
#define MAX_PARAGRAPH_LENGTH 1500000
#define MAX_PARAGRAPHS 500
#define MAX_LINES 2500

// Definitiile parametrilor de MPI
int rank;
int numtasks;

int number_of_paragraphs = 0;

int M[4], N, P, idx;

// indicii la care gasim liniile din paragrafele de prelucrat
int master_indices[4][MAX_PARAGRAPHS];

// dimensiunea fiecarui vector de mai sus
int master_sizes[4] = {0, 0, 0, 0};
int worker_size[4] = {0, 0, 0, 0};

// Bariera de pthread
pthread_barrier_t barrier;
pthread_mutex_t mutex;

// Statusul de MPI
MPI_Status status;

// Vectorii indicilor de start si de final
int start[256];
int end[256];

// Fisierele necesare
char input_file[50];
char output_file[50];

//char buffer[MAX_PARAGRAPH_LENGTH];
char master_paragraphs[MAX_PARAGRAPHS][MAX_PARAGRAPH_LENGTH];
char worker_lines[MAX_LINES][MAX_LINE_LENGTH];
char line_buffers[4][MAX_LINE_LENGTH];
char buffer[MAX_PARAGRAPH_LENGTH];
char aux[MAX_PARAGRAPH_LENGTH];

// Variablilele de multithreading la nivel de workeri
pthread_t process_thread[20];
int thread_id[20];

int min(int a, int b) {
	return (a < b) ? a : b;
}

void* master_thread_function(void* arg) {

	int id = *(int*)arg;
	
	if (id == 1) {
		strcpy(master_paragraphs[0], "");
	}

	// Citirea datelor intr-un singur thread
	FILE* fin = fopen(input_file, "r");

	// Obtinerea paragrafelor din text si a numarului de paragrafe
	while (fgets(line_buffers[id - 1], MAX_LINE_LENGTH, fin)) {
		if (id == 1) {
			if(strcmp(line_buffers[0], "\n"))
				strcat(master_paragraphs[number_of_paragraphs],
					line_buffers[0]);
			else // trecem la urmatorul paragraf
				strcpy(master_paragraphs[++number_of_paragraphs], "");
		}
	}

	// Atribuirea numarului de paragrafe pe celelalte thread-uri
	if (id == 1) {
		++number_of_paragraphs;
		for (int i = 0; i < 3; ++i)
			MPI_Send(&number_of_paragraphs, 1, MPI_INT,
				MASTER, 0, MPI_COMM_WORLD);
	}
	else
		MPI_Recv(&number_of_paragraphs, 1, MPI_INT,
			MASTER, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	

	// Procesarea paragrafelor dupa primul rand
	// si stabilirea tipului fiecaruia
	for (int i = 0; i < number_of_paragraphs; ++i) {
		if ((id == HORROR
			&& !strncmp(master_paragraphs[i], "horror", 6))

		|| (id == COMEDY
			&& !strncmp(master_paragraphs[i], "comedy", 6))

		|| (id == FNTASY
			&& !strncmp(master_paragraphs[i], "fantasy", 7))

		|| (id == SCI_FI
			&& !strncmp(master_paragraphs[i], "science-fiction", 15)))
			master_indices[id - 1][master_sizes[id - 1]++] = i;
	}



	// Trimiterea paragrafelor catre workeri si receptarea modificarilor
	// Pentru a primi paragrafele corecte, trebuie ca toata procedura urmatoare
	// sa fie plasata intr-o zona critica
	MPI_Send(&master_sizes[id - 1], 1, MPI_INT,
		id, 0, MPI_COMM_WORLD);
	
	pthread_mutex_lock(&mutex);
	size_t len;
	for (int j = 0; j < master_sizes[id - 1]; ++j) {

		// Care este indicele paragrafului pe care vrem sa-l trimitem?
		idx = master_indices[id - 1][j];
		
		len = strlen(master_paragraphs[idx]);
		// Trimiterea paragrafului curent catre worekerul corespunzator
		MPI_Send(&len, 1, MPI_INT,
			id, 0, MPI_COMM_WORLD);

		MPI_Send(master_paragraphs[idx], len, MPI_CHAR,
			id, j, MPI_COMM_WORLD);
		
		// Primirea rezultatului procesat
		// si actualizarea paragrafului curent
		MPI_Recv(&len, 1, MPI_INT,
			id, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		MPI_Recv(master_paragraphs[idx], len, MPI_CHAR,
			id, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	pthread_mutex_unlock(&mutex);

	// Nu putem trece mai departe pana nu avem toate paragrafele modificate
	pthread_barrier_wait(&barrier);
	
	// Scrierea rezultatului
	if (id == 1) {
		
		// Deschiderea fisierului
		FILE* out = fopen(output_file, "w");

		for (int i = 0; i < number_of_paragraphs; ++i)
			fprintf(out, "%s", master_paragraphs[i]);
		fclose(out);
	}
	fclose(fin);
	pthread_exit(NULL);
}

void modify_line(char s[MAX_LINE_LENGTH]) {
	switch (rank) {
		case HORROR:
			horror(s);
			break;
		case COMEDY:
			comedy(s);
			break;
		case FNTASY:
			fantasy(s);
			break;
		case SCI_FI:
			science_fiction(s);
			break;
		default:
			break;
	}
}

void* process_text_function(void* arg) {
	int id = *(int*)arg;

	for (int i = id; i < P; i += MAX_PROC - 1) {
		if (start[i] == end[i])
			modify_line(worker_lines[start[i]]);
		else {
			for (int j = start[i]; j < end[i]; ++j)
				modify_line(worker_lines[j]);
		}
	}
	pthread_exit(NULL);
}

void* worker_read(void* arg) {

	// Cate paragrafe are workerul actual de procesat?
	MPI_Recv(&M[rank - 1], 1, MPI_INT,
		MASTER, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	int len;
	for (int i = 0; i < M[rank - 1]; ++i) {

		// Primirea paragrafului
		memset(buffer, 0, MAX_PARAGRAPH_LENGTH);
		MPI_Recv(&len, 1, MPI_INT,
			MASTER, 0, MPI_COMM_WORLD, &status);
		
		MPI_Recv(buffer, len, MPI_CHAR,
			MASTER, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		// Spargerea paragrafului in linii
		idx = 0;
		for (char* p = strtok(buffer, "\n"); p; p = strtok(NULL, "\n")) {
			memset(worker_lines[idx], 0, MAX_LINE_LENGTH);
			strcpy(worker_lines[idx], p);
			strcat(worker_lines[idx++], "\n");
		}
		N = idx;
		
		// De cate thread-uri avem nevoie?
		P = 1 + (N - 1) / 20;

		for (int j = 0; j < P; ++j) {
			start[j] = 1 + j * 20;
			end[j] = min(N, 1 + (j + 1) * 20);
		}

		// Rularea thread-urilor
		int threads_to_open = min(P, MAX_PROC - 1);

		for (int j = 0; j < threads_to_open; ++j) {
			thread_id[j] = j;
			pthread_create(&process_thread[j], NULL,
				process_text_function, &thread_id[j]);	
		}

		for (int j = 0; j < threads_to_open; ++j)
			pthread_join(process_thread[j], NULL);

		// Obtinerea noului paragraf
		strcpy(buffer, "");
		for (int j = 0; j < N; ++j)
			strcat(buffer, worker_lines[j]);
		strcat(buffer, "\n");

		// Trimiterea rezultatului inapoi la master
		size_t len2 = strlen(buffer);
		MPI_Send(&len2, 1, MPI_INT,
			MASTER, 0, MPI_COMM_WORLD);
		
		MPI_Send(buffer, len2, MPI_CHAR,
			MASTER, 0, MPI_COMM_WORLD);

	}

	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
	
	strcpy(input_file, argv[1]);
	strcpy(output_file, input_file);

	// Setarea numelui fisierului de iesire
	output_file[strlen(output_file) - 3] = 'o';
	output_file[strlen(output_file) - 2] = 'u';
	output_file[strlen(output_file) - 1] = 't';

	// Initializarea MPI-ului
	int provided;
	MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided); 
	if(provided != MPI_THREAD_MULTIPLE) {
   		fprintf(stderr,
   			"This MPI implementation does not support MPI_THREAD_MULTIPLE.\n");
   		exit(-1);
	}

	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    pthread_mutex_init(&mutex, NULL);


    // Master
    if (rank == MASTER) {
    	pthread_barrier_init(&barrier, NULL, 4);

    	pthread_t tid[4];
    	int thread_id[4];

    	for (int i = 0; i < 4; ++i) {
    		thread_id[i] = i + 1;
    		pthread_create(&tid[i], NULL,
    			master_thread_function, &thread_id[i]);
    	}

    	for (int i = 0; i < 4; ++i)
    		pthread_join(tid[i], NULL);

    	pthread_barrier_destroy(&barrier);
	}

	// Worker
	else {
		
		pthread_t w_read;
		int thread_id = 0;

		pthread_create(&w_read, NULL, worker_read, &thread_id);
		pthread_join(w_read, NULL);
	}

	pthread_mutex_destroy(&mutex);
    MPI_Finalize();

	return 0;
}
