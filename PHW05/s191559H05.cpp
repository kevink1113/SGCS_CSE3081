#define _CRT_SECURE_NO_WARNINGS
using namespace std;
#include <stdio.h>
#include <stdlib.h>
#include <limits>
#include <algorithm>

int min3(int a, int b, int c) {											// �� �� �� ���� ���� �� return�ϴ� �Լ�. 
	int min;															// �ּҰ� ���� ����. 
	min = (a < b) ? a : b;												// a�� b �� ���� ��. 
	min = (min < c) ? min : c;											// �� ���� c �� �� �� ���� ��. 

	return min;															// ���� ���� ���� �� ��ȯ. 
}

int minIdx(int a, int b, int c,											// insert, delete, substitute �� ���� ���� ��� return. 
	int ins_cost, int del_cost, int sub_cost) {							// (�켱���� ��Ű�� (pdf 5�� �׸�))
	if (a == b && b <= c) {
		if (ins_cost <= del_cost) return 1;								// ins_cost <= del_cost�̸� �ݵ�� insert�� �����ؾ� �ϸ�, 
		else return 2;													// ins_cost > del_cost�� ��� delete�� �����Ѵ�. 
	}
	if (a <= b && b <= c) return 1;										// 
	else if (a <= c && c <= b) return 1;								// insert�� ��찡 ���� ���� ���, 1 return.
	else if (b <= c && c <= a) return 2;								// 
	else if (b < a && a <= c) return 2;									// delete�� ��찡 ���� ���� ���, 2 return.
	else return 3;														// substitution�� ��찡 ���� ���� ���, 3 return.
}

void Edit_Distance(
	//inputs 
	char *SS,															// Source string array 
	char *TS,															// Edited string array 
	int ins_cost, int del_cost, int sub_cost,	
																		// insertion, deletion, substitution cost

	//outputs 
	int **Table,														// Cost Table Calculated (2-d array (|SS|+1) x (|TS+1) )
																		// ���ο��� allocation�Ͽ� �����Ѵ�. 
	char **SR,															// Source string ('*' implies insertion of a char to T)
	char **OP,															// Operation sequence('i', 'd', 's', or '.' (nop))
	char **TR,															// Edited string ('*' implies deletion of a char from S)
	int *Mem_Allocated													// allocation�� SR, OP, TR�� ũ�� ���� �����Ͽ� main�� ����. 
) {
	int m = strlen(SS);													// Source String�� ����.
	int n = strlen(TS);													// Edited String�� ����.
	int alpha;															// Substitution / No operation�� ��� cost �ٸ� ����.

	/* ----- Table[i][j] calculation (bottom-up) ----- */
	for (int i = 0; i <= m; i++)										// 
		Table[i][0] = i * del_cost;										// 0�� ���� �ʱ�ȭ. 
	for (int j = 0; j <= n; j++)										// 
		Table[0][j] = j * ins_cost;										// 0�� ���� �ʱ�ȭ. 

	for (int i = 1; i <= m; i++) {										// ������ cost Table DP�� ä���.
		for (int j = 1; j <= n; j++) {			
			if (SS[i - 1] == TS[j - 1]) alpha = 0;						// �ٲ� �ʿ� ���� ���� 0, 
			else alpha = sub_cost;										// substitution�� ��� cost.

			Table[i][j] = min3(Table[i][j - 1] + ins_cost, Table[i - 1][j] + del_cost, Table[i - 1][j - 1] + alpha);
																		// insert, delete, substitute �� ���� ���� ��� ����.
		}
	}

	/* ----- Finding the Operations (top-down) ----- */
	int i = m, j = n;													// Backtracing ���� index.
	int idx = 0;														// SR, OP, TR�� �Է� index.
	int len = 0;														// SR, OP, TR�� ����.

	while (!(i == 0 && j == 0)) {										// (0, 0)�� ������ ������ �ݺ�
		bool same = false;
		if (SS[i - 1] == TS[j - 1]) {									// ���� ���, 
			alpha = 0;													// substitution ���ʿ�. (no operation)
			same = true;												// 
		}
		else alpha = sub_cost;											// �ٸ� ���, substitution cost �ʿ�.

		int ins, del, sub;												// insert, delete, substitute �� ����� cost.

		if (j - 1 >= 0) ins = Table[i][j - 1] + ins_cost;				// insert�� �� �� �ִ� ���
		else ins = INT_MAX;												// ���� ����� ��� -> �Ҵ����� MAX �� ��.
		if (i - 1 >= 0) del = Table[i - 1][j] + del_cost;				// delete�� �� �� �ִ� ���
		else del = INT_MAX;												// ���� ����� ��� -> �Ҵ����� MAX �� ��.
		if (i-1 >= 0 && j-1 >= 0) sub = Table[i - 1][j - 1] + alpha;	// substitute�� �� �� �ִ� ���
		else sub = INT_MAX;												// ���� ����� ��� -> �Ҵ����� MAX �� ��.

		int sel = minIdx(ins, del, sub, ins_cost, del_cost, sub_cost);	// insert, delete, substitute �� ���� ���� ��� return. 
		if (sel == 1) j--;												// insert�� ���õ� ���, j index�� 1 ���δ�. 
		else if (sel == 2) i--;											// delete�� ���õ� ���, i index�� 1 ���δ�. 
		else {															// substitute�� ���õ� ���, 
			i--; j--;													// i�� j index�� 1 ���δ�. 
		}
		len++;															// SR, OP, TR�� ���� count.
	}
	len++;																// '\0'�� ���� �߰� ���� 1.
	*SR = new char[len];												// 
	*OP = new char[len];												// 
	*TR = new char[len];												// len ���̸�ŭ SR, OP, TR �Ҵ�.
	(*Mem_Allocated) = 3 * len;											// �Ҵ��� �޸� ũ�� ��. 

	i = m; j = n;														// SR, OP, TR ���� ���� ���ϱ� ���� Backtracing.
	idx = 0;															// SR, OP, TR�� �Է� �ε���.

	while (!(i == 0 && j == 0)) {										// (0, 0)�� ������ ������ �ݺ�
		bool same = false;
		if (SS[i - 1] == TS[j - 1]) {									// ���� ���, 
			alpha = 0;													// substitution ���ʿ�. (no operation)
			same = true;												// 
		}
		else alpha = sub_cost;											// �ٸ� ���, substitution cost �ʿ�.

		int ins, del, sub;												// insert, delete, substitute �� ����� cost.
		
		if (j - 1 >= 0) ins = Table[i][j - 1] + ins_cost;				// insert�� �� �� �ִ� ��� 
		else ins = INT_MAX;												// ���� ����� ��� -> �Ҵ����� MAX �� ��.

		if (i - 1 >= 0) del = Table[i - 1][j] + del_cost;				// delete�� �� �� �ִ� ���
		else del = INT_MAX;												// ���� ����� ��� -> �Ҵ����� MAX �� ��.

		if (i-1 >= 0 && j-1 >= 0) sub = Table[i - 1][j - 1] + alpha;	// substitute�� �� �� �ִ� ���
		else sub = INT_MAX;												// ���� ����� ��� -> �Ҵ����� MAX �� ��.

		int sel = minIdx(ins, del, sub, ins_cost, del_cost, sub_cost);	// insert, delete, substitute �� ���� ���� ��� return. 
		if (sel == 1) {													// insert�� ���õ� ��� 
			(*SR)[idx] = '*';											// SR�� '*' ����. 
			(*OP)[idx] = 'i';											// OP�� 'i' ����. 
			(*TR)[idx] = TS[j-1];										// TR���� TS�� �� ����.
			idx++;														// SR, OP, TR�� ���� count.
			j--;														// j index 1 ����.
		}
		else if (sel == 2) {											// delete�� ���õ� ���
			(*SR)[idx] = SS[i-1];										// SR�� SS�� �� ����.
			(*OP)[idx] = 'd';											// OP�� 'd' ����.
			(*TR)[idx] = '*';											// TR�� '*' ����.
			idx++;														// SR, OP, TR�� ���� count.
			i--;														// i index 1 ����. 
		}
		else {															// substitute / no operation�� ���õ� ���
			(*SR)[idx] = SS[i-1];										// SR�� SS�� �� ����.
			if (same)													// ���� ���, (no operation)
				(*OP)[idx] = '.';										// OP�� '.' ����. 
			else														// ���� ���� ���, (substitution)
				(*OP)[idx] = 's';										// OP�� 's' ����. 
			(*TR)[idx] = TS[j-1];										// TR�� TS�� �� ����. 
			idx++;														// SR, OP, TR�� ���� count.
			i--; j--;													// i, j index 1 ����. 
		}
	}

	(*SR)[idx] = '\0';													// 
	(*OP)[idx] = '\0';													// 
	(*TR)[idx] = '\0';													// EOS ����. 
	reverse((*SR), (*SR) + idx);										// 
	reverse((*OP), (*OP) + idx);										// 
	reverse((*TR), (*TR) + idx);										// SR, OP, TR�� ���� �������ֱ�. (��->��) 
}