#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <list>

#define NONE      -1

void errorExit(const char *s);

typedef struct _SLL {
	int i;
	struct _SLL *p;
} SLL;

class SLList2 {
private:
	SLL *SLL_pool;
public:
	SLList2(void) {
		SLL_pool = NULL;
	}
	~SLList2(void) {
	}
	unsigned long SLL_cnt;
	unsigned long UsedMemoryForSLLs;

	SLL *allocSLL(void);
	void freeSLL(SLL *p);
	void freeSLL_pool(void);
};

//class sllStack :public SLList {
class sllStack2 {
private:
	SLL *ST;
public:
	sllStack2(void) {
		ST = NULL;
	}
	~sllStack2(void) {
	}
	void  push(SLL *p);
	SLL  *pop(void);
	SLL  *top(void);
	bool  empty(void);  // true is the stack is empty
};

typedef struct vertex {
	int		  name; // may have more information
	bool	  flag;
	sllStack2  S;    // adjacent list as a stack
} vertex;

typedef struct edge {
	int    name;  // may have more information
	int    cost;
	bool   flag;
	int    vf, vr;
} edge;

// Graph_adj_list_tree_check.cpp
int  Tree_Check_adj_list(int Vnum, vertex *V, int Enum, edge *E);

// Graph_adj_list_data_in.cpp
void Read_Graph_adj_list(int Vnum, vertex *V, int Enum, edge *E);
void Free_Graph_adj_list(int Vnum, vertex *V);

// BFS spanning tree generation
int BFS_Tree_adj_list(
	int     src,   // source node index
	int     Vnum,  // number of vertices
	vertex *V,     // vertex structure array (starting index 0)
	int     Enum,  // number of edges
	edge   *E     // edge structure array (starting index 0)
);