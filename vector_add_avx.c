#include <immintrin.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void vector_add_avx(float *A, float *B, float *C, int N) {
    int i;
    for (i = 0; i <= N - 8; i += 8) { // Load 8 floats from A and B into AVX registers
        __m256 vecA = _mm256_loadu_ps(&A[i]);
        __m256 vecB = _mm256_loadu_ps(&B[i]);
        __m256 vecC = _mm256_add_ps(vecA, vecB); // Perform the addition in parallel
        _mm256_storeu_ps(&C[i], vecC); // Store the result back into C
    }
    for (; i < N; i++) { // Handle remaining elements (less than 8) if N is not a multiple of 8.
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
    for (int i = 0; i < N; i++) { //// Initialize the vectors
        A[i] = i * 1.0f;
        B[i] = (i + 1) * 1.0f;
    }
    clock_t start = clock(); // Measure the execution time of the SIMD method
    vector_add_avx(A, B, C, N);
    clock_t end = clock();
    double avx_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("SIMD (AVX) execution time: %f seconds\n", avx_time);
    free(A);
    free(B);
    free(C);
    return 0;
}
