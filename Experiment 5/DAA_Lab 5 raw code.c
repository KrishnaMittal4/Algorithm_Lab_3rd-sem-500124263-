#include <stdio.h>
#include <limits.h>

void print_optimal_parens(int , int , int n, int s[n][n], char *);
void matrix_chain_order(int p[], int );
void print_optimal_parens(int i, int j, int n, int s[n][n], char *name) {
    if (i == j) {
        printf("A%c", *name);
        (*name)++;
        return;
    }
    printf("(");
    print_optimal_parens(i, s[i][j], n, s, name);
    print_optimal_parens(s[i][j] + 1, j, n, s, name);
    printf(")");
}


void matrix_chain_order(int p[], int n) {
    int m[n][n];  
    int s[n][n]; 
    for (int i = 1; i < n; i++)
        m[i][i] = 0;

    for (int L = 2; L < n; L++) {
        for (int i = 1; i < n - L + 1; i++) {
            int j = i + L - 1;
            m[i][j] = INT_MAX; 
            for (int k = i; k <= j - 1; k++) {
               
                int q = m[i][k] + m[k + 1][j] + p[i - 1] * p[k] * p[j];
                if (q < m[i][j]) {
                    m[i][j] = q;
                    s[i][j] = k;
                }
            }
        }
    }

    printf("Minimum number of multiplications is: %d\n", m[1][n - 1]);

    printf("Optimal parenthesization: ");
    char name = '1'; 
    print_optimal_parens(1, n - 1, n, s, &name);
    printf("\n");
}


int main() {

    int p[] = {50,35,20,5,10,15}; 
    int n = sizeof(p) / sizeof(p[0]); 

    matrix_chain_order(p, n);

    return 0;
}