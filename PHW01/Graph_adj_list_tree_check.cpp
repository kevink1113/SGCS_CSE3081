#include "Graph_adj_list.h"
#include <queue>

#define PARENT_OF_ROOT  -100
#define UNVISITED_IN_TREE_CHECK -1

int Tree_Check_adj_list(int Vnum, vertex *V, int Enum, edge *E ) {
	int u = 0, ep, w;
	int *visited;
	std::queue<int> q;
	for ( int i = 0; i < Enum; i++ ) {  // count the tree edge
		if ( E[i].flag == true ) {
			++u;
		}
	}
	if ( u != (Vnum-1) ) {
		return 0; // it is not a spanning tree
	}
	visited = new int[Vnum];
	for (int i=0; i < Vnum; i++ )
		visited[i] = UNVISITED_IN_TREE_CHECK;
	u = 0;
	visited[u] = PARENT_OF_ROOT;
 	do {
		// check adjacent list at v0 and set Costi_Ni
		for ( SLL *p = (V[u].S).top(); p != NULL; p = p->p ) {
			ep = p->i;
			if ( E[ep].flag == false ) {
				continue;  // not a tree edge
			}
			w = E[ep].vf;
			if ( u == E[ep].vf ) {
				w = E[ep].vr;
			}
			if ( visited[w] == UNVISITED_IN_TREE_CHECK ) {
				visited[w] = u;
				q.push(w);
			}
			else if ( visited[u] != w ) { // w is visited and w is not the parent of v,
										  // implies that G has a cycle
				while ( !q.empty() ) {
					q.pop();
				}
				return 0;
			}
		}
		if ( q.empty() ) {
			for (int i = 0; i < Vnum; i++) {
				if (visited[i] == UNVISITED_IN_TREE_CHECK)
					return 0;			// the graph is not connected
			}
			return 1;
		}
		u = q.front();
		q.pop();	
	} while ( 1 );
}