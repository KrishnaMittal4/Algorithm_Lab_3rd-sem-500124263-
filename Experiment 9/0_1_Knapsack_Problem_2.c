#include <stdio.h>  
#include <stdlib.h>  
#include <time.h>  

// Maximum function  
int max(int a, int b) { return (a > b) ? a : b; }  

//------------------------  
// Backtracking Approach  
//------------------------  
int knapsack_backtracking(int W, int wt[], int val[], int n) {  
    if (n == 0 || W == 0)  
        return 0;  
    if (wt[n-1] > W)  
        return knapsack_backtracking(W, wt, val, n-1);  
    else  
        return max(val[n-1] + knapsack_backtracking(W - wt[n-1], wt, val, n-1),  
                   knapsack_backtracking(W, wt, val, n-1));  
}  

//------------------------  
// Branch & Bound Approach  
//------------------------  
typedef struct {  
    int level;  
    int profit;  
    int weight;  
    float bound;  
} Node;  

float bound(Node u, int n, int W, int wt[], int val[]) {  
    if (u.weight >= W)  
        return 0;  
    float profit_bound = u.profit;  
    int j = u.level + 1;  
    int totweight = u.weight;  

    while (j < n && totweight + wt[j] <= W) {  
        totweight += wt[j];  
        profit_bound += val[j];  
        j++;  
    }  

    if (j < n)  
        profit_bound += (W - totweight) * (float)val[j] / wt[j];  

    return profit_bound;  
}  

int knapsack_branch_and_bound(int W, int wt[], int val[], int n) {  
    Node queue[1000];  
    int front = 0, rear = 0;  
    Node u, v;  
    u.level = -1;  
    u.profit = 0;  
    u.weight = 0;  
    u.bound = bound(u, n, W, wt, val);  
    queue[rear++] = u;  
    int max_profit = 0;  

    while (front < rear) {  
        u = queue[front++];  
        if (u.level == n - 1)  
            continue;  
        v.level = u.level + 1;  
        
        // Taking the item  
        v.weight = u.weight + wt[v.level];  
        v.profit = u.profit + val[v.level];  
        v.bound = bound(v, n, W, wt, val);  
        if (v.weight <= W && v.profit > max_profit)  
            max_profit = v.profit;  
        if (v.bound > max_profit)  
            queue[rear++] = v;  

        // Not taking the item  
        v.weight = u.weight;  
        v.profit = u.profit;  
        v.bound = bound(v, n, W, wt, val);  
        if (v.bound > max_profit)  
            queue[rear++] = v;  
    }  

    return max_profit;  
}  

//------------------------  
// Dynamic Programming Approach  
//------------------------  
int knapsack_dp(int W, int wt[], int val[], int n) {  
    int dp[n+1][W+1];  
    for (int i = 0; i <= n; i++) {  
        for (int w = 0; w <= W; w++) {  
            if (i == 0 || w == 0)  
                dp[i][w] = 0;  
            else if (wt[i-1] <= w)  
                dp[i][w] = max(val[i-1] + dp[i-1][w - wt[i-1]], dp[i-1][w]);  
            else  
                dp[i][w] = dp[i-1][w];  
        }  
    }  
    return dp[n][W];  
}  

//------------------------  
// Main function to test all approaches  
//------------------------  
int main() {  
    int val[] = {240, 400, 480, 560, 600, 800};  
    int wt[] = {40, 80, 120, 160, 200, 240};  
    int capacities[] = {420, 840, 1660}; // The knapsack capacities we want to test  
    int n = sizeof(val) / sizeof(val[0]);  
    int repetitions = 10000;  // Number of repetitions for averaging time  

    for (int idx = 0; idx < 3; idx++) {  
        int W = capacities[idx];  

        // Backtracking Approach  
        clock_t start = clock();  
        int result_backtracking = 0;  
        for (int i = 0; i < repetitions; i++) {  
            result_backtracking = knapsack_backtracking(W, wt, val, n);  
        }  
        clock_t end = clock();  
        printf("Backtracking Result for W=%d: %d, Time: %lf ms\n", W, result_backtracking,   
               (double)(end - start) * 1000 / (CLOCKS_PER_SEC * repetitions));  

        // Branch & Bound Approach  
        start = clock();  
        int result_branch_bound = 0;  
        for (int i = 0; i < repetitions; i++) {  
            result_branch_bound = knapsack_branch_and_bound(W, wt, val, n);  
        }  
        end = clock();  
        printf("Branch & Bound Result for W=%d: %d, Time: %lf ms\n", W, result_branch_bound,   
               (double)(end - start) * 1000 / (CLOCKS_PER_SEC * repetitions));  

        // Dynamic Programming Approach  
        start = clock();  
        int result_dp = 0;  
        for (int i = 0; i < repetitions; i++) {  
            result_dp = knapsack_dp(W, wt, val, n);  
        }  
        end = clock();  
        printf("Dynamic Programming Result for W=%d: %d, Time: %lf ms\n", W, result_dp,   
               (double)(end - start) * 1000 / (CLOCKS_PER_SEC * repetitions));  
    }  

    return 0;  
}