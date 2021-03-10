#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 2

int a = 0;
pthread_mutex_t mutex;

void *f(void* arg) {
	pthread_mutex_t m = *((pthread_mutex_t*) arg);
	pthread_mutex_lock(&m);
	
	for (int idx = 0; idx < 100; ++idx)
		a += 2;
	
	pthread_mutex_unlock(&m);

	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
	int i, r;
	void *status;
	pthread_t threads[NUM_THREADS];

	pthread_mutex_init(&mutex, NULL);

	for (i = 0; i < NUM_THREADS; i++) {
		r = pthread_create(&threads[i], NULL, f, (void*)&mutex);

		if (r) {
			printf("Eroare la crearea thread-ului %d\n", i);
			exit(-1);
		}
	}

	for (i = 0; i < NUM_THREADS; i++) {
		r = pthread_join(threads[i], &status);

		if (r) {
			printf("Eroare la asteptarea thread-ului %d\n", i);
			exit(-1);
		}
	}

	pthread_mutex_destroy(&mutex);
	printf("a = %d\n", a);

	return 0;
}
