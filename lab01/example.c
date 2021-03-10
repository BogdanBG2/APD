#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_CORES (int)sysconf(_SC_NPROCESSORS_CONF)

void *f(void *arg) {
  	long id = *(long*) arg;
    for(int i = 0; i < 100; ++i)
        printf("Hello World from thread %ld, iteration (%d)!\n", id, i+1);
  	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
	pthread_t threads[NUM_CORES];
	int r;
	long id;
	void *status;
	long arguments[NUM_CORES];


    printf("%d cores.\n", NUM_CORES);
	for (id = 0; id < NUM_CORES; id++) {
    arguments[id] = id;
	  r = pthread_create(&threads[id], NULL, f, &arguments[id]);

	  if (r) {
  		printf("Eroare la crearea thread-ului %ld\n", id);
  		exit(-1);
	  }
	}

	for (id = 0; id < NUM_CORES; id++) {
    r = pthread_join(threads[id], &status);
		if (r) {
	  		printf("Eroare la asteptarea thread-ului %ld\n", id);
	  		exit(-1);
		}
	}

  	pthread_exit(NULL);
}
