#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <queue>

#define NONE -1

typedef struct elm_edge {
	int  vf, vr;  // e = (vf, vr) where vf, vr are vertex array indices
	int  cost;    // edge cost
	bool flag;    // true if the edge is in the SP spanning tree, false otherwise 
	int  fp, rp;  // adj list ptr of forward and reverse (-1 if none)
} edge;

typedef struct elm_vertex {
	int  f_hd, r_hd;	// adj list header(-1 if none)
	int  distance;		// distance from src to this vertex
	int  heap_idx;		// minHeap index
	bool inS;			// true if SP length found(true when move from V-S to S) 
} vertex;

void swap(int *a, int *b) {													// �� ���� swap���ִ� �Լ�
	int tmp = (*a);
	(*a) = (*b);
	(*b) = tmp;
}

void push(vertex *V, int vertex, int *minHeap, int *heap_len) {
	++(*heap_len);															// minHeap�� ���� ���� ����. 
	minHeap[*heap_len] = vertex;											// heap�� �� �ڿ� ����. 
	V[vertex].heap_idx = (*heap_len);										// V[].heap_idx cross reference ����. 
	
	int child = (*heap_len);												// child: parent�� ��.
	int parent = child / 2;													// parent: child�� �θ� vertex. 
																			// child�� parent�� ���Ͽ� �� ��ܾ� �ø���. 
	while (child > 1 && V[minHeap[parent]].distance > V[minHeap[child]].distance) { 
																			// child vertex�� distance�� �� ���� ��� 
		swap(&minHeap[parent], &minHeap[child]);							// minHeap�� parent, child ��ġ ���� �ٲٱ�. 
		swap(&V[minHeap[parent]].heap_idx, &V[minHeap[child]].heap_idx);	// V[].heap_idx�� ����. (cross reference)

		child = parent;														// child index�� parent�� ����, 
		parent = child / 2;													// parent�� �ٽ� �� �θ� vertex. 
	}
}

int pop(vertex *V, int *minHeap, int *heap_len) {
	int res = minHeap[1];													// pop�Ͽ� return�� ��: 1��° ����. 
	V[minHeap[1]].heap_idx = NONE;											// ù ���� -1�� �����ϰ� 
	swap(&minHeap[1], &minHeap[*heap_len]);									// �� ���� ���� �ٲ۴�. 

	minHeap[*heap_len] = -1;												// minHeap[] cross reference. 
	(*heap_len)--;															// minHeap�� ���� ���� ����. 
	
	int parent = 1;															// ���� �θ� vertex. 
	int child = parent * 2;													// ���� �ڽ� vertex. 
	if (child + 1 <= *heap_len) {											// child + 1 <= minHeap ���� ������ ���� �Ʒ� ���� ���. 
		if (V[minHeap[child]].distance >= V[minHeap[child + 1]].distance)	// 
			child += 1;														// �� �ڽ� �� distance�� ���� ���� child ������ ��.  
	}
																			// parent�� child�� ���Ͽ� �� ��ܾ� ������. 
	while (child <= *heap_len && V[minHeap[parent]].distance > V[minHeap[child]].distance) {
																			// parent vertex�� distance�� �� ū ��� 
		swap(&minHeap[parent], &minHeap[child]);							// minHeap�� parent, child ��ġ ���� �ٲٱ�. 
		swap(&V[minHeap[parent]].heap_idx, &V[minHeap[child]].heap_idx);	// V[].heap_idx�� ����. (cross reference)
		
		parent = child;														// parent index�� child�� ����. 
		child *= 2;															// child�� �ٽ� �� �ڽ� vertex. 
		if (child + 1 <= *heap_len) {										// child + 1 <= minHeap ���� ������ ���� �Ʒ� ���� ���. 
			if (V[minHeap[child]].distance >= V[minHeap[child + 1]].distance) 
				child += 1;													// �� �ڽ� �� distance�� ���� ���� child ������ ��. 
		}
	}

	return res;																// pop�� ���� ù��° ���� ��ȯ. 
}

void moveUp(int changeVal, vertex *V, int *minHeap, int *heap_len) {
	int heapIdx = V[changeVal].heap_idx;									// ���� ��ȭ�� vertex�� minHeap �� index. 

	int child = heapIdx;													// child: parent�� ��. 
	int parent = child / 2;													// parent: child�� �θ� vertex. 
																			// child�� parent�� ���Ͽ� �� ��ܽ� �ø���. 
	while (child > 1 && V[minHeap[parent]].distance > V[minHeap[child]].distance) {
																			// parent vertex�� distance�� �� ū ��� 
		swap(&minHeap[parent], &minHeap[child]);							// minHeap�� parent, child ��ġ ���� �ٲٱ�. 
		swap(&V[minHeap[parent]].heap_idx, &V[minHeap[child]].heap_idx);	// V[].heap_idx�� ����. (cross reference) 
		
		child = parent;														// child index�� parent�� ����. 
		parent = child / 2;													// parent�� �ٽ� �� �θ� vertex. 
	}

}

int SPT_Dijkstra(
	int src,	// source vertex index
	// graph structure array
	// 1. the adjacency list structure is the same as PHW02
	// 2. additional fields are added for Dijkstra's algorithm(see .h file)
	int Vnum, vertex *V,	// Vertex array size and the array
	int Enum, edge *E,		// Edge array size and the array

	int *minHeap	// array for min heap (array size = Vnum+1)
		// heap index range is 1 ~ (Vnum - 1) note: src must not in the initial heap
		// just arry is passed. must construct min-heap in this function

	// OUTPUT
	// 1. V[].distance : shortest path length from src to this vertex
	// 2. shortest path spanning tree : set E[].flag = true if the edge is in the tree
	// 3. return the sum of edge costs in the shortest path spanning tree.
	//    should be small as possible (think a way to make it small)
) {
	int treeCost = 0;
	// *** �� �Լ��� �ۼ����� ***
	// �ݵ�� min-heap�� ����Ͽ� O((n+m)logn) �˰����� �����ؾ� �Ѵ�(�ƴϸ� trivial�� ���α׷���)
	// heap ���� �� �ʿ��� �Լ��� �����Ӱ� �ۼ��Ͽ� �߰��Ѵ�.
	// �׷��� global ����, dynamic array ���� �߰��� ������� �ʴ´�(������ �ʿ� ����)
	
	int heap_len = 0;														// minHeap�� ���� ����.
	
	for (int i = 0; i < Vnum; i++)											//
		V[i].distance = INT_MAX;											// distance[] INF�� �ʱ�ȭ.
	
	V[src].inS = true;														// src vertex�� �̹� {S}�� ����.
	V[src].distance = 0;													// src vertex�� dis = 0
	
	for (int idx = V[src].f_hd; idx != NONE; idx = E[idx].fp) {				// fp�� ���� src vertex�� ���� vertex Ž��.
		V[E[idx].vr].distance = E[idx].cost;								// src vertex�� ������ vertex�� �Ÿ��� �� edge cost�� ����. 
		push(V, E[idx].vr, minHeap, &heap_len);								// minHeap�� �ش� vertex �߰�.
	}
	for (int idx = V[src].r_hd; idx != NONE; idx = E[idx].rp) {				// rp�� ���� src vertex�� ���� vertex Ž��.
		V[E[idx].vf].distance = E[idx].cost;								// src vertex�� ������ vertex�� �Ÿ��� �� edge cost�� ����.
		push(V, E[idx].vf, minHeap, &heap_len);								// minHeap�� �ش� vertex �߰�.
	}
	
	for (int i = 0; i < Vnum; i++) {
		if (i != src && V[i].distance == INT_MAX)							// src�� �ռ� �߰��� vertex �̿� ������
			push(V, i, minHeap, &heap_len);									// vertex minHeap�� �߰�.
	}
	
	/* ===== (O(n+m)logn) Algorithm ===== */
	for (int i = 0; i < Vnum - 1; i++) {									// Vnum - 1 iterations 
		int u;																// distance[u]�� �ּ��� vertex�� index.
		u = pop(V, minHeap, &heap_len);										// pop �Լ��� �ּ� vertex ����, minHeap���� ����.
		V[u].inS = true;													// �ش� vertex {S}�� �߰�. 
		
		for (int idx = V[u].f_hd; idx != NONE; idx = E[idx].fp) {			// fp�� ���� u�� ������ w Ž��. 
			int w = E[idx].vr;												// ����� edge�� vr�� ����Ű�� vertex ����. 
			if (!V[w].inS && V[u].distance + E[idx].cost < V[w].distance) {	// (u�� distance)+(u-w edge�� cost) < (w�� distance)���
				V[w].distance = V[u].distance + E[idx].cost;				// w�� distance�� �ش� �� ���� ������ ����.
				moveUp(w, V, minHeap, &heap_len);							// moveUp �Լ��� �̿���, ������ w�� distance -> minHeap ����. 
			}
		}
		for (int idx = V[u].r_hd; idx != NONE; idx = E[idx].rp) {			// rp�� ���� u�� ������ w Ž��. 
			int w = E[idx].vf;												// ����� edge�� vf�� ����Ű�� vertex ����.
			if (!V[w].inS && V[u].distance + E[idx].cost < V[w].distance) {	// (u�� distance)+(u-w edge�� cost) < (w�� distance)���
				V[w].distance = V[u].distance + E[idx].cost;				// w�� distance�� �ش� �� ���� ������ ����. 
				moveUp(w, V, minHeap, &heap_len);							// moveUp �Լ��� �̿���, ������ w�� distance -> minHeap ����. 
			}
		}

	}

	/* ===== Backtracing ===== */
	for (int i = 0; i < Vnum; i++) {										// �� vertex�� ���Ͽ� 
		if (i != src) {														// (src vertex�� ����) 
			int parentEdge, child;											// parent: Ȯ�� ���� vertex�� parent���� �̾����� edge index. 
			child = i;														// child : Ȯ�� ���� vertex index. 
			bool found = false;												// �˸��� distance�� vertex ã�� ���� ���� ����. 

																			// fp�� ���� child�� ������ vertex Ž��. 
			for (parentEdge = V[child].f_hd; parentEdge != NONE; parentEdge = E[parentEdge].fp) {
																			// ���� vertex�� distance + edge cost == child�� cost�� ���� ã���� 
				if (V[E[parentEdge].vr].distance + E[parentEdge].cost == V[child].distance) {
					E[parentEdge].flag = true;								// �ش� edge�� Spanning Tree�� �߰�. 
					treeCost += E[parentEdge].cost;							// treeCost �ش� edge cost��ŭ ����. 
					found = true;											// ������ �����ϴ� parent vertex�� ã����. 
					break;													// �ݺ��� ����. 
				}
			}

			if (!found) {													// ���� �� �ݺ������� ������ �����ϴ� parent�� ã�� ���ߴٸ�, 
																			// rp�� ���� child�� ������ vertex Ž��.
				for (parentEdge = V[child].r_hd; parentEdge != NONE; parentEdge = E[parentEdge].rp) {
																			// ���� vertex�� distance + edge cost == child�� cost�� ���� ã����
					if (V[E[parentEdge].vf].distance + E[parentEdge].cost == V[child].distance) {
						E[parentEdge].flag = true;							// �ش� edge�� Spanning Tree�� �߰�. 
						treeCost += E[parentEdge].cost;						// treeCost �ش� edge cost��ŭ ����. 
						found = true;										// ������ �����ϴ� parent vertex�� ã����. 
						break;												// �ݺ��� ����. 
					}
				}
			}

		}
	}
	
	return treeCost;
}

void Read_Graph(int Vnum, vertex *V, int Enum, edge *E) {
	// Graph �ڷᱸ���� ����� �Լ�
	// *** �� �Լ��� �߰����� ***
	// PHW02�� Read_Graph_adj_array()�� �� ������ �ڷᱸ���� ���� ��¦ �����Ͽ� ����Ѵ�
	// ��, Read_Graph_adj_array()���� ���ʿ��� ������ ������ �� ���.

	for (int n = 0; n < Vnum; n++) {
		V[n].inS = false;													// V[].flag �⺻ false�� ����.
		V[n].f_hd = V[n].r_hd = NONE;										// �⺻ -1�� �ʱ�ȭ.
	}
	for (int n = 0; n < Enum; n++) {
		E[n].flag = false;													// E[].flag�� false�� ����.
		E[n].fp = E[n].rp = E[n].vf = E[n].vr = NONE;						// �⺻ -1�� �ʱ�ȭ.
	}

	int i, j, c, tmp;
	for (int n = 0; n < Enum; n++) {
		scanf_s("%d %d %d", &i, &j, &c);
		E[n].vf = i; E[n].vr = j; E[n].cost = c;							// Edge ������ �˸°� ��.

		// V[i]�� front header�� ���� edge index�� ����Ǿ� ���� ���� ��
		if (V[i].f_hd == NONE) {
			V[i].f_hd = n;													// �ش� edge index ����.
		}
		else {																// �̹� ����Ǿ� �ִٸ�
			tmp = V[i].f_hd;												// (�� ��ü �� �ӽ����� ����)
			V[i].f_hd = n;													// �ش� edge index ����.
			E[n].fp = tmp;													// ���� ����� edge index�� E[n].fp�� �ű��. 
		}

		// V[j]�� rear header�� ���� edge index�� ����Ǿ� ���� ���� ��
		if (V[j].r_hd == NONE) {
			V[j].r_hd = n;													// �ش� edge index ����.
		}
		else {																// �̹� ����Ǿ� �ִٸ�
			tmp = V[j].r_hd;												// (�� ��ü �� �ӽ����� ����)
			V[j].r_hd = n;													// �ش� edge index ����.
			E[n].rp = tmp;													// ���� ����� edge index�� E[n].rp�� �ű��. 
		}

	}

}

// the following functions are for testing if the submitted program is correct.
int  Tree_Check(int Vnum, vertex *V, int Enum, edge *E, int *visited);
bool SPT_test(int src, int Vnum, vertex *V, int Enum, edge *E, int *minHeap);
void Error_Exit(const char *s);

int main ( void ) {
	int		src;
	vertex *V;		int Vnum;
	edge   *E;		int Enum;
	int    *minHeap;
	int    Tree_cost;
	int    Tnum; 		// # of test cases
	clock_t start, finish;
	double cmpt;

	scanf_s("%d", &Tnum);		// read # of tests

	for (int t = 0; t < Tnum; t++ ) {
		scanf_s("%d %d %d", &Vnum, &Enum, &src);
		V = new vertex [Vnum];
		E = new edge [Enum];
		minHeap = new int[Vnum + 1];	// heap array allocation
		if ( V == NULL || E == NULL || minHeap == NULL ) {
			Error_Exit("Memory Allocation Error");
		}
		Read_Graph(Vnum, V, Enum, E);

		/**/start = clock();	// start timer

		Tree_cost = SPT_Dijkstra(src, Vnum, V, Enum, E, minHeap);	// code by students

		/**/finish = clock();	// stop timer
		cmpt = ((double)(finish - start)) / (double)CLK_TCK;

		// �Ʒ� Tree_Check�� SPT_test �Լ��� ÷���� SPT_test.obj ���Ͽ� �ִ�.
		// �� �׽�Ʈ���� ������ ���Ͽ� ���α׷��� �����ϸ� ���� �߸��� ���̴�(�����ص� 0��)
        if (Tree_Check(Vnum, V, Enum, E, minHeap) == 0) {
			Error_Exit("   ERROR The result is not a spanning tree");
		}
		if (SPT_test(src, Vnum, V, Enum, E, minHeap) == false) {
			Error_Exit("** Something wrong in applying Dijkstra's");
		}
		if ( t != 0 ) 
			printf("\n");

		printf("**T%d (Dijkstra) (V = %d, E = %d, time = %.3f sec) Tree Cost = %d\n", 
			t+1, Vnum, Enum, cmpt, Tree_cost);

		delete [] minHeap; delete [] V; delete [] E;
	}
	return 0;
}

void Error_Exit (const char *s ) {
  printf("%s\n", s);
  exit(-1);
}
