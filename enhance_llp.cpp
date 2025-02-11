#include <iostream>
#include <vector>
#include <chrono>
#include <immintrin.h>

const int N = 512;

// Function to check for loop-carried dependencies
bool hasLoopCarriedDependencies(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N - 1; k++) {
                if (A[i][k] != A[i][k + 1] || B[k][j] != B[k + 1][j]) {
                    return true;
                }
            }
        }
    }
    return false;
}

void matrixMultiplyUnrolled(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B, std::vector<std::vector<int>>& C) {
    if (hasLoopCarriedDependencies(A, B)) { // Dependence Analysis
        std::cerr << "Loop-carried dependencies detected. Cannot apply loop unrolling." << std::endl;
        return;
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0;
            for (int k = 0; k < N; k += 4) { // Loop Unrolling
                C[i][j] += A[i][k] * B[k][j];
                C[i][j] += A[i][k+1] * B[k+1][j];
                C[i][j] += A[i][k+2] * B[k+2][j];
                C[i][j] += A[i][k+3] * B[k+3][j];
            }
        }
    }
}

void matrixMultiplyVectorized(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B, std::vector<std::vector<int>>& C) {
    if (hasLoopCarriedDependencies(A, B)) { // Dependence Analysis
        std::cerr << "Loop-carried dependencies detected. Cannot apply vectorization." << std::endl;
        return;
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            __m256i sum = _mm256_setzero_si256();
            for (int k = 0; k < N; k += 8) { // Vectorization
                __m256i a = _mm256_loadu_si256((__m256i*)&A[i][k]);
                __m256i b = _mm256_loadu_si256((__m256i*)&B[k][j]);
                __m256i prod = _mm256_mullo_epi32(a, b);
                sum = _mm256_add_epi32(sum, prod);
            }
            int temp[8];
            _mm256_storeu_si256((__m256i*)temp, sum);
            C[i][j] = temp[0] + temp[1] + temp[2] + temp[3] + temp[4] + temp[5] + temp[6] + temp[7];
        }
    }
}

int main() {
    std::vector<std::vector<int>> A(N, std::vector<int>(N, 1));
    std::vector<std::vector<int>> B(N, std::vector<int>(N, 2));
    std::vector<std::vector<int>> C(N, std::vector<int>(N, 0));

    auto start = std::chrono::high_resolution_clock::now();
    matrixMultiplyUnrolled(A, B, C);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Unrolled Time taken: " << duration.count() << " seconds" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    matrixMultiplyVectorized(A, B, C);
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "Vectorized Time taken: " << duration.count() << " seconds" << std::endl;

    return 0;
}