#include<stdio.h>
#include <stdlib.h>
#include <time.h>

void printMatrix(int** , int );
void fillMatrix(int** , int );
void strassenMultiply(int** , int** , int** , int );
int** allocateMatrix(int );
void freeMatrix(int** , int );
void traditionalMultiply(int** , int** , int** , int );
void addMatrix(int** , int** , int** , int );
void subtractMatrix(int** , int** , int** , int );

int main(){
    int n;
    clock_t end_time,start_time;
    double Traditional_time,Strassen_time;

    printf("Enter the size of the matrix (must be a power of 2): ");
    scanf("%d", &n);

    int** A = allocateMatrix(n);
    int** B = allocateMatrix(n);
    int** C = allocateMatrix(n);

    fillMatrix(A, n);
    fillMatrix(B, n);

    start_time = clock();
    traditionalMultiply(A, B, C, n);
    end_time = clock();
    Traditional_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    printf("Time taken by Traditional Multiplication: %f seconds\n", Traditional_time);

    start_time = clock();
    strassenMultiply(A, B, C, n);
    end_time = clock();
    Strassen_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;


    printf("Time taken by Strassen's Multiplication: %f seconds\n", Strassen_time);

    freeMatrix(A, n);
    freeMatrix(B, n);
    freeMatrix(C, n);

    return 0;
}

int** allocateMatrix(int n) {
    int** matrix = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        matrix[i] = (int*)malloc(n * sizeof(int));
    }
    return matrix;
}

void freeMatrix(int** matrix, int n) {
    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
}


void traditionalMultiply(int** A, int** B, int** C, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = 0;
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void addMatrix(int** A, int** B, int** result, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            result[i][j] = A[i][j] + B[i][j];
        }
    }
}


void subtractMatrix(int** A, int** B, int** result, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            result[i][j] = A[i][j] - B[i][j];
        }
    }
}

void strassenMultiply(int** A, int** B, int** C, int n) {
    if (n <= 64) {  
        traditionalMultiply(A, B, C, n);
        return;
    }

    int newSize = n / 2;

    int** A11 = allocateMatrix(newSize);
    int** A12 = allocateMatrix(newSize);
    int** A21 = allocateMatrix(newSize);
    int** A22 = allocateMatrix(newSize);
    int** B11 = allocateMatrix(newSize);
    int** B12 = allocateMatrix(newSize);
    int** B21 = allocateMatrix(newSize);
    int** B22 = allocateMatrix(newSize);

    int** P1 = allocateMatrix(newSize);
    int** P2 = allocateMatrix(newSize);
    int** P3 = allocateMatrix(newSize);
    int** P4 = allocateMatrix(newSize);
    int** P5 = allocateMatrix(newSize);
    int** P6 = allocateMatrix(newSize);
    int** P7 = allocateMatrix(newSize);

    int** C11 = allocateMatrix(newSize);
    int** C12 = allocateMatrix(newSize);
    int** C21 = allocateMatrix(newSize);
    int** C22 = allocateMatrix(newSize);

    int** tempA = allocateMatrix(newSize);
    int** tempB = allocateMatrix(newSize);


    for (int i = 0; i < newSize; i++) {
        for (int j = 0; j < newSize; j++) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + newSize];
            A21[i][j] = A[i + newSize][j];
            A22[i][j] = A[i + newSize][j + newSize];

            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j + newSize];
            B21[i][j] = B[i + newSize][j];
            B22[i][j] = B[i + newSize][j + newSize];
        }
    }

    addMatrix(A11, A22, tempA, newSize);
    addMatrix(B11, B22, tempB, newSize);
    strassenMultiply(tempA, tempB, P1, newSize); 
    addMatrix(A21, A22, tempA, newSize);
    strassenMultiply(tempA, B11, P2, newSize);  

    subtractMatrix(B12, B22, tempB, newSize);
    strassenMultiply(A11, tempB, P3, newSize);  

    subtractMatrix(B21, B11, tempB, newSize);
    strassenMultiply(A22, tempB, P4, newSize);  

    addMatrix(A11, A12, tempA, newSize);
    strassenMultiply(tempA, B22, P5, newSize);  
    subtractMatrix(A21, A11, tempA, newSize);
    addMatrix(B11, B12, tempB, newSize);
    strassenMultiply(tempA, tempB, P6, newSize); 
    subtractMatrix(A12, A22, tempA, newSize);
    addMatrix(B21, B22, tempB, newSize);
    strassenMultiply(tempA, tempB, P7, newSize); 
    addMatrix(P1, P4, tempA, newSize);
    subtractMatrix(tempA, P5, tempB, newSize);
    addMatrix(tempB, P7, C11, newSize);  

    addMatrix(P3, P5, C12, newSize);  

    addMatrix(P2, P4, C21, newSize); 

    addMatrix(P1, P3, tempA, newSize);
    subtractMatrix(tempA, P2, tempB, newSize);
    addMatrix(tempB, P6, C22, newSize); 

    for (int i = 0; i < newSize; i++) {
        for (int j = 0; j < newSize; j++) {
            C[i][j] = C11[i][j];
            C[i][j + newSize] = C12[i][j];
            C[i + newSize][j] = C21[i][j];
            C[i + newSize][j + newSize] = C22[i][j];
        }
    }


    freeMatrix(A11, newSize); freeMatrix(A12, newSize);
    freeMatrix(A21, newSize); freeMatrix(A22, newSize);
    freeMatrix(B11, newSize); freeMatrix(B12, newSize);
    freeMatrix(B21, newSize); freeMatrix(B22, newSize);
    freeMatrix(P1, newSize); freeMatrix(P2, newSize);
    freeMatrix(P3, newSize); freeMatrix(P4, newSize);
    freeMatrix(P5, newSize); freeMatrix(P6, newSize);
    freeMatrix(P7, newSize);
    freeMatrix(C11, newSize); freeMatrix(C12, newSize);
    freeMatrix(C21, newSize); freeMatrix(C22, newSize);
    freeMatrix(tempA, newSize); freeMatrix(tempB, newSize);
}
void fillMatrix(int** matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = rand() % 10;
        }
    }
}