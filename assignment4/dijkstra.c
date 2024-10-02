/*
 * In this file, you'll implement Dijkstra's algorithm to find the least
 * expensive paths in the graph defined in `airports.dat`.  Don't forget to
 * include your name and @oregonstate.edu email address below.
 *
 * Name: Salem Demssie
 * Email: demssies@oregonstate.edu
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define DATA_FILE "airports.dat"
#define START_NODE 0

int main(int argc, char const *argv[]) {
	/*
	 * open file and read the first two int: num of nodes, num of edges
	 */
	int n_nodes, n_edges;
	FILE* file = fopen(DATA_FILE, "r");
	if (file == NULL) {
        perror("Error opening data file");
        return EXIT_FAILURE;
    }

	fscanf(file, " %d %d ", &n_nodes, &n_edges);
	
       // initialize the adjacency matrix
    int **graph = malloc(n_nodes * sizeof(int *));
    for (int i = 0; i < n_nodes; i++) {
        graph[i] = malloc(n_nodes * sizeof(int));
        for (int j = 0; j < n_nodes; j++) {
            graph[i][j] = (i == j) ? 0 : INT_MAX;
        }
    }
    
    // read edges and fill the adjacency matrix
    int src, dest, cost;
    for (int i = 0; i < n_edges; i++) {
        fscanf(file, "%d %d %d", &src, &dest, &cost);
        graph[src][dest] = cost;
    }
    fclose(file);
    
    // arrays for Dijkstra's algorithm
    int *distances = malloc(n_nodes * sizeof(int));
    int *previous = malloc(n_nodes * sizeof(int));
    int *visited = calloc(n_nodes, sizeof(int));

    // initialize distances to infinity, and distance to START_NODE to 0
    for (int i = 0; i < n_nodes; i++) {
        distances[i] = INT_MAX;
        previous[i] = -1; // initialize previous node as undefined
    }
    distances[START_NODE] = 0;
    previous[START_NODE] = START_NODE; // start node's previous is itself

    for (int i = 0; i < n_nodes; i++) {
        // find the unvisited node with the smallest distance
        int u = -1;
        for (int j = 0; j < n_nodes; j++) {
            if (!visited[j] && (u == -1 || distances[j] < distances[u])) {
                u = j;
            }
        }

        visited[u] = 1; // Mark the node as visited

        // Update the distance for each neighbor v of u
        for (int v = 0; v < n_nodes; v++) {
            if (graph[u][v] != INT_MAX && distances[u] + graph[u][v] < distances[v]) {
                distances[v] = distances[u] + graph[u][v];
                previous[v] = u; // Update the previous node
            }
        }
    }

    // Print out the least-cost paths and their previous nodes
    for (int i = 0; i < n_nodes; i++) {
        if (distances[i] == INT_MAX) {
            printf("Cost to node %d: unreachable -- Previous node: N/A\n", i);
        } else {
            printf("Cost to node %d: %d -- Previous node: %d\n", i, distances[i], previous[i]);
        }
    }

    // Free allocated memory
    for (int i = 0; i < n_nodes; i++) {
        free(graph[i]);
    }
    free(graph);
    free(distances);
    free(previous);
    free(visited);

    return 0;
}