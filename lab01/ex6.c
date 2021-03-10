#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int min(int a, int b) {
    if(a < b)
        return a;
    return b;
}

int* arr;
int array_size;
long no_threads;

void* execute_5(void * arg) {
    long id = (long) arg;
    int start = id * (double) array_size / no_threads;
    int end = min(array_size, (id + 1) * (double) array_size / no_threads);

    for(int i = start; i < end; arr[i++] += 100);

    pthread_exit(NULL);

}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        perror("Specificati dimensiunea array-ului!\n");
        exit(-1);
    }

    array_size = atoi(argv[1]);

    arr = malloc(array_size * sizeof(int));
    for (int i = 0; i < array_size; ++i)
        arr[i] = i;

    
    // TODO: aceasta operatie va fi paralelizata
    no_threads = sysconf(_SC_NPROCESSORS_CONF);
    if(argc == 3)
        no_threads = atol(argv[2]);
    // al doilea parametru de executie = 
    // = numarul de thread-uri pe care va rula programul

    pthread_t threads[no_threads];
    int r;
    void * status;
    for(long i = 0; i < no_threads; ++i) {
        r = pthread_create(&threads[i], NULL, execute_5, (void*) i);
        if(r) {
            printf("Eroare la crearea thread-ului %ld\n", i);
            exit(-1);
        }
    }

    for(long i = 0; i < no_threads; ++i) {
        r = pthread_join(threads[i], &status);
        if(r) {
            printf("Eroare la asteptarea thread-ului %ld\n", i);
            exit(-1);
        }
    }

  	pthread_exit(NULL);
    free(arr);

    return 0;
}
