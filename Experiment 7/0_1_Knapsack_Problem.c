#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Structure to represent an item with value and weight
struct Item {
    int value;
    int weight;
};

// Structure to represent a selected item
struct SelectedItem {
    int index;
    int value;
    int weight;
};

int compare(const void* a, const void* b);
int greedyKnapsack(struct Item items[], int n, int W, struct SelectedItem selected[], int *selectedCount) ;
int dpKnapsack(struct Item items[], int n, int W, struct SelectedItem selected[], int *selectedCount);
void printSelectedItems(struct SelectedItem selected[], int count, const char* approach);

// Comparator function to sort items by descending value-to-weight ratio
int compare(const void* a, const void* b) {
    struct Item* item1 = (struct Item*)a;
    struct Item* item2 = (struct Item*)b;
    double ratio1 = (double)item1->value / item1->weight;
    double ratio2 = (double)item2->value / item2->weight;
    if (ratio1 < ratio2)
        return 1;
    else if (ratio1 > ratio2)
        return -1;
    else
        return 0;
}

// ==================== Greedy Approach ====================
// Function to implement the greedy approach for 0/1 Knapsack
int greedyKnapsack(struct Item items[], int n, int W, struct SelectedItem selected[], int *selectedCount) {
    // Sort items by descending value-to-weight ratio
    struct Item sortedItems[n];
    for(int i=0; i<n; i++) sortedItems[i] = items[i];
    qsort(sortedItems, n, sizeof(struct Item), compare);

    int totalValue = 0;
    int totalWeight = 0;
    *selectedCount = 0;

    // Select items based on sorted order
    for (int i = 0; i < n; i++) {
        if (totalWeight + sortedItems[i].weight <= W) {
            totalWeight += sortedItems[i].weight;
            totalValue += sortedItems[i].value;
            selected[*selectedCount].index = i + 1; // 1-based indexing
            selected[*selectedCount].value = sortedItems[i].value;
            selected[*selectedCount].weight = sortedItems[i].weight;
            (*selectedCount)++;
        }
    }

    return totalValue; // Total value obtained by Greedy approach
}

// ==================== Dynamic Programming Approach ====================
// Function to implement the DP approach for 0/1 Knapsack and track selected items
int dpKnapsack(struct Item items[], int n, int W, struct SelectedItem selected[], int *selectedCount) {
    // Create a DP table where dp[i][w] represents the maximum value for the first i items and weight limit w
    int dp[n+1][W+1];

    // Build the table in bottom-up manner
    for (int i = 0; i <= n; i++) {
        for (int w = 0; w <= W; w++) {
            if (i == 0 || w == 0)
                dp[i][w] = 0;
            else if (items[i-1].weight <= w)
                dp[i][w] = (dp[i-1][w] > dp[i-1][w - items[i-1].weight] + items[i-1].value) ?
                           dp[i-1][w] : dp[i-1][w - items[i-1].weight] + items[i-1].value;
            else
                dp[i][w] = dp[i-1][w];
        }
    }

    // Now, find which items to include by tracing back the DP table
    int res = dp[n][W];
    int w = W;
    *selectedCount = 0;

    for(int i = n; i > 0 && res > 0; i--) {
        // If the value comes from the top (not including this item), skip it
        if(res == dp[i-1][w])
            continue;
        else {
            // This item is included
            selected[*selectedCount].index = i;
            selected[*selectedCount].value = items[i-1].value;
            selected[*selectedCount].weight = items[i-1].weight;
            (*selectedCount)++;
            // Subtract the value and weight of the included item
            res -= items[i-1].value;
            w -= items[i-1].weight;
        }
    }

    return dp[n][W]; // Maximum value that can be obtained
}

// ==================== Utility Function ====================
// Function to print the selected items
void printSelectedItems(struct SelectedItem selected[], int count, const char* approach) {
    printf("Items selected by %s approach:\n", approach);
    for(int i = count - 1; i >=0; i--) { // Reverse order for better readability
        printf("Item %d: Value = %d, Weight = %d\n", selected[i].index, selected[i].value, selected[i].weight);
    }
    printf("\n");
}

// ==================== Main Function ====================
int main() {
    // Define five different datasets

    // ----- Dataset 1: Greedy fails to find optimal solution -----
    struct Item items1[] = {
        {60, 10},
        {100, 20},
        {120, 30}
    };
    int n1 = sizeof(items1) / sizeof(items1[0]);
    int W1 = 50;

    // ----- Dataset 2: Greedy fails to find optimal solution -----
    struct Item items2[] = {
        {20, 10},
        {30, 20},
        {45, 30}
    };
    int n2 = sizeof(items2) / sizeof(items2[0]);
    int W2 = 50;

    // ----- Dataset 3: Greedy fails to find optimal solution -----
    struct Item items3[] = {
        {15, 5},
        {10, 4},
        {9, 3}
    };
    int n3 = sizeof(items3) / sizeof(items3[0]);
    int W3 = 7;

    // ----- Dataset 4: Greedy fails to find optimal solution -----
    struct Item items4[] = {
        {40, 20},
        {50, 20},
        {100, 40}
    };
    int n4 = sizeof(items4) / sizeof(items4[0]);
    int W4 = 60;

    // ----- Dataset 5: Greedy fails to find optimal solution -----
    struct Item items5[] = {
        {10, 2},
        {40, 20},
        {30, 10},
        {50, 30}
    };
    int n5 = sizeof(items5) / sizeof(items5[0]);
    int W5 = 50;

    // Array of datasets
    struct Item* datasets[] = {items1, items2, items3, items4, items5};
    int sizes[] = {n1, n2, n3, n4, n5};
    int capacities[] = {W1, W2, W3, W4, W5};
    int numDatasets = 5;

    // Iterate through each dataset and compare Greedy and DP approaches
    for(int d = 0; d < numDatasets; d++) {
        printf("=============================================\n");
        printf("Dataset %d:\n", d+1);
        printf("---------------------------------------------\n");
        printf("Number of items: %d\n", sizes[d]);
        printf("Knapsack Capacity: %d\n", capacities[d]);
        printf("\nItems:\n");
        printf("Index\tValue\tWeight\tValue/Weight\n");
        for(int i = 0; i < sizes[d]; i++) {
            double ratio = (double)datasets[d][i].value / datasets[d][i].weight;
            printf("%d\t%d\t%d\t%.2lf\n", i+1, datasets[d][i].value, datasets[d][i].weight, ratio);
        }
        printf("\n");

        // Greedy Approach
        struct SelectedItem selectedGreedy[sizes[d]];
        int selectedCountGreedy = 0;
        int greedyValue = greedyKnapsack(datasets[d], sizes[d], capacities[d], selectedGreedy, &selectedCountGreedy);
        printf("Greedy Approach:\n");
        printf("Total Value Obtained: %d\n", greedyValue);
        printSelectedItems(selectedGreedy, selectedCountGreedy, "Greedy");

        // Dynamic Programming Approach
        struct SelectedItem selectedDP[sizes[d]];
        int selectedCountDP = 0;
        int dpValue = dpKnapsack(datasets[d], sizes[d], capacities[d], selectedDP, &selectedCountDP);
        printf("Dynamic Programming Approach:\n");
        printf("Total Value Obtained: %d\n", dpValue);
        printSelectedItems(selectedDP, selectedCountDP, "Dynamic Programming");

        // Comparison
        if(greedyValue == dpValue)
            printf("Result: Greedy approach matches Dynamic Programming approach.\n");
        else
            printf("Result: Greedy approach does NOT match Dynamic Programming approach.\n");
        printf("=============================================\n\n");
    }

    return 0;
}
