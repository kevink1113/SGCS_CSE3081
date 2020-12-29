#define _CRT_SECURE_NO_WARNINGS
using namespace std;
#include <stdio.h>
#include <stdlib.h>
#include <limits>
#include <cmath>

void swap(unsigned* a, unsigned* b) {									// 입력으로 들어온 두 수를 swap하는 함수.
	unsigned tmp = (*a);												// 임시 변수 tmp에 (*a) 값 저장.												
	(*a) = (*b);														// (*a) 값을 (*b) 값으로, 
	(*b) = tmp;															// (*b) 값을 tmp (원래 (*a)값)으로.
}

void quickSort(unsigned low, unsigned high, double *X, unsigned *Xid) {	// QuickSort 수행 함수.
	if (low < high) {													// low < high일때까지만 수행 (partition)
		unsigned pivotpoint = Xid[low + (high - low) / 2];				// pivotpoint 설정.
		double pivotitem = X[pivotpoint];								// pivotpoint에서의 pivotitem.
		unsigned i = low - 1;											// 
		unsigned j = high + 1;											// 비교 연산용 index.

		while (1) {														// 엇갈린 상태일 때 값 서로 교체
			while (X[Xid[++i]] < pivotitem);							// pivot보다 작은 경우는 건너뜀. (크거나 같은 경우)
			while (X[Xid[--j]] > pivotitem);							// pivot보다 큰 경우는 건너뜀. (작거나 같은 경우)
			if (i >= j) break;											// 
			swap(&Xid[i], &Xid[j]);										// 엇갈린 상태일 때 값 서로 교체.
		}
		quickSort(low, j, X, Xid);										// 좌측 범위 sort (재귀).
		quickSort(j + 1, high, X, Xid);									// 우측 번위 sort (재귀).
	}
}

void selectionSort(unsigned L, unsigned R, double *Y, unsigned *Yid) {	// selection sort 수행 함수.
	unsigned minIdx;													// 최소값 index.
	for (unsigned i = L; i <= R; i++) {									// L ~ R 범위 선형 탐색.
		minIdx = i;														// 위치 선택.
		for (unsigned j = i + 1; j <= R; j++) {							// i+1번쨰 원소부터 선택한 위치와 비교.
			if (Y[Yid[j]] < Y[Yid[minIdx]])								// 더 작은 값 발견시, 
				minIdx = j;												// 최소값 갱신.
		}
		swap(&Yid[i], &Yid[minIdx]);									// 선택 index, 최소값 index swap.
	}
}

void sortXid(double *X, unsigned *Xid, unsigned *TMP, unsigned N) {		// Xid[]를 X[]값에 따라 정렬하는 함수.
	quickSort(0, N - 1, X, Xid);										// quickSort를 통해 x좌표에 대해 정렬.
}

void merge(double *Y, unsigned *Yid, unsigned *TMP, unsigned a1, unsigned b1, unsigned a2, unsigned b2) {
	// Input	: Y좌표 배열, Y index 배열
	//			  merge용 TMP 배열
	//			  범위 (a1 ~ b1), (a2 ~ b2)
	// Output	: (a1 ~ b1)과 (a2 ~ b2) 범위의 값들을 merge하여 배열 반환.
	
	unsigned i, j, k;													// 
	i = a1; j = a2; k = a1;												// index 변수 초기화.

	while (i <= b1 && j <= b2) {										// 범위 index 둘 중 하나가 끝날 때까지 반복.
		if (Y[Yid[i]] < Y[Yid[j]]) {									// 첫번째 범위 안의 수가 더 작을 때,
			TMP[k] = Yid[i];											// 해당 수를 우선 저장.
			i++;														// 첫번째 범위 index 1 증가.
		}
		else {															// 두번째 범위 안의 수가 더 작을 때, 
			TMP[k] = Yid[j];											// 해당 수를 우선 저장.
			j++;														// 두번째 범위 index 1 증가.
		}
		k++;															// merge 결과 배열 index 1 증가.
	}

	if (i > b1) {														// 두번째 범위에 아직 수가 남아있을 때
		for (unsigned t = 0; j + t <= b2; t++)							// 
			TMP[k + t] = Yid[j + t];									// 앞서 merge한 값 이후에 붙인다.
	}
	else {																// 첫번째 범위에 아직 수가 남아있을 때
		for (unsigned t = 0; i + t <= b1; t++)							// 
			TMP[k + t] = Yid[i + t];									// 앞서 merge한 값 이후에 붙인다.
	}

	for (unsigned t = a1; t <= b2; t++)									// TMP에 저장되어 있던 merge된 결과를 
		Yid[t] = TMP[t];												// 다시 Yid로 되돌려 넣는다.
}

double combine(
	double *X, double *Y, unsigned *Xid, unsigned *Yid, unsigned *TMP, unsigned L, unsigned R,
	double d_LR, double x_mid, unsigned *pt1, unsigned *pt2, double d_min
) {
	// Input	: X[], Xid[], Y[], Yid[], TMP[], L, R
	//			  x_mid, d_LR, ... etc.
	// Output	: (x_mid - d_LR) ~ (x_mid + d_LR) 범위 (중간 영역) 점들에 대해 y 좌표에 대해 정렬 후 
	//			  그 중 최근접 쌍을 구하고 그 거리를 반환한다. 

	unsigned k = 0;														// TMP 배열 저장용 index.
	for (unsigned i = L; i <= R; i++) {									// TMP[]에 중간 영역의 점들을 저장한다.
		if (x_mid - d_LR <= X[Yid[i]] && X[Yid[i]] <= x_mid + d_LR) {	// 중간 영역 (x_mid 좌우 d_LR 범위)에 점이 위치한다면, 
			TMP[k] = Yid[i];											// TMP[]에 y좌표에 대해 정렬한 순서대로 값을 넣는다.
			k++;
		}
	}
	if (k != 0) {														// TMP[]에 원소가 하나 이상 저장되었을 때, 
		for (unsigned i = 0; i < k - 1; i++) {							// TMP 안의 점의 쌍들 중 최소 거리 쌍을 찾는다. 
			for (unsigned j = i + 1; j < k; j++) {						// 
				if (Y[TMP[j]] - Y[TMP[i]] >= d_min) break;				// y 값의 차가 d_min 이상이면 더 이상의 탐색 불필요.
				
				double dis = sqrt(pow(X[TMP[i]] - X[TMP[j]], 2) + pow(Y[TMP[i]] - Y[TMP[j]], 2));
				
				if (dis < d_min) {										// 더 작은 값 발견시, 
					d_min = dis;										// 최소값 갱신.
					*pt1 = TMP[i];										// 
					*pt2 = TMP[j];										// 점 2개의 index 저장.
				}
			}
		}
	}

	return d_min;														// 중간 영역까지 고려한 이후의 최소 거리 반환.
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
	double d_min;														// 최소 거리 저장용 변수.

	/* ----- Threshold보다 대상 점의 개수가 작은 경우 ----- */
	if (R - L + 1 <= THR) {
		for (unsigned i = L; i <= R; i++) {								// 
			Yid[i] = Xid[i];											// Xid[L:R]을 Yid[L:R]로 복사.
		}
		selectionSort(L, R, Y, Yid);									// selection sort (O(n^2) algorithm)로 y좌표에 대해 정렬.
		d_min = DBL_MAX;												// d_min 값 (갱신용).

		for (unsigned i = L; i < R; i++) {								// brute force 방법을 사용하여 closest pair를 구한다. 
			for (unsigned j = i + 1; j <= R; j++) {						//
				if (Y[Yid[j]] - Y[Yid[i]] >= d_min) break;				// y 값의 차가 d_min 이상이면 더 이상의 탐색 불필요.
				
				double dis = sqrt(pow(X[Yid[i]] - X[Yid[j]], 2) + pow(Y[Yid[i]] - Y[Yid[j]], 2));
				
				if (dis < d_min) {										// 더 작은 값 발견시, 
					d_min = dis;										// 최소값 갱신.
					*pt1 = Yid[i];										// 
					*pt2 = Yid[j];										// 점 2개의 index 저장.
				}
			}
		}

	}
	
	/* ----- Threshold보다 대상 점의 개수가 큰 경우 ----- */
	else {
		unsigned pt_l1, pt_l2, pt_r1, pt_r2;
		unsigned M = (L + R) / 2;															// Middle = (Left + Right) / 2
		double d_L = closestPairDC(L, M, &pt_l1, &pt_l2, X, Y, Xid, Yid, TMP, THR);			// L ~ M에서의 최근접 점을 구한다. 
		double d_R = closestPairDC(M + 1, R, &pt_r1, &pt_r2, X, Y, Xid, Yid, TMP, THR);		// M+1 ~ R에서의 최근접 점을 구한다. 
																		
																		// Yid[L:M], Yid[M+1:R]이 해당 점들의 y좌표에 의해 정렬됨.
		merge(Y, Yid, TMP, L, M, M + 1, R);								// 두 배열을 merge하여 해당 점들의 y값에 대해 정렬.
		double d_LR;													// d_L, d_R 중 더 작은 값.
		if (d_L < d_R) {												// d_L이 더 작다면, 
			(*pt1) = pt_l1;												// 
			(*pt2) = pt_l2;												// L ~ M에서의 최근접 점 쌍 저장.
			d_LR = d_L;													// d_L 대입.
		}
		else {															// 그 외의 경우 (반대)
			(*pt1) = pt_r1;												// 
			(*pt2) = pt_r2;												// M+1 ~ R에서의 최근접 점 쌍 저장.
			d_LR = d_R;													// d_R 대입.
		}

		double x_mid = (X[Xid[M]] + X[Xid[M + 1]]) / 2;					// 중간 영역 점들의 중앙값.
		d_min = d_LR;													// d_min에 최종적으로 최소 거리 저장.
		unsigned pt_1, pt_2;											// 중간 영역 중의 최근접 점 쌍 저장용 변수.

		double d_c = combine(X, Y, Xid, Yid, TMP, L, R, d_LR, x_mid, &pt_1, &pt_2, d_min);
																		// 중간 영역의 최소 거리 반환.
		if (d_c < d_min) {												// 중간 영역의 최소 거리가 더 작다면, 
			(*pt1) = (pt_1);											// 
			(*pt2) = (pt_2);											// 최근접 점 쌍 갱신.
			d_min = d_c;												// 거리 최소값 갱신.
		}
	}

	return d_min;														// 최종 최소 거리 return.
}