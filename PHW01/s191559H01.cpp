#include "Graph_adj_list.h"
#include <queue>

extern void errorExit(const char *s);

SLList2 pool;
unsigned long UsedMemoryForArray = 0;

void Read_Graph_adj_list(int Vnum, vertex *V, int Enum, edge *E) {
	// 파일에서 그래프 정보를 읽어 V와 E 배열을 구성한다.
	// V[ ].name과 E[ ].name은 설정할 필요가 없으나 편의상 각 배열에서의
	//   index를 저장한다.
	// V[ ].S는 adjacent list header로 스택 헤더로도 사용된다.
	// 따라서 edge (i,j)를 읽으면 V[i].S에 edge index k를 push하고
	//  마찬가지로 V[j].S에 k를 push한다.
	// V[].flag과 E[].flag은 반드시 false로 설정해야 한다.

	// ***이 함수를 작성한다
	for (int n = 0; n < Vnum; n++) {
		V[n].flag = false;							// V[].flag 기본 false로 설정.
		V[n].name = n;								// V[].name 설정.
	}

	int i, j, c;

	for (int n = 0; n < Enum; n++) {
		scanf_s("%d %d %d", &i, &j, &c);
		E[n].vf = i; E[n].vr = j; E[n].cost = c;	// Edge 정보는 알맞게 들어감.
		
		E[n].name = n;								// 각 배열에서의 index 저장.
		E[n].flag = false;							// V[].flag와 E[].flag를 false로 설정.

		SLL* k1 = pool.allocSLL();					//
		SLL* k2 = pool.allocSLL();					// instance의 멤버함수
		k1->i = n;									//  
		k2->i = n;									// k->i에는 edge index가 담겨 있다. 

		V[i].S.push(k1);							// 
		V[j].S.push(k2);							// edge index k를 push.
	}

}

void Free_Graph_adj_list(int Vnum, vertex *V) {
	// V 배열의 adjacency list의 원소를 pop()을 사용하여
	// 모두  pool로 되돌려 준다

	// ***이 함수를 작성한다
	for (int n = 0; n < Vnum; n++) {
		while (!V[n].S.empty()) {					// 각 V[].S가 빌 때까지 
			SLL *p = V[n].S.pop();					// pop 수행
			pool.freeSLL(p);						// freeSLL로 p 반환
		}
	}
}

int BFS_Tree_adj_list(
	int     src,   // source node index
	int     Vnum,  // number of vertices
	vertex *V,     // vertex structure array (starting index 0)
	int     Enum,  // number of edges
	edge   *E     // edge structure array (starting index 0)
) {
	// BFS를 사용하여 BFS tree에 속하는 에지의 flag을 true로 설정한다.
	// BFS 시작 vertex는 입력 파일에서 지정된다(src).
	// V[]의 adjacent list 탐색은 .top() 멤버 함수를 사용하여
	//  adj list 헤더를 얻고 이를 따라가며 인접 vertex를 탐색한다.
	// BFS tree에 속한 에지의 cost 함을 return 한다
	//   (not unique but 모든 각 edge cost가 1이면 unique)
	// -- 함수의 모든 parameter가 사용될 필요는 없다.
	// -- BFS를 위한 queue가 필요하면 STL의 queue를 사용해도 좋다

	// ***이 함수를 작성한다
	int cost = 0;									// BFS tree에 속한 cost 합 저장하는 변수.

	// queue에는 vertex의 정보가 들어간다!
	std::queue<vertex> q;
	q.push(V[src]);
	V[src].flag = true; 

	while (!q.empty()) {							// queue가 빌 때까지 반복한다. 
		SLL* idx = q.front().S.top();
		q.pop();

		for (; idx != NULL; idx = idx->p) {			// V[].S에 연결된 edge index가 끝날때까지 반복
			if (!E[idx->i].flag) {					// 해당 edge를 방문하지 않았다면
				if (!V[E[idx->i].vf].flag) {		// 그 edge의 vf의 방문 여부를 확인해 
					q.push(V[E[idx->i].vf]);		// 방문한 적 없다면 queue에 push.
					cost += E[idx->i].cost;			// BFS cost 해당 edge의 cost만큼 증가
					
					V[E[idx->i].vf].flag = true;	// 해당 vertex 방문 처리!
					E[idx->i].flag = true;			// 해당 edge 방문 처리!
				}

				if (!V[E[idx->i].vr].flag) {		// 그 edge의 vr의 방문 여부를 확인해 
					q.push(V[E[idx->i].vr]);		// 방문한 적 없다면 queue에 push.
					cost += E[idx->i].cost;			// BFS cost 해당 edge의 cost만큼 증가
					
					V[E[idx->i].vr].flag = true;	// 해당 vertex 방문 처리!
					E[idx->i].flag = true;			// 해당 edge 방문 처리!
				}

			}
		}
	}
	
	return cost;									// BFS spanning tree의 cost 반환.
}

// SLL management functions
// 아래 함수들은 02_04_CSE3081_CPrg&DataStructure_추가자료.pdf를
//   참조하여 각 함수들을 채워 넣자.
// 스택만 필요하므로 큐의 멤버 함수는 아래 함수 리스트에 없다

// stack member fuctions
void sllStack2::push(SLL *p) {
	// insert p to the top of stack ST
	p->p = ST;
	ST = p;
}

SLL *sllStack2::pop(void) {
	// remove and return p at the top of ST

	// ***이 함수를 작성한다
	SLL *p = ST;
	if (ST->p == NULL)								// 다음 원소가 NULL이라면 
		ST = NULL;									// 시작 원소를 NULL로 설정.
	else
		ST = ST->p;									// 이외 (대부분의 경우) ST를 한칸 민다. 	
	return p;
}

SLL *sllStack2::top(void) {
	// return p at the top of ST
	return ST;
}

bool sllStack2::empty(void) {
	// return true if empty
	if (ST == NULL)
		return true;
	else
		return false;
}

// SLList2 member functions
SLL *SLList2::allocSLL(void) {		// allocate an SLL element
	SLL *p;

	// ***이 함수를 작성한다
	if (SLL_pool == NULL) {							// SLL_pool 안이 남아있지 않다면 
		p = (SLL *)malloc(sizeof(SLL));				// malloc을 통해 선언
		if (p == NULL)
			errorExit("Memory allocation error(allocSLL)");
		UsedMemoryForSLLs += sizeof(SLL);			// SLL에 사용된 메모리 증가
		p->i = NONE;
	}
	else {											// 이외 (대부분의 경우)
		p = SLL_pool;								// SLL_pool에서 가져와 쓴다. 
		SLL_pool = p->p;
	}
	p->p = NULL;
	++SLL_cnt;										// SLL 개수 1 증가.

	return(p);										// 할당한 메모리 return.
}

void SLList2::freeSLL(SLL *p) {		// free *p (move it to SLL_pool
	if (p->i == NONE) {
		errorExit("This is already freed(freeSLL).");
	}

	// ***이 함수를 작성한다
	p->i = NONE;
	p->p = SLL_pool;								// free하고자 하는 메모리의 p를 SLL_pool에 연결.
	SLL_pool = p;									// SLL_pool의 처음을 바꿔 준다. 
	--SLL_cnt;
}

void SLList2::freeSLL_pool(void) {	// clear SLL_pool
	SLL *p = SLL_pool;

	// ***이 함수를 작성한다
	while (p != NULL) {								// p가 NULL이 될때까지
		SLL_pool = p->p;							// 하나씩 travel하면서
		free(p);									// free한다. 
		UsedMemoryForSLLs -= sizeof(SLL);
		p = SLL_pool;
	}
	////

	if (SLL_cnt != 0) {
		errorExit("Non-zero SLL_cnt after cleanup.");
	}
}