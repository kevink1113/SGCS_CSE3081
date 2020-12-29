#define _CRT_SECURE_NO_WARNINGS
using namespace std;
#include <stdio.h>
#include <stdlib.h>
#include <limits>
#include <cmath>

void swap(unsigned* a, unsigned* b) {									// �Է����� ���� �� ���� swap�ϴ� �Լ�.
	unsigned tmp = (*a);												// �ӽ� ���� tmp�� (*a) �� ����.												
	(*a) = (*b);														// (*a) ���� (*b) ������, 
	(*b) = tmp;															// (*b) ���� tmp (���� (*a)��)����.
}

void quickSort(unsigned low, unsigned high, double *X, unsigned *Xid) {	// QuickSort ���� �Լ�.
	if (low < high) {													// low < high�϶������� ���� (partition)
		unsigned pivotpoint = Xid[low + (high - low) / 2];				// pivotpoint ����.
		double pivotitem = X[pivotpoint];								// pivotpoint������ pivotitem.
		unsigned i = low - 1;											// 
		unsigned j = high + 1;											// �� ����� index.

		while (1) {														// ������ ������ �� �� ���� ��ü
			while (X[Xid[++i]] < pivotitem);							// pivot���� ���� ���� �ǳʶ�. (ũ�ų� ���� ���)
			while (X[Xid[--j]] > pivotitem);							// pivot���� ū ���� �ǳʶ�. (�۰ų� ���� ���)
			if (i >= j) break;											// 
			swap(&Xid[i], &Xid[j]);										// ������ ������ �� �� ���� ��ü.
		}
		quickSort(low, j, X, Xid);										// ���� ���� sort (���).
		quickSort(j + 1, high, X, Xid);									// ���� ���� sort (���).
	}
}

void selectionSort(unsigned L, unsigned R, double *Y, unsigned *Yid) {	// selection sort ���� �Լ�.
	unsigned minIdx;													// �ּҰ� index.
	for (unsigned i = L; i <= R; i++) {									// L ~ R ���� ���� Ž��.
		minIdx = i;														// ��ġ ����.
		for (unsigned j = i + 1; j <= R; j++) {							// i+1���� ���Һ��� ������ ��ġ�� ��.
			if (Y[Yid[j]] < Y[Yid[minIdx]])								// �� ���� �� �߽߰�, 
				minIdx = j;												// �ּҰ� ����.
		}
		swap(&Yid[i], &Yid[minIdx]);									// ���� index, �ּҰ� index swap.
	}
}

void sortXid(double *X, unsigned *Xid, unsigned *TMP, unsigned N) {		// Xid[]�� X[]���� ���� �����ϴ� �Լ�.
	quickSort(0, N - 1, X, Xid);										// quickSort�� ���� x��ǥ�� ���� ����.
}

void merge(double *Y, unsigned *Yid, unsigned *TMP, unsigned a1, unsigned b1, unsigned a2, unsigned b2) {
	// Input	: Y��ǥ �迭, Y index �迭
	//			  merge�� TMP �迭
	//			  ���� (a1 ~ b1), (a2 ~ b2)
	// Output	: (a1 ~ b1)�� (a2 ~ b2) ������ ������ merge�Ͽ� �迭 ��ȯ.
	
	unsigned i, j, k;													// 
	i = a1; j = a2; k = a1;												// index ���� �ʱ�ȭ.

	while (i <= b1 && j <= b2) {										// ���� index �� �� �ϳ��� ���� ������ �ݺ�.
		if (Y[Yid[i]] < Y[Yid[j]]) {									// ù��° ���� ���� ���� �� ���� ��,
			TMP[k] = Yid[i];											// �ش� ���� �켱 ����.
			i++;														// ù��° ���� index 1 ����.
		}
		else {															// �ι�° ���� ���� ���� �� ���� ��, 
			TMP[k] = Yid[j];											// �ش� ���� �켱 ����.
			j++;														// �ι�° ���� index 1 ����.
		}
		k++;															// merge ��� �迭 index 1 ����.
	}

	if (i > b1) {														// �ι�° ������ ���� ���� �������� ��
		for (unsigned t = 0; j + t <= b2; t++)							// 
			TMP[k + t] = Yid[j + t];									// �ռ� merge�� �� ���Ŀ� ���δ�.
	}
	else {																// ù��° ������ ���� ���� �������� ��
		for (unsigned t = 0; i + t <= b1; t++)							// 
			TMP[k + t] = Yid[i + t];									// �ռ� merge�� �� ���Ŀ� ���δ�.
	}

	for (unsigned t = a1; t <= b2; t++)									// TMP�� ����Ǿ� �ִ� merge�� ����� 
		Yid[t] = TMP[t];												// �ٽ� Yid�� �ǵ��� �ִ´�.
}

double combine(
	double *X, double *Y, unsigned *Xid, unsigned *Yid, unsigned *TMP, unsigned L, unsigned R,
	double d_LR, double x_mid, unsigned *pt1, unsigned *pt2, double d_min
) {
	// Input	: X[], Xid[], Y[], Yid[], TMP[], L, R
	//			  x_mid, d_LR, ... etc.
	// Output	: (x_mid - d_LR) ~ (x_mid + d_LR) ���� (�߰� ����) ���鿡 ���� y ��ǥ�� ���� ���� �� 
	//			  �� �� �ֱ��� ���� ���ϰ� �� �Ÿ��� ��ȯ�Ѵ�. 

	unsigned k = 0;														// TMP �迭 ����� index.
	for (unsigned i = L; i <= R; i++) {									// TMP[]�� �߰� ������ ������ �����Ѵ�.
		if (x_mid - d_LR <= X[Yid[i]] && X[Yid[i]] <= x_mid + d_LR) {	// �߰� ���� (x_mid �¿� d_LR ����)�� ���� ��ġ�Ѵٸ�, 
			TMP[k] = Yid[i];											// TMP[]�� y��ǥ�� ���� ������ ������� ���� �ִ´�.
			k++;
		}
	}
	if (k != 0) {														// TMP[]�� ���Ұ� �ϳ� �̻� ����Ǿ��� ��, 
		for (unsigned i = 0; i < k - 1; i++) {							// TMP ���� ���� �ֵ� �� �ּ� �Ÿ� ���� ã�´�. 
			for (unsigned j = i + 1; j < k; j++) {						// 
				if (Y[TMP[j]] - Y[TMP[i]] >= d_min) break;				// y ���� ���� d_min �̻��̸� �� �̻��� Ž�� ���ʿ�.
				
				double dis = sqrt(pow(X[TMP[i]] - X[TMP[j]], 2) + pow(Y[TMP[i]] - Y[TMP[j]], 2));
				
				if (dis < d_min) {										// �� ���� �� �߽߰�, 
					d_min = dis;										// �ּҰ� ����.
					*pt1 = TMP[i];										// 
					*pt2 = TMP[j];										// �� 2���� index ����.
				}
			}
		}
	}

	return d_min;														// �߰� �������� ����� ������ �ּ� �Ÿ� ��ȯ.
}

double closestPairDC(
	unsigned L, unsigned R,												// current leftmost and rightmost indices
	unsigned *pt1, unsigned *pt2,										// closest pair points indices
	double *X, double *Y,												// (x,y) positions array(input)
	unsigned *Xid,														// point index array(sorted by x coordinates)
	unsigned *Yid,														// point index array(gradually sorted by y coordinates)
	unsigned *TMP,														// temporal index array
	unsigned THR														// threshold value
) {
	double d_min;														// �ּ� �Ÿ� ����� ����.

	/* ----- Threshold���� ��� ���� ������ ���� ��� ----- */
	if (R - L + 1 <= THR) {
		for (unsigned i = L; i <= R; i++) {								// 
			Yid[i] = Xid[i];											// Xid[L:R]�� Yid[L:R]�� ����.
		}
		selectionSort(L, R, Y, Yid);									// selection sort (O(n^2) algorithm)�� y��ǥ�� ���� ����.
		d_min = DBL_MAX;												// d_min �� (���ſ�).

		for (unsigned i = L; i < R; i++) {								// brute force ����� ����Ͽ� closest pair�� ���Ѵ�. 
			for (unsigned j = i + 1; j <= R; j++) {						//
				if (Y[Yid[j]] - Y[Yid[i]] >= d_min) break;				// y ���� ���� d_min �̻��̸� �� �̻��� Ž�� ���ʿ�.
				
				double dis = sqrt(pow(X[Yid[i]] - X[Yid[j]], 2) + pow(Y[Yid[i]] - Y[Yid[j]], 2));
				
				if (dis < d_min) {										// �� ���� �� �߽߰�, 
					d_min = dis;										// �ּҰ� ����.
					*pt1 = Yid[i];										// 
					*pt2 = Yid[j];										// �� 2���� index ����.
				}
			}
		}

	}
	
	/* ----- Threshold���� ��� ���� ������ ū ��� ----- */
	else {
		unsigned pt_l1, pt_l2, pt_r1, pt_r2;
		unsigned M = (L + R) / 2;															// Middle = (Left + Right) / 2
		double d_L = closestPairDC(L, M, &pt_l1, &pt_l2, X, Y, Xid, Yid, TMP, THR);			// L ~ M������ �ֱ��� ���� ���Ѵ�. 
		double d_R = closestPairDC(M + 1, R, &pt_r1, &pt_r2, X, Y, Xid, Yid, TMP, THR);		// M+1 ~ R������ �ֱ��� ���� ���Ѵ�. 
																		
																		// Yid[L:M], Yid[M+1:R]�� �ش� ������ y��ǥ�� ���� ���ĵ�.
		merge(Y, Yid, TMP, L, M, M + 1, R);								// �� �迭�� merge�Ͽ� �ش� ������ y���� ���� ����.
		double d_LR;													// d_L, d_R �� �� ���� ��.
		if (d_L < d_R) {												// d_L�� �� �۴ٸ�, 
			(*pt1) = pt_l1;												// 
			(*pt2) = pt_l2;												// L ~ M������ �ֱ��� �� �� ����.
			d_LR = d_L;													// d_L ����.
		}
		else {															// �� ���� ��� (�ݴ�)
			(*pt1) = pt_r1;												// 
			(*pt2) = pt_r2;												// M+1 ~ R������ �ֱ��� �� �� ����.
			d_LR = d_R;													// d_R ����.
		}

		double x_mid = (X[Xid[M]] + X[Xid[M + 1]]) / 2;					// �߰� ���� ������ �߾Ӱ�.
		d_min = d_LR;													// d_min�� ���������� �ּ� �Ÿ� ����.
		unsigned pt_1, pt_2;											// �߰� ���� ���� �ֱ��� �� �� ����� ����.

		double d_c = combine(X, Y, Xid, Yid, TMP, L, R, d_LR, x_mid, &pt_1, &pt_2, d_min);
																		// �߰� ������ �ּ� �Ÿ� ��ȯ.
		if (d_c < d_min) {												// �߰� ������ �ּ� �Ÿ��� �� �۴ٸ�, 
			(*pt1) = (pt_1);											// 
			(*pt2) = (pt_2);											// �ֱ��� �� �� ����.
			d_min = d_c;												// �Ÿ� �ּҰ� ����.
		}
	}

	return d_min;														// ���� �ּ� �Ÿ� return.
}