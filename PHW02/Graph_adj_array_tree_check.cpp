#include "Graph_adj_array.h"
//#include "Graph_adj_array_extern.h"
//#include "Graph_adj_array_function.h"
#include <queue>

#define PARENT_OF_ROOT  -100

int Tree_Check_adj_array(int Vnum, vertex V[], int Enum, edge E[]) {
	int u = 0, ep, w;
	int *visited;
	std::queue<int> q;
	for ( int i = 0; i < Enum; i++ ) {
		if ( E[i].flag == true ) {
			++u;
		}
	}
	if ( u != (Vnum-1) ) {
		return 0; // it is not a spanning tree
	}
	visited = new int[Vnum];  // allocate parent array
	for (int i=0; i < Vnum; i++ )
		visited[i] = NONE;
	u = 0;
	visited[u] = PARENT_OF_ROOT;
 	do {
		// check adjacent list at v0 and set Costi_Ni
		for ( ep = V[u].f_hd; ep != NONE; ep = E[ep].fp ) { // forward
			if ( E[ep].flag == false ) {
				continue;
			}
			w = E[ep].vr;
			if ( visited[w] == NONE ) {
				visited[w] = u;
				q.push(w);
			}
			else if ( visited[u] != w ) { // w is visited and w is not the parent of v,
										  // implies that G has a cycle
				while ( !q.empty() ) {
					q.pop();
				}
				delete [] visited; 
				return 0;
			}
		}
		for ( ep = V[u].r_hd; ep != NONE; ep = E[ep].rp ) { // backward
			if ( E[ep].flag == false ) {
				continue;
			}
			w = E[ep].vf;
			if ( visited[w] == NONE ) {
				visited[w] = u;
				q.push(w);
			}
			else if ( visited[u] != w ) { // w is visited and w is not the parent of v,
										  // implies that G has a cycle
				while ( !q.empty() ) {
					q.pop();
				}
				delete [] visited; 
				return 0;
			}
		}
		if ( q.empty() ) {
			for (int i=0; i < Vnum; i++ )
				if (visited[i] == NONE) {
					delete [] visited; 
					return 0;			// the graph is not connected
				}
			delete [] visited; 
			return 1;
		}
		u = q.front();
		q.pop();	
	} while ( 1 );
}