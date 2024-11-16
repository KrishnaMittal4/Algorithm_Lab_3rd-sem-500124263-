#include <stdio.h>  
#include <stdlib.h>  
#include <limits.h>  
#include <time.h>  

struct Edge {  
    int src, dest, weight;  
};  

struct Graph {  
    int V, E;  
    struct Edge* edges;  
};  

// Function to create a graph with V vertices and E edges  
struct Graph* createGraph(int V, int E) {  
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));  
    graph->V = V;  
    graph->E = E;  
    graph->edges = (struct Edge*)malloc(E * sizeof(struct Edge));  
    return graph;  
}  

// Function to generate random edges for the graph  
void generateRandomEdges(struct Graph* graph) {  
    int edgeCount = 0;  
    while (edgeCount < graph->E) {  
        int src = rand() % graph->V;  
        int dest = rand() % graph->V;  
        int weight = rand() % 10 + 1; // Random weight between 1 and 10  

        // Avoid self-loops and duplicate edges  
        if (src != dest) {  
            int duplicate = 0;  
            for (int i = 0; i < edgeCount; i++) {  
                if ((graph->edges[i].src == src && graph->edges[i].dest == dest) ||  
                    (graph->edges[i].src == dest && graph->edges[i].dest == src)) {  
                    duplicate = 1;  
                    break;  
                }  
            }  
            if (!duplicate) {  
                graph->edges[edgeCount++] = (struct Edge){src, dest, weight};  
            }  
        }  
    }  
}  

// Function to implement Dijkstra's algorithm  
void dijkstra(int V, int src, int** graph) {  
    int* dist = (int*)malloc(V * sizeof(int));  
    int* visited = (int*)malloc(V * sizeof(int));  
    
    for (int i = 0; i < V; i++) {  
        dist[i] = INT_MAX;  
        visited[i] = 0;  
    }  
    dist[src] = 0;  

    for (int count = 0; count < V - 1; count++) {  
        int minIndex = -1;  
        int minDist = INT_MAX;  
        for (int v = 0; v < V; v++) {  
            if (!visited[v] && dist[v] <= minDist) {  
                minDist = dist[v];  
                minIndex = v;  
            }  
        }  
        visited[minIndex] = 1;  

        for (int v = 0; v < V; v++) {  
            if (!visited[v] && graph[minIndex][v] && dist[minIndex] != INT_MAX  
                && dist[minIndex] + graph[minIndex][v] < dist[v]) {  
                dist[v] = dist[minIndex] + graph[minIndex][v];  
            }  
        }  
    }  

    free(dist);  
    free(visited);  
}  

// Function to implement Bellman-Ford algorithm  
int bellmanFord(struct Graph* graph, int src) {  
    int V = graph->V;  
    int E = graph->E;  
    int* dist = (int*)malloc(V * sizeof(int));  
    
    for (int i = 0; i < V; i++) {  
        dist[i] = INT_MAX;  
    }  
    dist[src] = 0;  

    for (int i = 1; i <= V - 1; i++) {  
        for (int j = 0; j < E; j++) {  
            int u = graph->edges[j].src;  
            int v = graph->edges[j].dest;  
            int weight = graph->edges[j].weight;  
            if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {  
                dist[v] = dist[u] + weight;  
            }  
        }  
    }  

    for (int j = 0; j < E; j++) {  
        int u = graph->edges[j].src;  
        int v = graph->edges[j].dest;  
        int weight = graph->edges[j].weight;  
        if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {  
            free(dist);  
            return 0; // Negative cycle found  
        }  
    }  
    
    free(dist);  
    return 1; // No negative cycle  
}  

void measure_performance(int V, int E) {  
    struct Graph* edgeGraph = createGraph(V, E);  
    generateRandomEdges(edgeGraph);  

    // Create a dense graph representation for Dijkstra's algorithm  
    int** graph = (int**)malloc(V * sizeof(int*));  
    for (int i = 0; i < V; i++) {  
        graph[i] = (int*)calloc(V, sizeof(int));  
    }  

    for (int i = 0; i < edgeGraph->E; i++) {  
        int u = edgeGraph->edges[i].src;  
        int v = edgeGraph->edges[i].dest;  
        graph[u][v] = edgeGraph->edges[i].weight; // Store edge weight in the adjacency matrix  
    }  

    // Measure Dijkstra's algorithm performance  
    double total_dijkstra_time = 0.0;  
    for (int trial = 0; trial < 10; trial++) { // Run 10 trials  
        clock_t start = clock();  
        dijkstra(V, 0, graph);  
        clock_t end = clock();  
        total_dijkstra_time += ((double)(end - start)) / CLOCKS_PER_SEC;  
    }  
    double avg_dijkstra_time = total_dijkstra_time / 10;  

    // Measure Bellman-Ford algorithm performance  
    double total_bellman_time = 0.0;  
    for (int trial = 0; trial < 10; trial++) { // Run 10 trials  
        clock_t start = clock();  
        bellmanFord(edgeGraph, 0);  
        clock_t end = clock();  
        total_bellman_time += ((double)(end - start)) / CLOCKS_PER_SEC;  
    }  
    double avg_bellman_time = total_bellman_time / 10;  

    // Output performance results  
    printf("For graph with %d vertices and %d edges:\n", V, E);  
    printf("Average Dijkstra time: %f seconds\n", avg_dijkstra_time);  
    printf("Average Bellman-Ford time: %f seconds\n\n", avg_bellman_time);  

    // Free allocated memory  
    for (int i = 0; i < V; i++) {  
        free(graph[i]);  
    }  
    free(graph);  
    free(edgeGraph->edges);  
    free(edgeGraph);  
}  

int main() {  
    srand((unsigned int)time(NULL)); // Seed the random number generator  
    int sizes[4][2] = {{500, 2000}, {1000, 5000}, {1500, 7000}, {2000, 10000}}; // Adjust sizes for larger graphs  
    for (int i = 0; i < 4; i++) {  
        measure_performance(sizes[i][0], sizes[i][1]);  
    }  
    return 0;  
}