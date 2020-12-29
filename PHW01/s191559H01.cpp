#include "Graph_adj_list.h"
#include <queue>

extern void errorExit(const char *s);

SLList2 pool;
unsigned long UsedMemoryForArray = 0;

void Read_Graph_adj_list(int Vnum, vertex *V, int Enum, edge *E) {
	// ���Ͽ��� �׷��� ������ �о� V�� E �迭�� �����Ѵ�.
	// V[ ].name�� E[ ].name�� ������ �ʿ䰡 ������ ���ǻ� �� �迭������
	//   index�� �����Ѵ�.
	// V[ ].S�� adjacent list header�� ���� ����ε� ���ȴ�.
	// ���� edge (i,j)�� ������ V[i].S�� edge index k�� push�ϰ�
	//  ���������� V[j].S�� k�� push�Ѵ�.
	// V[].flag�� E[].flag�� �ݵ�� false�� �����ؾ� �Ѵ�.

	// ***�� �Լ��� �ۼ��Ѵ�
	for (int n = 0; n < Vnum; n++) {
		V[n].flag = false;							// V[].flag �⺻ false�� ����.
		V[n].name = n;								// V[].name ����.
	}

	int i, j, c;

	for (int n = 0; n < Enum; n++) {
		scanf_s("%d %d %d", &i, &j, &c);
		E[n].vf = i; E[n].vr = j; E[n].cost = c;	// Edge ������ �˸°� ��.
		
		E[n].name = n;								// �� �迭������ index ����.
		E[n].flag = false;							// V[].flag�� E[].flag�� false�� ����.

		SLL* k1 = pool.allocSLL();					//
		SLL* k2 = pool.allocSLL();					// instance�� ����Լ�
		k1->i = n;									//  
		k2->i = n;									// k->i���� edge index�� ��� �ִ�. 

		V[i].S.push(k1);							// 
		V[j].S.push(k2);							// edge index k�� push.
	}

}

void Free_Graph_adj_list(int Vnum, vertex *V) {
	// V �迭�� adjacency list�� ���Ҹ� pop()�� ����Ͽ�
	// ���  pool�� �ǵ��� �ش�

	// ***�� �Լ��� �ۼ��Ѵ�
	for (int n = 0; n < Vnum; n++) {
		while (!V[n].S.empty()) {					// �� V[].S�� �� ������ 
			SLL *p = V[n].S.pop();					// pop ����
			pool.freeSLL(p);						// freeSLL�� p ��ȯ
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
	// BFS�� ����Ͽ� BFS tree�� ���ϴ� ������ flag�� true�� �����Ѵ�.
	// BFS ���� vertex�� �Է� ���Ͽ��� �����ȴ�(src).
	// V[]�� adjacent list Ž���� .top() ��� �Լ��� ����Ͽ�
	//  adj list ����� ��� �̸� ���󰡸� ���� vertex�� Ž���Ѵ�.
	// BFS tree�� ���� ������ cost ���� return �Ѵ�
	//   (not unique but ��� �� edge cost�� 1�̸� unique)
	// -- �Լ��� ��� parameter�� ���� �ʿ�� ����.
	// -- BFS�� ���� queue�� �ʿ��ϸ� STL�� queue�� ����ص� ����

	// ***�� �Լ��� �ۼ��Ѵ�
	int cost = 0;									// BFS tree�� ���� cost �� �����ϴ� ����.

	// queue���� vertex�� ������ ����!
	std::queue<vertex> q;
	q.push(V[src]);
	V[src].flag = true; 

	while (!q.empty()) {							// queue�� �� ������ �ݺ��Ѵ�. 
		SLL* idx = q.front().S.top();
		q.pop();

		for (; idx != NULL; idx = idx->p) {			// V[].S�� ����� edge index�� ���������� �ݺ�
			if (!E[idx->i].flag) {					// �ش� edge�� �湮���� �ʾҴٸ�
				if (!V[E[idx->i].vf].flag) {		// �� edge�� vf�� �湮 ���θ� Ȯ���� 
					q.push(V[E[idx->i].vf]);		// �湮�� �� ���ٸ� queue�� push.
					cost += E[idx->i].cost;			// BFS cost �ش� edge�� cost��ŭ ����
					
					V[E[idx->i].vf].flag = true;	// �ش� vertex �湮 ó��!
					E[idx->i].flag = true;			// �ش� edge �湮 ó��!
				}

				if (!V[E[idx->i].vr].flag) {		// �� edge�� vr�� �湮 ���θ� Ȯ���� 
					q.push(V[E[idx->i].vr]);		// �湮�� �� ���ٸ� queue�� push.
					cost += E[idx->i].cost;			// BFS cost �ش� edge�� cost��ŭ ����
					
					V[E[idx->i].vr].flag = true;	// �ش� vertex �湮 ó��!
					E[idx->i].flag = true;			// �ش� edge �湮 ó��!
				}

			}
		}
	}
	
	return cost;									// BFS spanning tree�� cost ��ȯ.
}

// SLL management functions
// �Ʒ� �Լ����� 02_04_CSE3081_CPrg&DataStructure_�߰��ڷ�.pdf��
//   �����Ͽ� �� �Լ����� ä�� ����.
// ���ø� �ʿ��ϹǷ� ť�� ��� �Լ��� �Ʒ� �Լ� ����Ʈ�� ����

// stack member fuctions
void sllStack2::push(SLL *p) {
	// insert p to the top of stack ST
	p->p = ST;
	ST = p;
}

SLL *sllStack2::pop(void) {
	// remove and return p at the top of ST

	// ***�� �Լ��� �ۼ��Ѵ�
	SLL *p = ST;
	if (ST->p == NULL)								// ���� ���Ұ� NULL�̶�� 
		ST = NULL;									// ���� ���Ҹ� NULL�� ����.
	else
		ST = ST->p;									// �̿� (��κ��� ���) ST�� ��ĭ �δ�. 	
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

	// ***�� �Լ��� �ۼ��Ѵ�
	if (SLL_pool == NULL) {							// SLL_pool ���� �������� �ʴٸ� 
		p = (SLL *)malloc(sizeof(SLL));				// malloc�� ���� ����
		if (p == NULL)
			errorExit("Memory allocation error(allocSLL)");
		UsedMemoryForSLLs += sizeof(SLL);			// SLL�� ���� �޸� ����
		p->i = NONE;
	}
	else {											// �̿� (��κ��� ���)
		p = SLL_pool;								// SLL_pool���� ������ ����. 
		SLL_pool = p->p;
	}
	p->p = NULL;
	++SLL_cnt;										// SLL ���� 1 ����.

	return(p);										// �Ҵ��� �޸� return.
}

void SLList2::freeSLL(SLL *p) {		// free *p (move it to SLL_pool
	if (p->i == NONE) {
		errorExit("This is already freed(freeSLL).");
	}

	// ***�� �Լ��� �ۼ��Ѵ�
	p->i = NONE;
	p->p = SLL_pool;								// free�ϰ��� �ϴ� �޸��� p�� SLL_pool�� ����.
	SLL_pool = p;									// SLL_pool�� ó���� �ٲ� �ش�. 
	--SLL_cnt;
}

void SLList2::freeSLL_pool(void) {	// clear SLL_pool
	SLL *p = SLL_pool;

	// ***�� �Լ��� �ۼ��Ѵ�
	while (p != NULL) {								// p�� NULL�� �ɶ�����
		SLL_pool = p->p;							// �ϳ��� travel�ϸ鼭
		free(p);									// free�Ѵ�. 
		UsedMemoryForSLLs -= sizeof(SLL);
		p = SLL_pool;
	}
	////

	if (SLL_cnt != 0) {
		errorExit("Non-zero SLL_cnt after cleanup.");
	}
}