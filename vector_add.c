#include <stdio.h>
#define VECTOR_SIZE 1000

void vector_addition(int *a, int *b, int *c, int size) {
    for (int i = 0; i < size; ++i) {
        c[i] = a[i] + b[i];
    }
}

int main() {
    int a[VECTOR_SIZE], b[VECTOR_SIZE], c[VECTOR_SIZE];
    
    for (int i = 0; i < VECTOR_SIZE; ++i) {
        a[i] = i;
        b[i] = VECTOR_SIZE - i;
    }
    
    vector_addition(a, b, c, VECTOR_SIZE);
    
    for (int i = 0; i < 10; ++i) { // Print the first 10 elements
        printf("%d ", c[i]);
    }
    printf("\n");
    
    return 0;
}