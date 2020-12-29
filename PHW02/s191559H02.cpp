#include "Graph_adj_array.h"

void Read_Graph_adj_array(int Vnum, vertex V[], int Enum, edge E[]) {
	// read graph information
	// V�� E�� name field�� ���� �ڽ��� �ε����� �����Ѵ�
	// V[].flag�� E[].flag�� �ݵ�� false�� �����ؾ� �Ѵ�.

	// ***�� �Լ��� �ۼ��Ѵ�
	for (int n = 0; n < Vnum; n++) {
		V[n].flag = false;								// V[].flag �⺻ false�� ����.
		V[n].name = n;									// V[].name index�� ����.
		V[n].f_hd = V[n].r_hd = NONE;					// �⺻ -1�� �ʱ�ȭ.
	}
	for (int n = 0; n < Enum; n++) {
		E[n].flag = false;								// E[].flag�� false�� ����.
		E[n].name = n;									// �� �迭������ index ����.
		E[n].fp = E[n].rp = E[n].vf = E[n].vr = NONE;	// �⺻ -1�� �ʱ�ȭ.
	}

	int i, j, c, tmp;
	for (int n = 0; n < Enum; n++) {
		scanf_s("%d %d %d", &i, &j, &c);
		E[n].vf = i; E[n].vr = j; E[n].cost = c;		// Edge ������ �˸°� ��.

		// V[i]�� front header�� ���� edge index�� ����Ǿ� ���� ���� ��
		if (V[i].f_hd == NONE) {
			V[i].f_hd = n;								// �ش� edge index ����.
		} else {										// �̹� ����Ǿ� �ִٸ�
			tmp = V[i].f_hd;							// (�� ��ü �� �ӽ����� ����)
			V[i].f_hd = n;								// �ش� edge index ����.
			E[n].fp = tmp;								// ���� ����� edge index�� E[n].fp�� �ű��. 
		}

		// V[j]�� rear header�� ���� edge index�� ����Ǿ� ���� ���� ��
		if (V[j].r_hd == NONE) {
			V[j].r_hd = n;								// �ش� edge index ����.
		} else {										// �̹� ����Ǿ� �ִٸ�
			tmp = V[j].r_hd;							// (�� ��ü �� �ӽ����� ����)
			V[j].r_hd = n;								// �ش� edge index ����.
			E[n].rp = tmp;								// ���� ����� edge index�� E[n].rp�� �ű��. 
		}

	}
}

int DFS_Tree_adj_array(
	int     src,   // source node index
	int     Vnum,  // number of vertices
	vertex *V,     // vertex structure array (starting index 0)
	int     Enum,  // number of edges
	edge   *E     // edge structure array (starting index 0) 
) {
	// DFS�� ����Ͽ� DFS tree�� ���ϴ� ������ flag�� true�� �����Ѵ�.
	// DFS ���� vertex�� �Է� ���Ͽ��� �����ȴ�(src).
	// DFS tree�� ���� ������ cost ���� return �Ѵ�(��� �� edge cost�� 1�̸� unique)
	// recursive �Լ��� �ۼ��ص� �����ϴ�.

	// ***�� �Լ��� �ۼ��Ѵ�***
	V[src].flag = true;
	int idx;											// Ž���� ���ư��� edge index
	int cost = 0;										// DFS Spanning Tree�� cost ���� ����.

	// front header�� ���� ����� edge�� ���� vertex Ž��.
	for (idx = V[src].f_hd; idx != NONE; idx = E[idx].fp) {
		
		if (!V[E[idx].vr].flag) {						// �ش� edge�� ������ vr vertex�� ���� �湮���� �ʾҴٸ�
			V[E[idx].vr].flag = true;					// vertex flag �湮 ó��.
			E[idx].flag = true;							// edge flag �湮 ó��.
			if (idx == NONE) cost += E[idx].cost;		// ���� ����� edge�� ���ٸ� �ش� edge cost ���ϸ� ������.
														// ������ ��� ��ͷ� DFS Ž���ϸ� tree cost�� ���� ���� ���Ѵ�. 
			cost += E[idx].cost + DFS_Tree_adj_array(E[idx].vr, Vnum, V, Enum, E);
		}
	}

	// rear header�� ���� ����� edge�� ���� vertex Ž��.
	for (idx = V[src].r_hd; idx != NONE; idx = E[idx].rp) {
		
		if (!V[E[idx].vf].flag) {						// �ش� edge�� ������ vr vertex�� ���� �湮���� �ʾҴٸ�
			V[E[idx].vf].flag = true;					// vertex flag �湮 ó��.
			E[idx].flag = true;							// edge flag �湮 ó��.
			if (idx == NONE) cost += E[idx].cost;		// ���� ����� edge�� ���ٸ� �ش� edge cost ���ϸ� ������.
														// ������ ��� ��ͷ� DFS Ž���ϸ� tree cost�� ���� ���� ���Ѵ�. 
			cost += E[idx].cost + DFS_Tree_adj_array(E[idx].vf, Vnum, V, Enum, E);
		}
	}

	return cost;										// DFS Spanning Tree cost ��ȯ.
}