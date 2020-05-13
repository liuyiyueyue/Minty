/**
 * Using Adjacency Matrix Representation of Graph
 * 
 */

#include <stdio.h>
#include <stdlib.h>

#define NODENUM 12

graphinfo *init_graph();
void add_edge(int **adjacency_matrix, int node1, int node2);
void print_matrix(int **adjacency_matrix);

typedef struct {
    int size;	// number of nodes
    int *vertices;
    int **adjacency_matrix;
    int *vertix_weight;

} graphinfo;

graphinfo *info = init_graph();
int **graph = info->adjacency_matrix; // user input
int *vertix_weight = info->vertix_weight;



/**
 * Algorithm: compute using Minty
 * 
 */
int *
compute() {
	int i, j, k;
	Boolean stop = true;

	// number of white augmenting paths for each iter.
	int num = 1;

	/* Step 1: Initialize an empty stable set S. */
	int S[NODENUM];
	for (i = 0; i < NODENUM ; i++) {
		S[i] = 0; // 0 is unstable/white
	}

	/* Step 2 & 3 */
	while (num > 0) {
		num = 0;
		int max_weight;
		int *P = calloc(0, sizeof(int) * NODENUM); // path

		/* Step 2 */
		// 2-1: Generate all white alternating paths of length 0 or 2
		for (i = 0; i < NODENUM; i++) { // length 0
			if (S[i] == 0) { // is white node
				if (i == 0) {
					max_weight = vertix_weight[i];
					P[i] = 1;
					num++;
					if (vertix_weight[i] > 0) stop = false;

				} else {
					if (max_weight < vertix_weight[i]) {
						max_weight = vertix_weight[i];
						int *temp = P;
						P = calloc(0, sizeof(int) * NODENUM);
						P[i] = 1;
						free(temp);
					}
					num++;
					if (vertix_weight[i] > 0) stop = false;
				}
			}
		} // end of for loop

		for (i = 0; i < NODENUM ; i++) { // length 2: might generate duplicated paths; but doesn't matter.
			int reach[NODENUM];
			if (S[i] == 1) continue; // start node should be white
			reach[i] = 1;

			for (j = 0; j < NODENUM ; j++) {
				if (graph[i][j] == 0 || S[j] == 0 || reach[j]) continue; // middle node should be black
				reach[j] = 1;

				for (k = 0; k < NODENUM; k++) {
					if (graph[j][k] == 0 || S[k] == 1 || reach[k]) continue; // end node should be white
					reach[k] = 1;

					int temp_weight = vertix_weight[i] - vertix_weight[j] + vertix_weight[k];

					if (max_weight < temp_weight) {
						int *temp = P;
						P = calloc(0, sizeof(int) * NODENUM);
						P[i] = 1;
						P[j] = 1;
						P[k] = 1;
						free(temp);
						max_weight = temp_weight;
					}
					num++;	
					if (temp_weight > 0) stop = false;
				}		
			}
		} // end of for loop

		// 2-2: for each pair of non-adjacent free vertices a and b:
		int a, b, xa, xb;
		for (a = 0; a < NODENUM ; a++) {
			if (S[a] == 1) continue;

			for (b = 0; b < NODENUM ; b++) {
				if (S[b] == 1 && graph[a][b] == 1) xa = b; continue;
				if (S[b] == 1 || graph[a][b] == 1) continue;

				for (i = 0; i < NODENUM ; i++) {
					if (S[i] == 1 && graph[b][i] == 1) { 
						xb = i;
						break;
					}
				}

				if (xa != xb) {
					// TODO: hardest part...
					// find a maximum weight white alternating path between a and b
					// find all white alternating paths and then filter out the max is inefficient..
					// maybe convert to min weight problem and then use Dijkstra's algo...
					int *path;
					
					int temp_weight = findMWWAP(a, b, vertix_weight, path);

					if (max_weight < temp_weight) {
						int *temp = P;
						P = path;
						free(temp);
						max_weight = temp_weight;
					}

					num++;
					if (temp_weight > 0) stop = false;
				}
			}
		} // end of for loop

		// 2-7: nonpositive weight?
		if (stop == true) return (S);


		/* Step 3: symmetric difference between S and P */
		for (i = 0; i < NODENUM; i++) {			
			if (P[i] == 1 && S[i] == 0)
				S[i] == 1;
			else if (P[i] == 1 && S[i] == 1)
				S[i] == 0;		
		}

	} // end of while loop

	/* Output S: if there exists no white augmenting path of S. i.e. num = 0 */
	return (S);
}


static int
findMWWAP(int a, int b, int *vertix_weight, int *path) {
	// TODO:
}

/**
 * initialize the graph; 
 * adjacency_matrix all sets to 0
 */ 
graphinfo *
init_graph() {
	// dummy initialization.
	int i, j;
	graphinfo *g = malloc(sizeof(graphinfo));

	g->size = NODENUM;
	g->vertices = malloc(sizeof(int) * g->size);
	g->adjacency_matrix = malloc(sizeof(int *) * g->size);

	for(i = 0; i < g->size; i++){
		g->adjacency_matrix[i] = malloc(sizeof(int) * g->size);

		for (j = 0; j < g->size; j++){
			g->adjacency_matrix[i][j] = 0;
		}
	}

	return (g);
}

/**
 * add edge between two vertexes
 * assume non-directed graph
 * 
 */ 
void 
add_edge(int **adjacency_matrix, int node1, int node2) {
	
	adjacency_matrix[node1][node2] = 1;
	adjacency_matrix[node2][node1] = 1; 
}

/**
 * Print the adjacency matrix
 */ 
void
print_matrix(int **adjacency_matrix) {
	int row, col;

	for (row = 0; row < NODENUM; row++) {
		for(col = 0; col < NODENUM; col++) {
			printf("%d\t", adjacency_matrix[row][col]);
		}
	printf("\n");
	}
}



int
main(void) {
	printf("Inside main...\n");

	struct graph *g = init_graph();
	add_edge(g->adjacency_matrix, 1, 2);
	print_matrix(g->adjacency_matrix);

	return (0);
}
