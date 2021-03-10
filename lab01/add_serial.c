#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
    schelet pentru exercitiul 5
*/

int min(int a, int b) {
    if(a < b)
        return a;
    return b;
}

int* arr;
int array_size;
long cores;

void* execute_5(void * arg) {
    long id = (long) arg;
    int start = id * (double) array_size / cores;
    int end = min(array_size, (id + 1) * (double) array_size / cores);

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


    printf("[");
    for (int i = 0; i < array_size; ++i) {
        printf("%d", arr[i]);
        if (i < array_size - 1)
            printf(", ");
        else
            printf("]\n");
    }
    
    // TODO: aceasta operatie va fi paralelizata
    cores = sysconf(_SC_NPROCESSORS_CONF);

    pthread_t threads[cores];
    int r;
    void * status;
    for(long i = 0; i < cores; ++i) {
        r = pthread_create(&threads[i], NULL, execute_5, (void*) i);
        if(r) {
            printf("Eroare la crearea thread-ului %ld\n", i);
            exit(-1);
        }
    }

    for(long i = 0; i < cores; ++i) {
        r = pthread_join(threads[i], &status);
        if(r) {
            printf("Eroare la asteptarea thread-ului %ld\n", i);
            exit(-1);
        }
    }

    printf("[");
    for (int i = 0; i < array_size; ++i) {
        printf("%d", arr[i]);
        if (i < array_size - 1)
            printf(", ");
        else
            printf("]\n");
    }
  	
    pthread_exit(NULL);
    free(arr);

    return 0;
}
