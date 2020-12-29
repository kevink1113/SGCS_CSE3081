#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define NONE      -1

extern void Error_Exit(const char *s);
//
// Functions for Doubly Linked List for Graph Adjacent List
//
typedef struct _DBL {	// Adjacent list element structure
	int d;
	struct _DBL *twin;	// the other DBL pointer
	struct _DBL *lp, *rp;
} DBL;

class DBList {
private:
	DBL *DBL_pool;
public:
	unsigned long DBL_cnt;
	unsigned long UsedMemoryForDBLs;

	DBList(void) {
		DBL_pool = NULL;
		DBL_cnt = 0;
		UsedMemoryForDBLs = 0;
	}
	~DBList(void) {
	}

	DBL *allocDBL(void);
	void freeDBL(DBL *p);
	void freeDBL_pool(void);
};

class dblStack {
private:
	DBL *ST;
public:
	dblStack(void) {
		ST = NULL;
	}
	~dblStack(void) {
	}
	void  push(DBL *p);
	DBL  *pop(void);
	void  remove(DBL *p);

	DBL  *top(void);
	bool  empty(void);  // true if the stack is empty
};