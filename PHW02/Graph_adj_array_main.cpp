
#include "Graph_adj_array.h"
#include <queue>

int Tree_Check_adj_array(int Vnum, vertex V[], int Enum, edge E[]);
void Read_Graph_adj_array(int Vnum, vertex V[], int Enum, edge E[]);
int DFS_Tree_adj_array(int src, int Vnum, vertex *V, int Enum, edge *E);

unsigned long usedMemoryForArray;
void errorExit(const char *s);

int main ( void ) {
	int    Vnum, Enum, src;
	vertex *V;
	edge   *E;
	int    Tnum; // # of test cases
	int    Tree_cost; // DFS_tree cost
	int    Memsize_for_array;
	clock_t start, finish;
	double cmpt;

	scanf_s("%d", &Tnum);

	for (int t = 0; t < Tnum; t++ ) {
		scanf_s("%d %d %d", &Vnum, &Enum, &src);
		V = new vertex [Vnum];
		E = new edge [Enum];
		if ( V == NULL || E == NULL ) {
			errorExit("Memory Allocation Error");
		}
		Memsize_for_array = Vnum * sizeof(vertex) + Enum * sizeof(edge);
		if (usedMemoryForArray < (unsigned long)Memsize_for_array ) {
			usedMemoryForArray = Memsize_for_array;
		}
		Read_Graph_adj_array(Vnum, V, Enum, E);
		// Do DFS
		start = clock();

	    for ( int i = 0; i < Vnum; i++ ) {
		  V[i].flag = false;  // clear flag
	    }
	    for ( int i = 0; i < Enum; i++ ) {
		  E[i].flag = false;  // clear flag
	    }
		Tree_cost = DFS_Tree_adj_array (src, Vnum, V, Enum, E);

		finish = clock();
		cmpt = ((double)(finish - start)) / (double)CLK_TCK;

        if (Tree_Check_adj_array(Vnum, V, Enum, E) == 0) { // check if spanning tree
			if ( t != 0 ) printf("\n");
			printf("**T%d (DFS) (V = %d, E = %d, time = %.3f sec) ERROR: NOT A TREE!!", 
				t+1, Vnum, Enum, cmpt);
		}
		else {
			if ( t != 0 ) printf("\n");
			printf("**T%d (DFS) (V = %d, E = %d, time = %.3f sec) Tree Cost = %d", 
				t+1, Vnum, Enum, cmpt, Tree_cost);
		}
		delete V; delete E;
	}
	printf("\nTotal memory used = %d", usedMemoryForArray);
	return 0;
}

void errorExit(const char *s) {
	printf("%s\n", s);
	exit(-1);
}