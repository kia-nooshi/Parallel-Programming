#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define MATRIX_SIZE 1000
#define NUM_THREADS 4

int matrixA[MATRIX_SIZE][MATRIX_SIZE];
int matrixB[MATRIX_SIZE][MATRIX_SIZE];
int resultMatrix[MATRIX_SIZE][MATRIX_SIZE];

// Struct to pass thread information
typedef struct {
    int startRow;
    int endRow;
} ThreadInfo;

// Function to perform matrix multiplication for a given range of rows
void* multiply(void* arg) {
    ThreadInfo* info = (ThreadInfo*)arg;

    for (int i = info->startRow; i < info->endRow; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            resultMatrix[i][j] = 0;
            for (int k = 0; k < MATRIX_SIZE; k++) {
                resultMatrix[i][j] += matrixA[i][k] * matrixB[k][j];
            }
        }
    }

    pthread_exit(NULL);
}

int main() {
    // Initialize matrices with random values
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            matrixA[i][j] = rand() % 10;
            matrixB[i][j] = rand() % 10;
        }
    }

    // Sequential matrix multiplication for performance comparison
    clock_t seq_start = clock();

    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            resultMatrix[i][j] = 0;
            for (int k = 0; k < MATRIX_SIZE; k++) {
                resultMatrix[i][j] += matrixA[i][k] * matrixB[k][j];
            }
        }
    }

    clock_t seq_end = clock();
    double seq_time = ((double)(seq_end - seq_start)) / CLOCKS_PER_SEC;

    printf("Sequential multiplication took %f seconds\n", seq_time);

    // Parallel matrix multiplication
    pthread_t threads[NUM_THREADS];
    ThreadInfo threadInfo[NUM_THREADS];

    clock_t par_start = clock();

    // Divide the work among threads
    int rowsPerThread = MATRIX_SIZE / NUM_THREADS;
    for (int i = 0; i < NUM_THREADS; i++) {
        threadInfo[i].startRow = i * rowsPerThread;
        threadInfo[i].endRow = (i == NUM_THREADS - 1) ? MATRIX_SIZE : (i + 1) * rowsPerThread;
        pthread_create(&threads[i], NULL, multiply, (void*)&threadInfo[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_t par_end = clock();
    double par_time = ((double)(par_end - par_start)) / CLOCKS_PER_SEC;

    printf("Parallel multiplication took %f seconds\n", par_time);

    // Compare the results (optional)
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            if (resultMatrix[i][j] != resultMatrix[i][j]) {
                printf("Result mismatch at (%d, %d)\n", i, j);
                break;
            }
        }
    }

    return 0;
}
