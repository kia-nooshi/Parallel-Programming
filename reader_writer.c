#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t mutex, resource_access;
int reader_count = 0;

void *reader(void *arg) {
    sem_wait(&mutex);
    if (++reader_count == 1) {
        sem_wait(&resource_access);
    }
    sem_post(&mutex);

    // Reading from the resource
    printf("Reader is reading...\n");

    sem_wait(&mutex);
    if (--reader_count == 0) {
        sem_post(&resource_access);
    }
    sem_post(&mutex);

    pthread_exit(NULL);
}

void *writer(void *arg) {
    sem_wait(&resource_access);

    // Writing to the resource
    printf("Writer is writing...\n");

    sem_post(&resource_access);

    pthread_exit(NULL);
}

int main() {
    sem_init(&mutex, 0, 1);
    sem_init(&resource_access, 0, 1);

    pthread_t readers[5], writers[5];
    int i;

    for (i = 0; i < 5; i++) {
        pthread_create(&readers[i], NULL, reader, NULL);
        pthread_create(&writers[i], NULL, writer, NULL);
    }

    for (i = 0; i < 5; i++) {
        pthread_join(readers[i], NULL);
        pthread_join(writers[i], NULL);
    }

    sem_destroy(&mutex);
    sem_destroy(&resource_access);

    return 0;
}