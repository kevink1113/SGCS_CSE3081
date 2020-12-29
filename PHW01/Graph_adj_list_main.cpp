#include "Graph_adj_list.h"

extern SLList2 pool;
extern unsigned long UsedMemoryForArray;

int main ( void ) {
	int Vnum, Enum, src;
	vertex *V;
	edge   *E;
	int    Tree_cost; // BFS_tree cost
	int    Tnum;      // # of test cases
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
		if (UsedMemoryForArray < (unsigned long)Memsize_for_array ) {
			UsedMemoryForArray = Memsize_for_array;
		}
		Read_Graph_adj_list(Vnum, V, Enum, E);
		// Do BFS
		start = clock();	// start timer

		Tree_cost = BFS_Tree_adj_list(src, Vnum, V, Enum, E);

		finish = clock();
		cmpt = ((double)(finish - start)) / (double)CLK_TCK;

        if (Tree_Check_adj_list(Vnum, V, Enum, E ) == 0) { // check if spanning tree
			if ( t != 0 ) printf("\n");
			printf("**T%d (BFS) (V = %d, E = %d, time = %.3f sec) ERROR: NOT A TREE!!", 
				t+1, Vnum, Enum, cmpt);
		}
		else {
			if ( t != 0 ) printf("\n");
			printf("**T%d (BFS) (V = %d, E = %d, time = %.3f sec) Tree Cost = %d", 
				t+1, Vnum, Enum, cmpt, Tree_cost);
		}

		Free_Graph_adj_list(Vnum, V); // free adj list
		if (pool.SLL_cnt != 0) {
			printf("ERROR: SLL_cnt must be zero.");
			exit(-1);
		}
		fflush(stdout);
	    delete[] V;	// free Vertex Array V
		delete[] E;   // free Edge Array E
	}
	printf("\nTotal memory used = %d", 
		pool.UsedMemoryForSLLs + UsedMemoryForArray);
	pool.freeSLL_pool( ); // return ptr_Ls in the pool to the system memory

	return 0;
}

void errorExit ( const char *s ) {
  printf("%s\n", s);
  exit(-1);
}