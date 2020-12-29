#define _CRT_SECURE_NO_WARNINGS
using namespace std;
#include <stdio.h>
#include <stdlib.h>
#include <limits>
#include <algorithm>

int min3(int a, int b, int c) {											// 세 수 중 가장 작은 값 return하는 함수. 
	int min;															// 최소값 저장 변수. 
	min = (a < b) ? a : b;												// a와 b 중 작은 값. 
	min = (min < c) ? min : c;											// 위 값과 c 값 중 더 작은 값. 

	return min;															// 최종 가장 작은 값 반환. 
}

int minIdx(int a, int b, int c,											// insert, delete, substitute 중 가장 작은 경우 return. 
	int ins_cost, int del_cost, int sub_cost) {							// (우선순위 지키며 (pdf 5번 항목))
	if (a == b && b <= c) {
		if (ins_cost <= del_cost) return 1;								// ins_cost <= del_cost이면 반드시 insert를 선택해야 하며, 
		else return 2;													// ins_cost > del_cost일 경우 delete를 선택한다. 
	}
	if (a <= b && b <= c) return 1;										// 
	else if (a <= c && c <= b) return 1;								// insert의 경우가 가장 작은 경우, 1 return.
	else if (b <= c && c <= a) return 2;								// 
	else if (b < a && a <= c) return 2;									// delete의 경우가 가장 작은 경우, 2 return.
	else return 3;														// substitution의 경우가 가장 작은 경우, 3 return.
}

void Edit_Distance(
	//inputs 
	char *SS,															// Source string array 
	char *TS,															// Edited string array 
	int ins_cost, int del_cost, int sub_cost,	
																		// insertion, deletion, substitution cost

	//outputs 
	int **Table,														// Cost Table Calculated (2-d array (|SS|+1) x (|TS+1) )
																		// 메인에서 allocation하여 제공한다. 
	char **SR,															// Source string ('*' implies insertion of a char to T)
	char **OP,															// Operation sequence('i', 'd', 's', or '.' (nop))
	char **TR,															// Edited string ('*' implies deletion of a char from S)
	int *Mem_Allocated													// allocation한 SR, OP, TR의 크기 합을 저장하여 main에 전달. 
) {
	int m = strlen(SS);													// Source String의 길이.
	int n = strlen(TS);													// Edited String의 길이.
	int alpha;															// Substitution / No operation의 경우 cost 다름 변수.

	/* ----- Table[i][j] calculation (bottom-up) ----- */
	for (int i = 0; i <= m; i++)										// 
		Table[i][0] = i * del_cost;										// 0번 열의 초기화. 
	for (int j = 0; j <= n; j++)										// 
		Table[0][j] = j * ins_cost;										// 0번 행의 초기화. 

	for (int i = 1; i <= m; i++) {										// 나머지 cost Table DP로 채우기.
		for (int j = 1; j <= n; j++) {			
			if (SS[i - 1] == TS[j - 1]) alpha = 0;						// 바꿀 필요 없을 경우는 0, 
			else alpha = sub_cost;										// substitution의 경우 cost.

			Table[i][j] = min3(Table[i][j - 1] + ins_cost, Table[i - 1][j] + del_cost, Table[i - 1][j - 1] + alpha);
																		// insert, delete, substitute 중 가장 작은 경우 선택.
		}
	}

	/* ----- Finding the Operations (top-down) ----- */
	int i = m, j = n;													// Backtracing 시의 index.
	int idx = 0;														// SR, OP, TR의 입력 index.
	int len = 0;														// SR, OP, TR의 길이.

	while (!(i == 0 && j == 0)) {										// (0, 0)에 도달할 때까지 반복
		bool same = false;
		if (SS[i - 1] == TS[j - 1]) {									// 같은 경우, 
			alpha = 0;													// substitution 불필요. (no operation)
			same = true;												// 
		}
		else alpha = sub_cost;											// 다른 경우, substitution cost 필요.

		int ins, del, sub;												// insert, delete, substitute 세 경우의 cost.

		if (j - 1 >= 0) ins = Table[i][j - 1] + ins_cost;				// insert를 할 수 있는 경우
		else ins = INT_MAX;												// 범위 벗어나는 경우 -> 불능으로 MAX 값 줌.
		if (i - 1 >= 0) del = Table[i - 1][j] + del_cost;				// delete를 할 수 있는 경우
		else del = INT_MAX;												// 범위 벗어나는 경우 -> 불능으로 MAX 값 줌.
		if (i-1 >= 0 && j-1 >= 0) sub = Table[i - 1][j - 1] + alpha;	// substitute를 할 수 있는 경우
		else sub = INT_MAX;												// 범위 벗어나는 경우 -> 불능으로 MAX 값 줌.

		int sel = minIdx(ins, del, sub, ins_cost, del_cost, sub_cost);	// insert, delete, substitute 중 가장 작은 경우 return. 
		if (sel == 1) j--;												// insert가 선택된 경우, j index를 1 줄인다. 
		else if (sel == 2) i--;											// delete가 선택된 경우, i index를 1 줄인다. 
		else {															// substitute가 선택된 경우, 
			i--; j--;													// i와 j index를 1 줄인다. 
		}
		len++;															// SR, OP, TR의 길이 count.
	}
	len++;																// '\0'을 위한 추가 길이 1.
	*SR = new char[len];												// 
	*OP = new char[len];												// 
	*TR = new char[len];												// len 길이만큼 SR, OP, TR 할당.
	(*Mem_Allocated) = 3 * len;											// 할당한 메모리 크기 합. 

	i = m; j = n;														// SR, OP, TR 안의 값을 구하기 위한 Backtracing.
	idx = 0;															// SR, OP, TR의 입력 인덱스.

	while (!(i == 0 && j == 0)) {										// (0, 0)에 도달할 때까지 반복
		bool same = false;
		if (SS[i - 1] == TS[j - 1]) {									// 같은 경우, 
			alpha = 0;													// substitution 불필요. (no operation)
			same = true;												// 
		}
		else alpha = sub_cost;											// 다른 경우, substitution cost 필요.

		int ins, del, sub;												// insert, delete, substitute 세 경우의 cost.
		
		if (j - 1 >= 0) ins = Table[i][j - 1] + ins_cost;				// insert를 할 수 있는 경우 
		else ins = INT_MAX;												// 범위 벗어나는 경우 -> 불능으로 MAX 값 줌.

		if (i - 1 >= 0) del = Table[i - 1][j] + del_cost;				// delete를 할 수 있는 경우
		else del = INT_MAX;												// 범위 벗어나는 경우 -> 불능으로 MAX 값 줌.

		if (i-1 >= 0 && j-1 >= 0) sub = Table[i - 1][j - 1] + alpha;	// substitute를 할 수 있는 경우
		else sub = INT_MAX;												// 범위 벗어나는 경우 -> 불능으로 MAX 값 줌.

		int sel = minIdx(ins, del, sub, ins_cost, del_cost, sub_cost);	// insert, delete, substitute 중 가장 작은 경우 return. 
		if (sel == 1) {													// insert가 선택된 경우 
			(*SR)[idx] = '*';											// SR에 '*' 삽입. 
			(*OP)[idx] = 'i';											// OP에 'i' 삽입. 
			(*TR)[idx] = TS[j-1];										// TR에는 TS의 값 삽입.
			idx++;														// SR, OP, TR의 길이 count.
			j--;														// j index 1 감소.
		}
		else if (sel == 2) {											// delete가 선택된 경우
			(*SR)[idx] = SS[i-1];										// SR에 SS의 값 삽입.
			(*OP)[idx] = 'd';											// OP에 'd' 삽입.
			(*TR)[idx] = '*';											// TR에 '*' 삽입.
			idx++;														// SR, OP, TR의 길이 count.
			i--;														// i index 1 감소. 
		}
		else {															// substitute / no operation이 선택된 경우
			(*SR)[idx] = SS[i-1];										// SR에 SS의 값 삽입.
			if (same)													// 같은 경우, (no operation)
				(*OP)[idx] = '.';										// OP에 '.' 삽입. 
			else														// 같지 않은 경우, (substitution)
				(*OP)[idx] = 's';										// OP에 's' 삽입. 
			(*TR)[idx] = TS[j-1];										// TR에 TS의 값 삽입. 
			idx++;														// SR, OP, TR의 길이 count.
			i--; j--;													// i, j index 1 감소. 
		}
	}

	(*SR)[idx] = '\0';													// 
	(*OP)[idx] = '\0';													// 
	(*TR)[idx] = '\0';													// EOS 삽입. 
	reverse((*SR), (*SR) + idx);										// 
	reverse((*OP), (*OP) + idx);										// 
	reverse((*TR), (*TR) + idx);										// SR, OP, TR의 순서 뒤집어주기. (앞->뒤) 
}