#include <stdio.h>

#define MAX 100

void printSubset(int subset[], int size);
void sumOfSubsets(int set[], int subset[], int n, int subsetSize, int total, int nodeIndex, int targetSum, int* found);

// Function to print a solution (a valid subset)
void printSubset(int subset[], int size) {
    printf("{ ");
    for (int i = 0; i < size; i++) {
        printf("%d ", subset[i]);
    }
    printf("}\n");
}

// Recursive function to find subsets with sum equal to targetSum
void sumOfSubsets(int set[], int subset[], int n, int subsetSize, int total, int nodeIndex, int targetSum, int* found) {
    // If the total is equal to targetSum, we found a solution
    if (total == targetSum) {
        printSubset(subset, subsetSize);
        *found = 1;  // Set found to true
        return;
    }

    // If total exceeds targetSum or we've explored all elements, return
    if (total > targetSum || nodeIndex >= n) {
        return;
    }

    // Include the current element in the subset and recurse
    subset[subsetSize] = set[nodeIndex];
    sumOfSubsets(set, subset, n, subsetSize + 1, total + set[nodeIndex], nodeIndex + 1, targetSum, found);

    // Exclude the current element from the subset and recurse
    sumOfSubsets(set, subset, n, subsetSize, total, nodeIndex + 1, targetSum, found);
}

int main() {
    int n, targetSum;
    int set[MAX], subset[MAX];
    int found = 0;  // Flag to indicate if a solution is found

    // Input the number of elements in the set
    printf("Enter the number of elements in the set: ");
    scanf("%d", &n);

    // Input the elements of the set
    printf("Enter the elements of the set:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &set[i]);
    }

    // Input the target sum
    printf("Enter the target sum: ");
    scanf("%d", &targetSum);

    printf("Subsets that sum to %d are:\n", targetSum);
    // Call the recursive function to find all subsets that sum to targetSum
    sumOfSubsets(set, subset, n, 0, 0, 0, targetSum, &found);

    // If no subset found, print a message
    if (!found) {
        printf("No solution found.\n");
    }

    return 0;
}
