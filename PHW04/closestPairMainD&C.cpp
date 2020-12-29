#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define ERROR_BOUND 1.0e-30

void   errorExit(const char *s);

void   sortXid (double *X, unsigned *Xid, unsigned *TMP, unsigned N);
  // input  : X[]   (x position array of N points)
  //          Xid[] (index array of X (initially [0,1,...,N-1])
  //          TMP[] (temporary array of size N. may or may not be used)
  //          N   number of points to sort
  // output : Xid[] (sorted indices by x position in X[])

double closestPairDC(
	unsigned L, unsigned R,	  // current leftmost and rightmost indices
	unsigned *pt1, unsigned *pt2, // closest pair points indices
	double *X, double *Y,         // (x,y) positions array(input)
	unsigned *Xid,  // point index array(sorted by x coordinates)
	unsigned *Yid,  // point index array(gradually sorted by y coordinates)
    unsigned *TMP,  // temporal index array
	unsigned THR	// threshold value
);
  // Input : L, R, X[], Y[], Xid[]
  // Output : pt1, pt2 ( the distance between (X[pt1],Y[pt1]) and
  //                     (X[pt2],Y[pt2]) is the closest )
  //          return the distance value
  // Others ; Yid[], TMP[] (scratch arrays for D&C algorithm)

int main ( void ) {
    int TNUM, THR;
	unsigned N, pt1, pt2;
	double cmpt, cd; // cd : closest distance
	double cd1;      // distance between pt1 and pt2(for verification)
	clock_t start, finish;
	double *X, *Y;
	unsigned *Xid, *Yid, *TMP;

	scanf_s("%d", &TNUM); // read the number of tests
	for ( int t = 0; t < TNUM; t++ ) {  // for each test case
		scanf_s("%u %d", &N, &THR); // read point number and threshold
		X  = new double [N];        
		Y = new double [N];  // allocate arrays
		Xid = new unsigned [N]; 
		Yid = new unsigned [N];
		TMP = new unsigned [N];
		if ( X == NULL || Y == NULL || Xid == NULL || Yid == NULL || TMP == NULL ) {
			errorExit("Memory allocation error!");
		}
		for (unsigned i = 0; i < N; i++) { // read points
			scanf_s("%le %le",&X[i], &Y[i]); 
			Xid[i] = i; // set indices of points
		}

		start = clock();
		// ** Apply the D&C algorithm
			sortXid ( X, Xid, TMP, N ); // sort point indices by x-coords
			cd = closestPairDC(0, N-1, &pt1, &pt2, X, Y, Xid, Yid, TMP, THR);
		// **End of thr algorithm
		finish = clock();

		cmpt = ((double)(finish - start)) / CLK_TCK;
		if ( t != 0 ) 
			printf("\n");

		printf("**Test Case %d (N = %u, THR = %d, time = %.3f sec)", t+1, N, THR, cmpt);

		cd1 = sqrt((Y[pt1] - Y[pt2])*(Y[pt1] - Y[pt2]) + (X[pt1] - X[pt2])*(X[pt1] - X[pt2]));
		if ( fabs(cd - cd1) <= ERROR_BOUND ) {
			printf("\npt pair = (%u, %u), min_dist = %23.16le", pt1, pt2, cd);
		}
		else {
			printf("\npt pair = (%u, %u), min_dist = %23.16le, ERROR: dist(pt1,pt2) != cd.", pt1, pt2, cd);
		}
		delete[] X; delete[] Y;
		delete[] Xid; delete[] Yid; delete[] TMP;
	}
	return 0;
}

void errorExit(const char *s) {
	printf("%s", s);
	exit(-100);
}
