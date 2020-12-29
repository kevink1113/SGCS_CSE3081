#include "Graph_adj_array.h"

void Read_Graph_adj_array(int Vnum, vertex V[], int Enum, edge E[]) {
	// read graph information
	// V와 E의 name field는 각각 자신의 인덱스를 저장한다
	// V[].flag과 E[].flag은 반드시 false로 설정해야 한다.

	// ***이 함수를 작성한다
	for (int n = 0; n < Vnum; n++) {
		V[n].flag = false;								// V[].flag 기본 false로 설정.
		V[n].name = n;									// V[].name index로 설정.
		V[n].f_hd = V[n].r_hd = NONE;					// 기본 -1로 초기화.
	}
	for (int n = 0; n < Enum; n++) {
		E[n].flag = false;								// E[].flag를 false로 설정.
		E[n].name = n;									// 각 배열에서의 index 저장.
		E[n].fp = E[n].rp = E[n].vf = E[n].vr = NONE;	// 기본 -1로 초기화.
	}

	int i, j, c, tmp;
	for (int n = 0; n < Enum; n++) {
		scanf_s("%d %d %d", &i, &j, &c);
		E[n].vf = i; E[n].vr = j; E[n].cost = c;		// Edge 정보는 알맞게 들어감.

		// V[i]의 front header에 인접 edge index가 저장되어 있지 않을 때
		if (V[i].f_hd == NONE) {
			V[i].f_hd = n;								// 해당 edge index 저장.
		} else {										// 이미 저장되어 있다면
			tmp = V[i].f_hd;							// (값 교체 전 임시저장 변수)
			V[i].f_hd = n;								// 해당 edge index 저장.
			E[n].fp = tmp;								// 원래 저장된 edge index를 E[n].fp로 옮긴다. 
		}

		// V[j]의 rear header에 인접 edge index가 저장되어 있지 않을 때
		if (V[j].r_hd == NONE) {
			V[j].r_hd = n;								// 해당 edge index 저장.
		} else {										// 이미 저장되어 있다면
			tmp = V[j].r_hd;							// (값 교체 전 임시저장 변수)
			V[j].r_hd = n;								// 해당 edge index 저장.
			E[n].rp = tmp;								// 원래 저장된 edge index를 E[n].rp로 옮긴다. 
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
	// DFS를 사용하여 DFS tree에 속하는 에지의 flag을 true로 설정한다.
	// DFS 시작 vertex는 입력 파일에서 지정된다(src).
	// DFS tree에 속한 에지의 cost 함을 return 한다(모든 각 edge cost가 1이면 unique)
	// recursive 함수로 작성해도 무방하다.

	// ***이 함수를 작성한다***
	V[src].flag = true;
	int idx;											// 탐색해 나아가는 edge index
	int cost = 0;										// DFS Spanning Tree의 cost 저장 변수.

	// front header를 따라 연결된 edge의 인접 vertex 탐색.
	for (idx = V[src].f_hd; idx != NONE; idx = E[idx].fp) {
		
		if (!V[E[idx].vr].flag) {						// 해당 edge에 인접한 vr vertex를 아직 방문하지 않았다면
			V[E[idx].vr].flag = true;					// vertex flag 방문 처리.
			E[idx].flag = true;							// edge flag 방문 처리.
			if (idx == NONE) cost += E[idx].cost;		// 다음 연결된 edge가 없다면 해당 edge cost 더하며 마무리.
														// 존재할 경우 재귀로 DFS 탐색하며 tree cost의 누적 합을 구한다. 
			cost += E[idx].cost + DFS_Tree_adj_array(E[idx].vr, Vnum, V, Enum, E);
		}
	}

	// rear header를 따라 연결된 edge의 인접 vertex 탐색.
	for (idx = V[src].r_hd; idx != NONE; idx = E[idx].rp) {
		
		if (!V[E[idx].vf].flag) {						// 해당 edge에 인접한 vr vertex를 아직 방문하지 않았다면
			V[E[idx].vf].flag = true;					// vertex flag 방문 처리.
			E[idx].flag = true;							// edge flag 방문 처리.
			if (idx == NONE) cost += E[idx].cost;		// 다음 연결된 edge가 없다면 해당 edge cost 더하며 마무리.
														// 존재할 경우 재귀로 DFS 탐색하며 tree cost의 누적 합을 구한다. 
			cost += E[idx].cost + DFS_Tree_adj_array(E[idx].vf, Vnum, V, Enum, E);
		}
	}

	return cost;										// DFS Spanning Tree cost 반환.
}