#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct Node {
    int key;
    struct Node* left, *right;
};

struct Node* newNode(int item) {
    struct Node* temp = (struct Node*)malloc(sizeof(struct Node));
    temp->key = item;
    temp->left = temp->right = NULL;
    return temp;
}

struct Node* insertRecursive(struct Node* node, int key) {
  
    if (node == NULL) return newNode(key);

    if (key < node->key)
        node->left = insertRecursive(node->left, key);
    else if (key > node->key)
        node->right = insertRecursive(node->right, key);

    
    return node;
}

struct Node* insertIterative(struct Node* root, int key) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->key = key;
    newNode->left = newNode->right = NULL;

    if (root == NULL) {
        return newNode;
    }

    struct Node* parent = NULL;
    struct Node* current = root;

    while (current != NULL) {
        parent = current;
        if (key < current->key)
            current = current->left;
        else if (key > current->key)
            current = current->right;
        else
            return root; 
    }

    if (key < parent->key)
        parent->left = newNode;
    else
        parent->right = newNode;

    return root;
}


double measureTime(void (*insertFunc)(struct Node**, int), struct Node** root, int key) {
    clock_t start, end;
    start = clock();
    insertFunc(root, key);  // Call insertion function
    end = clock();
    return ((double)(end - start)) / CLOCKS_PER_SEC;
}

void recursiveInsertWrapper(struct Node** root, int key) {
    *root = insertRecursive(*root, key);
}

void iterativeInsertWrapper(struct Node** root, int key) {
    *root = insertIterative(*root, key);
}

int main() {
   
    int sizes[] = {5000, 10000, 15000, 20000, 25000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    int j;

    for ( j = 0; j < num_sizes; j++) {
        struct Node* root1 = NULL;
        struct Node* root2 = NULL;
         int n = sizes[j]; 
        
        double time_recursive = 0.0;
        for (int i = 0; i < n; i++) {
            time_recursive += measureTime(recursiveInsertWrapper, &root1, i);
        }

        
        double time_iterative = 0.0;
        for (int i = 0; i < n; i++) {
            time_iterative += measureTime(iterativeInsertWrapper, &root2, i);
        }

       
        printf("For size %d:\n", n);
        printf("Time taken for recursive insertion: %f seconds\n", time_recursive);
        printf("Time taken for iterative insertion: %f seconds\n", time_iterative);
        printf("--------------------------\n");
    }

    return 0;
}
