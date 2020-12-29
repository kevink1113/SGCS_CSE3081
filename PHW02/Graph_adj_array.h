#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NONE -1

typedef struct _edge {
	int  name;
	int  vf, vr;  // e = (vf, vr) where vf, vr are vertex array indices
	int  cost;    // edge cost
	bool flag;    // true if in an SP, false otherwise 
	int  fp, rp;  // adj list ptr of forward and reverse (-1 if none)
} edge;

typedef struct _vertex {
	int  name;
	bool flag;
	int  f_hd, r_hd; // adj list header(-1 if none)
} vertex;
