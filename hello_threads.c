#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *printHello(void *threadID) {
    printf("Hello, World from a thread!\n");
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number of threads>\n", argv[0]);
        return 1;
    }

    int numThreads = atoi(argv[1]);

    if (numThreads <= 0) {
        fprintf(stderr, "Number of threads should be a positive integer.\n");
        return 1;
    }

    pthread_t threads[numThreads];
    int rc;
    long t;

    for (t = 0; t < numThreads; t++) {
        printf("Creating thread #%ld\n", t);
        rc = pthread_create(&threads[t], NULL, printHello, NULL);
        if (rc) {
            fprintf(stderr, "Error: Unable to create thread #%ld. Return code: %d\n", t, rc);
            exit(-1);
        }
    }

    for (t = 0; t < numThreads; t++) {
        rc = pthread_join(threads[t], NULL);
        if (rc) {
            fprintf(stderr, "Error: Unable to join thread #%ld. Return code: %d\n", t, rc);
            exit(-1);
        }
        printf("Thread #%ld joined\n", t);
    }

    printf("All threads have been joined. Exiting the program.\n");

    pthread_exit(NULL);
}

