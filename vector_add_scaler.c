#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void vector_add_scalar(float *A, float *B, float *C, int N) {
    for (int i = 0; i < N; i++) {
        C[i] = A[i] + B[i];
    }
}

int main() {
    int N = 1024 * 1024;  // Size of the vectors (1 million elements)
    float *A = (float*)malloc(N * sizeof(float));
    float *B = (float*)malloc(N * sizeof(float));
    float *C = (float*)malloc(N * sizeof(float));

    if (A == NULL || B == NULL || C == NULL) {
        printf("Memory allocation failed!\n");
        return 1;  // Exit if memory allocation fails
    }

    // Initialize the vectors
    for (int i = 0; i < N; i++) {
        A[i] = i * 1.0f;
        B[i] = (i + 1) * 1.0f;
    }

    // Measure the execution time of the scalar method
    clock_t start = clock();
    vector_add_scalar(A, B, C, N);
    clock_t end = clock();
    double scalar_time = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Scalar execution time: %f seconds\n", scalar_time);

     // Free allocated memory
    free(A);
    free(B);
    free(C);

    return 0;
}
