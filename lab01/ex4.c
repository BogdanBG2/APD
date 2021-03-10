#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


void *f(void *arg) { // binary print of arg^2
  	long id = (long) arg;
    
  	printf("[THREAD 1] ");
  	for(int i = 15; i >= 0; --i)
  		printf("%ld", ((id * id) & (1 << i)) >> i);
  	printf("\n");
    pthread_exit(NULL);
}

void *g(void *arg) { // binary print of arg+1
	long id = (long) arg;
    
  	printf("[THREAD 2] ");
  	for(int i = 15; i >= 0; --i)
  		printf("%ld", ((id + 1) & (1 << i)) >> i);
  	printf("\n");
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
	pthread_t thread1, thread2;
	int r;
	void *status;
  	
  	long n = 255;
  	r = pthread_create(&thread1, NULL, f, (void*)n);
  	if (r) {
		printf("Eroare la crearea thread-ului 1\n");
		exit(-1);
  	}

  	r = pthread_create(&thread2, NULL, g, (void*)n);
  	if (r) {
		printf("Eroare la crearea thread-ului 2\n");
		exit(-1);
  	}

    r = pthread_join(thread1, &status);
	if (r) {
  		printf("Eroare la asteptarea thread-ului 1\n");
  		exit(-1);
	}

	r = pthread_join(thread2, &status);
	if (r) {
  		printf("Eroare la asteptarea thread-ului 2\n");
  		exit(-1);
	}

  	pthread_exit(NULL);
}
