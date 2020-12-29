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

void swap(int *a, int *b) {													// 두 변수 swap해주는 함수
	int tmp = (*a);
	(*a) = (*b);
	(*b) = tmp;
}

void push(vertex *V, int vertex, int *minHeap, int *heap_len) {
	++(*heap_len);															// minHeap의 원소 개수 증가. 
	minHeap[*heap_len] = vertex;											// heap의 맨 뒤에 삽입. 
	V[vertex].heap_idx = (*heap_len);										// V[].heap_idx cross reference 설정. 
	
	int child = (*heap_len);												// child: parent와 비교.
	int parent = child / 2;													// parent: child의 부모 vertex. 
																			// child를 parent와 비교하여 한 계단씩 올린다. 
	while (child > 1 && V[minHeap[parent]].distance > V[minHeap[child]].distance) { 
																			// child vertex의 distance가 더 작은 경우 
		swap(&minHeap[parent], &minHeap[child]);							// minHeap의 parent, child 위치 서로 바꾸기. 
		swap(&V[minHeap[parent]].heap_idx, &V[minHeap[child]].heap_idx);	// V[].heap_idx도 갱신. (cross reference)

		child = parent;														// child index를 parent로 설정, 
		parent = child / 2;													// parent는 다시 그 부모 vertex. 
	}
}

int pop(vertex *V, int *minHeap, int *heap_len) {
	int res = minHeap[1];													// pop하여 return할 값: 1번째 원소. 
	V[minHeap[1]].heap_idx = NONE;											// 첫 값을 -1로 설정하고 
	swap(&minHeap[1], &minHeap[*heap_len]);									// 맨 뒤의 값과 바꾼다. 

	minHeap[*heap_len] = -1;												// minHeap[] cross reference. 
	(*heap_len)--;															// minHeap의 원소 개수 감소. 
	
	int parent = 1;															// 비교할 부모 vertex. 
	int child = parent * 2;													// 비교할 자식 vertex. 
	if (child + 1 <= *heap_len) {											// child + 1 <= minHeap 원소 개수일 때만 아래 조건 고려. 
		if (V[minHeap[child]].distance >= V[minHeap[child + 1]].distance)	// 
			child += 1;														// 두 자식 중 distance가 작은 값을 child 변수로 택.  
	}
																			// parent를 child와 비교하여 한 계단씩 내린다. 
	while (child <= *heap_len && V[minHeap[parent]].distance > V[minHeap[child]].distance) {
																			// parent vertex의 distance가 더 큰 경우 
		swap(&minHeap[parent], &minHeap[child]);							// minHeap의 parent, child 위치 서로 바꾸기. 
		swap(&V[minHeap[parent]].heap_idx, &V[minHeap[child]].heap_idx);	// V[].heap_idx도 갱신. (cross reference)
		
		parent = child;														// parent index를 child로 설정. 
		child *= 2;															// child는 다시 그 자식 vertex. 
		if (child + 1 <= *heap_len) {										// child + 1 <= minHeap 원소 개수일 때만 아래 조건 고려. 
			if (V[minHeap[child]].distance >= V[minHeap[child + 1]].distance) 
				child += 1;													// 두 자식 중 distance가 작은 값을 child 변수로 택. 
		}
	}

	return res;																// pop할 기존 첫번째 원소 반환. 
}

void moveUp(int changeVal, vertex *V, int *minHeap, int *heap_len) {
	int heapIdx = V[changeVal].heap_idx;									// 값이 변화한 vertex의 minHeap 상 index. 

	int child = heapIdx;													// child: parent와 비교. 
	int parent = child / 2;													// parent: child의 부모 vertex. 
																			// child를 parent와 비교하여 한 계단식 올린다. 
	while (child > 1 && V[minHeap[parent]].distance > V[minHeap[child]].distance) {
																			// parent vertex의 distance가 더 큰 경우 
		swap(&minHeap[parent], &minHeap[child]);							// minHeap의 parent, child 위치 서로 바꾸기. 
		swap(&V[minHeap[parent]].heap_idx, &V[minHeap[child]].heap_idx);	// V[].heap_idx도 갱신. (cross reference) 
		
		child = parent;														// child index를 parent로 설정. 
		parent = child / 2;													// parent는 다시 그 부모 vertex. 
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
	// *** 이 함수를 작성하자 ***
	// 반드시 min-heap을 사용하여 O((n+m)logn) 알고리즘을 구현해야 한다(아니면 trivial한 프로그램임)
	// heap 연산 등 필요한 함수는 자유롭게 작성하여 추가한다.
	// 그러나 global 변수, dynamic array 등은 추가로 사용하지 않는다(실제로 필요 없다)
	
	int heap_len = 0;														// minHeap의 원소 개수.
	
	for (int i = 0; i < Vnum; i++)											//
		V[i].distance = INT_MAX;											// distance[] INF로 초기화.
	
	V[src].inS = true;														// src vertex은 이미 {S}에 있음.
	V[src].distance = 0;													// src vertex의 dis = 0
	
	for (int idx = V[src].f_hd; idx != NONE; idx = E[idx].fp) {				// fp를 따라 src vertex의 인접 vertex 탐색.
		V[E[idx].vr].distance = E[idx].cost;								// src vertex과 인접한 vertex의 거리는 그 edge cost와 같다. 
		push(V, E[idx].vr, minHeap, &heap_len);								// minHeap에 해당 vertex 추가.
	}
	for (int idx = V[src].r_hd; idx != NONE; idx = E[idx].rp) {				// rp를 따라 src vertex의 인접 vertex 탐색.
		V[E[idx].vf].distance = E[idx].cost;								// src vertex과 인접한 vertex의 거리는 그 edge cost와 같다.
		push(V, E[idx].vf, minHeap, &heap_len);								// minHeap에 해당 vertex 추가.
	}
	
	for (int i = 0; i < Vnum; i++) {
		if (i != src && V[i].distance == INT_MAX)							// src와 앞서 추가한 vertex 이외 나머지
			push(V, i, minHeap, &heap_len);									// vertex minHeap에 추가.
	}
	
	/* ===== (O(n+m)logn) Algorithm ===== */
	for (int i = 0; i < Vnum - 1; i++) {									// Vnum - 1 iterations 
		int u;																// distance[u]가 최소인 vertex의 index.
		u = pop(V, minHeap, &heap_len);										// pop 함수로 최소 vertex 추출, minHeap에서 제거.
		V[u].inS = true;													// 해당 vertex {S}에 추가. 
		
		for (int idx = V[u].f_hd; idx != NONE; idx = E[idx].fp) {			// fp를 따라 u에 인접한 w 탐색. 
			int w = E[idx].vr;												// 연결된 edge의 vr이 가리키는 vertex 선택. 
			if (!V[w].inS && V[u].distance + E[idx].cost < V[w].distance) {	// (u의 distance)+(u-w edge의 cost) < (w의 distance)라면
				V[w].distance = V[u].distance + E[idx].cost;				// w의 distance를 해당 더 작은 값으로 갱신.
				moveUp(w, V, minHeap, &heap_len);							// moveUp 함수를 이용해, 수정된 w의 distance -> minHeap 갱신. 
			}
		}
		for (int idx = V[u].r_hd; idx != NONE; idx = E[idx].rp) {			// rp를 따라 u에 인접한 w 탐색. 
			int w = E[idx].vf;												// 연결된 edge의 vf이 가리키는 vertex 선택.
			if (!V[w].inS && V[u].distance + E[idx].cost < V[w].distance) {	// (u의 distance)+(u-w edge의 cost) < (w의 distance)라면
				V[w].distance = V[u].distance + E[idx].cost;				// w의 distance를 해당 더 작은 값으로 갱신. 
				moveUp(w, V, minHeap, &heap_len);							// moveUp 함수를 이용해, 수정된 w의 distance -> minHeap 갱신. 
			}
		}

	}

	/* ===== Backtracing ===== */
	for (int i = 0; i < Vnum; i++) {										// 각 vertex에 대하여 
		if (i != src) {														// (src vertex는 제외) 
			int parentEdge, child;											// parent: 확인 중인 vertex의 parent으로 이어지는 edge index. 
			child = i;														// child : 확인 중인 vertex index. 
			bool found = false;												// 알맞은 distance의 vertex 찾은 여부 저장 변수. 

																			// fp를 따라 child에 인접한 vertex 탐색. 
			for (parentEdge = V[child].f_hd; parentEdge != NONE; parentEdge = E[parentEdge].fp) {
																			// 인접 vertex의 distance + edge cost == child의 cost일 때를 찾으면 
				if (V[E[parentEdge].vr].distance + E[parentEdge].cost == V[child].distance) {
					E[parentEdge].flag = true;								// 해당 edge를 Spanning Tree에 추가. 
					treeCost += E[parentEdge].cost;							// treeCost 해당 edge cost만큼 증가. 
					found = true;											// 조건을 만족하는 parent vertex를 찾았음. 
					break;													// 반복문 종료. 
				}
			}

			if (!found) {													// 만약 위 반복문에서 조건을 만족하는 parent를 찾지 못했다면, 
																			// rp를 따라 child에 인접한 vertex 탐색.
				for (parentEdge = V[child].r_hd; parentEdge != NONE; parentEdge = E[parentEdge].rp) {
																			// 인접 vertex의 distance + edge cost == child의 cost일 때를 찾으면
					if (V[E[parentEdge].vf].distance + E[parentEdge].cost == V[child].distance) {
						E[parentEdge].flag = true;							// 해당 edge를 Spanning Tree에 추가. 
						treeCost += E[parentEdge].cost;						// treeCost 해당 edge cost만큼 증가. 
						found = true;										// 조건을 만족하는 parent vertex를 찾았음. 
						break;												// 반복문 종료. 
					}
				}
			}

		}
	}
	
	return treeCost;
}

void Read_Graph(int Vnum, vertex *V, int Enum, edge *E) {
	// Graph 자료구조를 만드는 함수
	// *** 이 함수를 추가하자 ***
	// PHW02의 Read_Graph_adj_array()를 이 과제의 자료구조에 맞춰 살짝 수정하여 사용한다
	// 즉, Read_Graph_adj_array()에서 불필요한 내용을 제거한 후 사용.

	for (int n = 0; n < Vnum; n++) {
		V[n].inS = false;													// V[].flag 기본 false로 설정.
		V[n].f_hd = V[n].r_hd = NONE;										// 기본 -1로 초기화.
	}
	for (int n = 0; n < Enum; n++) {
		E[n].flag = false;													// E[].flag를 false로 설정.
		E[n].fp = E[n].rp = E[n].vf = E[n].vr = NONE;						// 기본 -1로 초기화.
	}

	int i, j, c, tmp;
	for (int n = 0; n < Enum; n++) {
		scanf_s("%d %d %d", &i, &j, &c);
		E[n].vf = i; E[n].vr = j; E[n].cost = c;							// Edge 정보는 알맞게 들어감.

		// V[i]의 front header에 인접 edge index가 저장되어 있지 않을 때
		if (V[i].f_hd == NONE) {
			V[i].f_hd = n;													// 해당 edge index 저장.
		}
		else {																// 이미 저장되어 있다면
			tmp = V[i].f_hd;												// (값 교체 전 임시저장 변수)
			V[i].f_hd = n;													// 해당 edge index 저장.
			E[n].fp = tmp;													// 원래 저장된 edge index를 E[n].fp로 옮긴다. 
		}

		// V[j]의 rear header에 인접 edge index가 저장되어 있지 않을 때
		if (V[j].r_hd == NONE) {
			V[j].r_hd = n;													// 해당 edge index 저장.
		}
		else {																// 이미 저장되어 있다면
			tmp = V[j].r_hd;												// (값 교체 전 임시저장 변수)
			V[j].r_hd = n;													// 해당 edge index 저장.
			E[n].rp = tmp;													// 원래 저장된 edge index를 E[n].rp로 옮긴다. 
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

		// 아래 Tree_Check와 SPT_test 함수는 첨부한 SPT_test.obj 파일에 있다.
		// 이 테스트에서 오류로 인하여 프로그램이 정지하면 뭔가 잘못된 것이다(제출해도 0점)
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
