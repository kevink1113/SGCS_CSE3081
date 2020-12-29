#define _CRT_SECURE_NO_WARNINGS
using namespace std;
#include <time.h>
#include <stack>
#include "DBL.h"

//#define NO_PATH_OUT   // comment out this line for path output
void Error_Exit(const char *s);

typedef struct _Vertex {
	dblStack S;		// adj list contains edge index
	int degree;
} Vertex;

typedef struct _Edge {
	int v1, v2;
} Edge;

void graphGeneration(Vertex **V, Edge **E, int *VN, int *EN);
void adjListGenerate(Vertex *V, Edge *E, int VN, int EN);
void deallocGraph(Vertex *V, Edge *E, int VN);
int *Find_Euler(Vertex *V, Edge *E, int VN, int EN, int *flag, int *pathN);

DBList pool;	// DBL storage pool

int main() {
	int T, VN, EN;
	Vertex *V;
	Edge   *E;
	int *path;	// Euler cycle or path
	int pathN;  // path length
	int  flag;	// 0: none, 1: cycle, 2:path
	clock_t start_time, finish_time;

	scanf("%d", &T);	// read # of tests
	for (int t = 1; t <= T; t++) {	// for each test
		graphGeneration(&V, &E, &VN, &EN);

		start_time = clock(); // set the start time

		path = Find_Euler(V, E, VN, EN, &flag, &pathN); // find an Euler path or cycle

		finish_time = clock(); // set finish time

		double cmpt = (((double)(finish_time - start_time)) / CLK_TCK) * 1000; // compute the time passed
		printf("Test= %d flag= %d VnumInCycle/Path)= %d ", t, flag, pathN);

		if (flag == 0)
			printf("Euler_cycle(exec_time= %.2f msec)\n", cmpt);
		else if (flag == 1)
			printf("Euler_path(exec_time= %.2f msec)\n", cmpt);
		else
			printf("not_Eulerian(exec_time= %.2f msec)\n", cmpt);

#ifndef NO_PATH_OUT
		if (flag != 2) {
			for (int i = 0; i < EN + 1; i++) {
				printf("%d\n", path[i]);
			}
		}
#endif
		if (flag != 2)
			delete[] path;
		deallocGraph(V, E, VN);
	}
	pool.freeDBL_pool();	// clear all the DBL elements

	return 0;
}

int *Find_Euler(Vertex *V, Edge *E, int VN, int EN, int *flag, int *pathN) {
	// input V, VN, E, EN
	// output through paramters
	//   *flag = 0 (Euler cycle), 1 (Euler path), 2 (not Eulerian)
	//   *pathN = size of path[] array
	// output by return
	//   *path = list of vertex ids found(Euler cycle or path)

	stack<int> ST;		// use STL stack as explained in the class
	int *path = NULL;

	// *** 이 부분을 작성하세요

	*pathN = 0;

	int oddNum = 0;									// degree가 홀수인 vertex 개수
	for (int i = 0; i < VN; i++) {
		if (V[i].degree % 2) oddNum++;				// degree가 홀수인 vertex 개수 세기.
	}

	if (oddNum == 0) *flag = 0;						// 모든 vertex degree가 짝수라면, Euler cycle이 존재한다. 
	else if (oddNum == 2) *flag = 1;				// Vertex 두 개만의 degree가 홀수라면, Euler Path가 존재한다. 
	else {											// 그 외의 경우에는 not Eulerian.
		*flag = 2;
		return path;								// not Eulerian인 경우에는 더 탐색해볼 필요가 없다. 
	}

	if (*flag == 0) ST.push(0);						// Euler cycle인 경우, 탐색 시작 vertex는 임의의 것(0)을 고르면 된다. 
	else if (*flag == 1) {							// Euler path인 경우, 탐색 시작 vertex는 degree가 홀수여야 한다. 
		for (int i = 0; i < VN; i++) {				// 순차 탐색
			if (V[i].degree % 2) {					// degree가 홀수라면, 처음 push할 degree로 선택,
				ST.push(i);							// 
				break;								// 순차 탐색 종료.
			}
		}
	}

	path = new int[EN + 1];							// 크기가 |E| + 1인 배열 path 준비.
	int pathIdx = 0;								// path 배열의 index를 나타내는 변수.

	while (!ST.empty()) {							// Stack이 빌 때까지 반복
		int v = ST.top();							// Stack의 top 원소 선택.
		if (V[v].degree == 0) {						// 해당 vertex의 degree가 0이라면, 
			path[pathIdx] = v;						// v를 path에 추가하고, 
			(*pathN)++;								// (path 길이 1 증가)
			pathIdx++;								// (path 배열 index 1 증가)
			ST.pop();								// Stack에서 제거.
		}
		else {
			DBL* e = V[v].S.top();					// v에 인접한 edge e를 선택.
			int w;
			if (v == E[e->d].v1) w = E[e->d].v2;	// 
			else w = E[e->d].v1;					// 해당 edge에서 w 선택.

			V[w].degree--;							// w vertex의 degree 감소.
			V[v].degree--;							// v vertex의 degree 감소.

			V[w].S.remove(e->twin);					// 
			V[v].S.remove(e);						// edge e를 graph에서 제거한다. 

			pool.freeDBL(e);						// e는 remove 실행 완료 후 
			pool.freeDBL(e->twin);					// freeDBL()을 호출하여 pool로 돌려준다. 

			ST.push(w);								// w vertex를 stack에 push. 
		}
	}

	return path;									// path 배열 return. 
}

void deallocGraph(Vertex *V, Edge *E, int VN) {
	DBL *p;

	// *** 여기에 adj list를 삭제하는 routine을 작성하세요
	for (int i = 0; i < VN; i++) {					// Vertex number 끝까지 반복
		while (!V[i].S.empty()) {					// 해당 stack이 빌 때까지 
			p = V[i].S.pop();						// 해당 pointer를 pop한 후 
			pool.freeDBL(p);						// free해준다. 
		}
	}

	delete[] V;
	delete[] E;
}

void graphGeneration(Vertex **V, Edge **E, int *VN, int *EN) {
	scanf("%d %d", VN, EN);	// read # of vertices and edges
	//allocVandEarray(V, E, *VN, *EN);	// vertex and edge array allocation

	*V = new Vertex[*VN];
	*E = new Edge[*EN];
	if (*V == NULL || *E == NULL) {
		Error_Exit("Memory Allocation Error!");
	}

	for (int v = 0; v < *VN; v++) {
		(*V)[v].degree = 0;
	}
	for (int e = 0; e < *EN; e++) {
		scanf("%d %d", &((*E)[e].v1), &((*E)[e].v2));	// read edge information
		++((*V)[(*E)[e].v1].degree);
		++((*V)[(*E)[e].v2].degree);
	}
	adjListGenerate(*V, *E, *VN, *EN);	// create adj lust in G=(V,E)
}

void adjListGenerate(Vertex *V, Edge *E, int VN, int EN) {
	// Construct adjacent list in V array
	int v1, v2;
	DBL *adjE1, *adjE2;

	// *** 이 부분을 작성하세요
	for (int i = 0; i < EN; i++) {
		adjE1 = pool.allocDBL();					// 
		adjE2 = pool.allocDBL();					// memory allocation.

		adjE1->d = i;								//
		adjE2->d = i;								// Edge index 저장.

		adjE1->twin = adjE2;						//
		adjE2->twin = adjE1;						// twin DBL pointer 저장.

		v1 = E[i].v1;								// v1, v2에 해당 edge의 
		v2 = E[i].v2;								// 인접 vertex index 저장.

		V[v1].S.push(adjE1);						// 
		V[v2].S.push(adjE2);						// edge를 V[] Stack에 push. 
	}

}

void Error_Exit(const char *s) {
	printf("%s", s);
	exit(-1);
}

DBL *DBList::allocDBL(void) {		// allocate a DBL element
	DBL *p;

	// *** 이 부분을 작성하세요
	if (DBL_pool == NULL) {							// pool이 비었다면, malloc으로 할당.
		p = new DBL;
		if (p == NULL) {
			Error_Exit("Memory alloc error(Alloc_DBL)");
		}
		UsedMemoryForDBLs += sizeof(DBL); p->d = NONE;
	}
	else {											// DBL_pool에서 slm 가져오기.
		p = DBL_pool;
		DBL_pool = p->rp;							// rp를 사용하여 pool 연결 
	}
	p->rp = p->lp = p->twin = NULL;					// pointer clear.

	++DBL_cnt;										// DBL 개수 1 증가.

	return(p);										// 할당한 memory 전달.
}

void DBList::freeDBL(DBL *p) {
	// move p to pool

	if (p->d == NONE) {
		Error_Exit("This element is already freed(Free_DBL).");
	}

	// *** 이 부분을 작성하세요
	p->d = NONE;									// -1 저장. 
	p->rp = DBL_pool;								// 
	DBL_pool = p;									// DBL_pool로 메모리 옮기기. 
	
	--DBL_cnt;										// 활성화된 DBL 개수 1 감소.
}

void DBList::freeDBL_pool(void) {
	DBL *p = DBL_pool;

	while (p != NULL) {
		DBL_pool = p->rp;	// get next pointer
		delete p;
		p = DBL_pool;
		UsedMemoryForDBLs -= sizeof(DBL);
	}
	if (DBL_cnt != 0) {
		Error_Exit("Non-zero DBL_cnt after cleanup.");
	}
}

void dblStack::push(DBL *p) {

	// *** 이 부분을 작성하세요
	if (ST != NULL) {								// 시작이 NULL이 아니라면
		ST->lp = p;									// left에 p 할당. 
	}
	p->rp = ST;										// 
	p->lp = NULL;									// 
	ST = p;											// p와 ST의 위치 교환. 
}

DBL *dblStack::pop(void) {	// remove and return p in front of Q
	DBL *p;

	// *** 이 부분을 작성하세요
	p = ST;
	if (ST->rp == NULL)								// ST의 right가 NULL이라면, 
		ST = NULL;									// ST도 NULL 처리. 
	else {											// 그 외의 경우, 
		ST = ST->rp;								// ST를 한 칸 오른쪽으로 바꾸고 
		ST->lp = NULL;								// ST의 left는 NULL 처리.
	}

	return p;										// pop한 포인터 반환. 
}

void dblStack::remove(DBL *p) {	// delete p from ST

	// *** 이 부분을 작성하세요
	if (ST == p) {									// p가 ST의 첫번째 원소라면, 
		ST = p->rp;									// ST를 p의 오른쪽 원소로.
		if (p->rp != NULL) {						// p가 유일한 원소가 아니라면,
			(p->rp)->lp = NULL;						// p 우의 좌를 NULL 처리.
		}
	}
	else {											// p가 ST의 첫번째 원소가 아니라면,  
		(p->lp)->rp = p->rp;						// p의 좌의 우를 p의 우로
		if (p->rp != NULL) {						// p가 마지막 원소가 아니라면, 
			(p->rp)->lp = p->lp;					// p의 우의 좌를 p의 좌로
		}
	}
}

DBL *dblStack::top(void) {
	return ST;
}

bool dblStack::empty(void) {
	if (ST == NULL)
		return true;
	else
		return false;
}
