#include <stdlib.h>  
#include <time.h>  
#include <stdio.h> // Include stdio for printf

void merge(int arr[], int left, int mid, int right) {  
    int i, j, k;  
    int n1 = mid - left + 1;  
    int n2 = right - mid;  

    int* L = (int*)malloc(n1 * sizeof(int));  
    int* R = (int*)malloc(n2 * sizeof(int));  

    for (i = 0; i < n1; i++)  
        L[i] = arr[left + i];  
    for (j = 0; j < n2; j++)  
        R[j] = arr[mid + 1 + j];  

    i = 0;  
    j = 0;  
    k = left;  

    while (i < n1 && j < n2) {  
        if (L[i] <= R[j]) {  
            arr[k++] = L[i++];  
        } else {  
            arr[k++] = R[j++];  
        }  
    }  

    while (i < n1)  
        arr[k++] = L[i++];  

    while (j < n2)  
        arr[k++] = R[j++];  

    free(L);  
    free(R);  
}  

void mergeSort(int arr[], int left, int right) {  
    if (left < right) {  
        int mid = left + (right - left) / 2;  

        mergeSort(arr, left, mid);  
        mergeSort(arr, mid + 1, right);  
        merge(arr, left, mid, right);  
    }  
}  

void swap(int* a, int* b) {  
    int t = *a;  
    *a = *b;  
    *b = t;  
}  

int partition(int arr[], int low, int high) {  
    int pivot = arr[high];  
    int i = (low - 1);  

    for (int j = low; j <= high - 1; j++) {  
        if (arr[j] < pivot) {  
            i++;  
            swap(&arr[i], &arr[j]);  
        }  
    }  
    swap(&arr[i + 1], &arr[high]);  
    return (i + 1);  
}  

void quickSort(int arr[], int low, int high) {  
    if (low < high) {  
        int pi = partition(arr, low, high);  
        quickSort(arr, low, pi - 1);  
        quickSort(arr, pi + 1, high);  
    }  
}  

void generateRepeatedArray(int arr[], int size, int value) {  
    for (int i = 0; i < size; i++) {  
        arr[i] = value;  
    }  
}  

int main() {  
    const int NUM_SORTS = 1000;  
    const int SIZES[] = {5000,10000,15000,20000,25000};  
    const int NUM_SIZES = sizeof(SIZES) / sizeof(SIZES[0]);  

    for (int sizeIndex = 0; sizeIndex < NUM_SIZES; sizeIndex++) {  
        int SIZE = SIZES[sizeIndex];  
        int* arr1 = (int*)malloc(SIZE * sizeof(int));  
        int* arr2 = (int*)malloc(SIZE * sizeof(int));  
     generateRepeatedArray(arr1, SIZE, 42);  
        
        printf("Array Size: %d\n", SIZE);  
        double mergeTime = 0.0;  
        double quickTime = 0.0;  

        for (int k = 0; k < NUM_SORTS; k++) {  
          
            for (int j = 0; j < SIZE; j++) {  
                arr2[j] = arr1[j];  
            }  

            clock_t start = clock();  
            mergeSort(arr1, 0, SIZE - 1);  
            clock_t end = clock();  
            mergeTime += ((double)(end - start)) / CLOCKS_PER_SEC;  

         
            for (int j = 0; j < SIZE; j++) {  
                arr2[j] = arr1[j];  
            }  

            start = clock();  
            quickSort(arr2, 0, SIZE - 1);  
            end = clock();  
            quickTime += ((double)(end - start)) / CLOCKS_PER_SEC;  
        }  

        mergeTime /= NUM_SORTS;  
        quickTime /= NUM_SORTS;  

        printf("Merge Sort Time: %f seconds\n", mergeTime);  
        printf("Quick Sort Time: %f seconds\n", quickTime);  
        printf("\n");   

        free(arr1);  
        free(arr2);  
    }  

    return 0;  
}