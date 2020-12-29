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

	// *** �� �κ��� �ۼ��ϼ���

	*pathN = 0;

	int oddNum = 0;									// degree�� Ȧ���� vertex ����
	for (int i = 0; i < VN; i++) {
		if (V[i].degree % 2) oddNum++;				// degree�� Ȧ���� vertex ���� ����.
	}

	if (oddNum == 0) *flag = 0;						// ��� vertex degree�� ¦�����, Euler cycle�� �����Ѵ�. 
	else if (oddNum == 2) *flag = 1;				// Vertex �� ������ degree�� Ȧ�����, Euler Path�� �����Ѵ�. 
	else {											// �� ���� ��쿡�� not Eulerian.
		*flag = 2;
		return path;								// not Eulerian�� ��쿡�� �� Ž���غ� �ʿ䰡 ����. 
	}

	if (*flag == 0) ST.push(0);						// Euler cycle�� ���, Ž�� ���� vertex�� ������ ��(0)�� ���� �ȴ�. 
	else if (*flag == 1) {							// Euler path�� ���, Ž�� ���� vertex�� degree�� Ȧ������ �Ѵ�. 
		for (int i = 0; i < VN; i++) {				// ���� Ž��
			if (V[i].degree % 2) {					// degree�� Ȧ�����, ó�� push�� degree�� ����,
				ST.push(i);							// 
				break;								// ���� Ž�� ����.
			}
		}
	}

	path = new int[EN + 1];							// ũ�Ⱑ |E| + 1�� �迭 path �غ�.
	int pathIdx = 0;								// path �迭�� index�� ��Ÿ���� ����.

	while (!ST.empty()) {							// Stack�� �� ������ �ݺ�
		int v = ST.top();							// Stack�� top ���� ����.
		if (V[v].degree == 0) {						// �ش� vertex�� degree�� 0�̶��, 
			path[pathIdx] = v;						// v�� path�� �߰��ϰ�, 
			(*pathN)++;								// (path ���� 1 ����)
			pathIdx++;								// (path �迭 index 1 ����)
			ST.pop();								// Stack���� ����.
		}
		else {
			DBL* e = V[v].S.top();					// v�� ������ edge e�� ����.
			int w;
			if (v == E[e->d].v1) w = E[e->d].v2;	// 
			else w = E[e->d].v1;					// �ش� edge���� w ����.

			V[w].degree--;							// w vertex�� degree ����.
			V[v].degree--;							// v vertex�� degree ����.

			V[w].S.remove(e->twin);					// 
			V[v].S.remove(e);						// edge e�� graph���� �����Ѵ�. 

			pool.freeDBL(e);						// e�� remove ���� �Ϸ� �� 
			pool.freeDBL(e->twin);					// freeDBL()�� ȣ���Ͽ� pool�� �����ش�. 

			ST.push(w);								// w vertex�� stack�� push. 
		}
	}

	return path;									// path �迭 return. 
}

void deallocGraph(Vertex *V, Edge *E, int VN) {
	DBL *p;

	// *** ���⿡ adj list�� �����ϴ� routine�� �ۼ��ϼ���
	for (int i = 0; i < VN; i++) {					// Vertex number ������ �ݺ�
		while (!V[i].S.empty()) {					// �ش� stack�� �� ������ 
			p = V[i].S.pop();						// �ش� pointer�� pop�� �� 
			pool.freeDBL(p);						// free���ش�. 
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

	// *** �� �κ��� �ۼ��ϼ���
	for (int i = 0; i < EN; i++) {
		adjE1 = pool.allocDBL();					// 
		adjE2 = pool.allocDBL();					// memory allocation.

		adjE1->d = i;								//
		adjE2->d = i;								// Edge index ����.

		adjE1->twin = adjE2;						//
		adjE2->twin = adjE1;						// twin DBL pointer ����.

		v1 = E[i].v1;								// v1, v2�� �ش� edge�� 
		v2 = E[i].v2;								// ���� vertex index ����.

		V[v1].S.push(adjE1);						// 
		V[v2].S.push(adjE2);						// edge�� V[] Stack�� push. 
	}

}

void Error_Exit(const char *s) {
	printf("%s", s);
	exit(-1);
}

DBL *DBList::allocDBL(void) {		// allocate a DBL element
	DBL *p;

	// *** �� �κ��� �ۼ��ϼ���
	if (DBL_pool == NULL) {							// pool�� ����ٸ�, malloc���� �Ҵ�.
		p = new DBL;
		if (p == NULL) {
			Error_Exit("Memory alloc error(Alloc_DBL)");
		}
		UsedMemoryForDBLs += sizeof(DBL); p->d = NONE;
	}
	else {											// DBL_pool���� slm ��������.
		p = DBL_pool;
		DBL_pool = p->rp;							// rp�� ����Ͽ� pool ���� 
	}
	p->rp = p->lp = p->twin = NULL;					// pointer clear.

	++DBL_cnt;										// DBL ���� 1 ����.

	return(p);										// �Ҵ��� memory ����.
}

void DBList::freeDBL(DBL *p) {
	// move p to pool

	if (p->d == NONE) {
		Error_Exit("This element is already freed(Free_DBL).");
	}

	// *** �� �κ��� �ۼ��ϼ���
	p->d = NONE;									// -1 ����. 
	p->rp = DBL_pool;								// 
	DBL_pool = p;									// DBL_pool�� �޸� �ű��. 
	
	--DBL_cnt;										// Ȱ��ȭ�� DBL ���� 1 ����.
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

	// *** �� �κ��� �ۼ��ϼ���
	if (ST != NULL) {								// ������ NULL�� �ƴ϶��
		ST->lp = p;									// left�� p �Ҵ�. 
	}
	p->rp = ST;										// 
	p->lp = NULL;									// 
	ST = p;											// p�� ST�� ��ġ ��ȯ. 
}

DBL *dblStack::pop(void) {	// remove and return p in front of Q
	DBL *p;

	// *** �� �κ��� �ۼ��ϼ���
	p = ST;
	if (ST->rp == NULL)								// ST�� right�� NULL�̶��, 
		ST = NULL;									// ST�� NULL ó��. 
	else {											// �� ���� ���, 
		ST = ST->rp;								// ST�� �� ĭ ���������� �ٲٰ� 
		ST->lp = NULL;								// ST�� left�� NULL ó��.
	}

	return p;										// pop�� ������ ��ȯ. 
}

void dblStack::remove(DBL *p) {	// delete p from ST

	// *** �� �κ��� �ۼ��ϼ���
	if (ST == p) {									// p�� ST�� ù��° ���Ҷ��, 
		ST = p->rp;									// ST�� p�� ������ ���ҷ�.
		if (p->rp != NULL) {						// p�� ������ ���Ұ� �ƴ϶��,
			(p->rp)->lp = NULL;						// p ���� �¸� NULL ó��.
		}
	}
	else {											// p�� ST�� ù��° ���Ұ� �ƴ϶��,  
		(p->lp)->rp = p->rp;						// p�� ���� �츦 p�� ���
		if (p->rp != NULL) {						// p�� ������ ���Ұ� �ƴ϶��, 
			(p->rp)->lp = p->lp;					// p�� ���� �¸� p�� �·�
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
