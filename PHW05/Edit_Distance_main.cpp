#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void Edit_Distance ( 
	// inputs
	char *SS,		// Source string array
	char *TS,		// Edited string array
	int ins_cost, int del_cost, int sub_cost,	
					// insertion, deletion, substitution cost
	// outputs
	int **Table,    // Cost Table Calculated (2-d array (|SS|+1) x (|TS|+1) will be set)
	char **SR,		// Source string with insertion to T marks '*'
	char **OP,		// Operation sequence('i', 'd', 's', or '.' (nop))
	char **TR,       // Edited string with deletion form S mark '*'
					// NOTE: ST, OP, TR must be allocated inside the function as needed(minimum as possible)
	int  *Mem_Allocated
);

int Test_Edit ( char *SS, char *TS, char *SR, char *TR, char *OP, int ED_cost, int MAX_STR_SIZE,
			   int ins_cost, int del_cost, int sub_cost);

void Error_Exit ( char *s );

void main ( void ) {

	char *SS, *TS;
	char *SR, *TR, *OP;
	int  Tnum, SN, TN;
	int  MAX_STR_SIZE, INS_COST, DEL_COST, SUB_COST;
	int  **Table;
	clock_t start, finish;
	double cmpt;
	int  Max_Memory_Used_In_Edit_Distance = 0;
	int  Mem_Allocated, test_mem_size, test_max_mem_size = 0;

	start = clock();	// reset timer

	// Obtain max table size by checking every test cases
	scanf_s("%d %d", &Tnum, &MAX_STR_SIZE);
	scanf_s("%d %d %d", &INS_COST, &DEL_COST, &SUB_COST);
	// Allocate memory for input strings S and T
	SS = new char [MAX_STR_SIZE + 1];
	TS = new char [MAX_STR_SIZE + 1];
	if ( SS == NULL || TS == NULL ) {
		Error_Exit("Memory Allocation Error");
	}

	// Now scan input string to calculate the Table size
	scanf_s("%s %s", SS, MAX_STR_SIZE + 1, TS, MAX_STR_SIZE + 1);
	SN = strlen(SS);
	TN = strlen(TS);
	for (int ts = 1; ts < Tnum; ts++ ) {
		scanf_s("%d %d %d", &INS_COST, &DEL_COST, &SUB_COST);
		scanf_s("%s %s", SS, MAX_STR_SIZE + 1, TS, MAX_STR_SIZE + 1);
		if ( SN < (int)strlen(SS) ) {
			SN = (int)strlen(SS);     // max size of S
		}
		if ( TN < (int)strlen(TS) ) {
			TN = (int)strlen(TS);     // max size of T
		}
	}

	// Allocate memory for the cost table
	Table = new int* [SN + 1]; // Size must be 1 larger than SN since null string
	if ( Table == NULL ) {
		Error_Exit("Memory Allocation Error!");
	}
	for ( int i = 0; i <= SN; i++ ) {
	  Table[i] = new int [TN + 1];
	  if ( Table[i] == NULL ) {
		  Error_Exit("Memory Allocation Error!");
	  }
	}

	// rewind input
	rewind(stdin);

	// Start reading input again
	scanf_s("%d %d", &Tnum, &MAX_STR_SIZE); // read the num of test cases and max string size
	printf("Edit Distance Results (max possible input string size = %d)\n", MAX_STR_SIZE);
	// Init Output String Pointers
	SR = OP = TR = NULL;
	// Process pair of strings
	for (int ts = 0; ts < Tnum; ts++ ) {
		scanf_s("%d %d %d", &INS_COST, &DEL_COST, &SUB_COST);
		scanf_s("%s %s", SS, MAX_STR_SIZE + 1, TS, MAX_STR_SIZE + 1);

		Edit_Distance ( SS, TS, INS_COST, DEL_COST, SUB_COST, Table, &SR, &OP, &TR, &Mem_Allocated );

		printf("**Test Case %3d (i = %d, d = %d, s = %d) : OP cost = %4d\n", ts, 
			INS_COST, DEL_COST, SUB_COST, Table[strlen(SS)][strlen(TS)]);
		test_mem_size = Test_Edit ( SS, TS, SR, TR, OP, Table[strlen(SS)][strlen(TS)], MAX_STR_SIZE,
			INS_COST, DEL_COST, SUB_COST );
		if ( test_max_mem_size < test_mem_size ) {
			test_max_mem_size = test_mem_size;
		}
		printf("S  = %s\n", SR);
		printf("OP = %s\n", OP);
		printf("T  = %s\n", TR);
		if ( Max_Memory_Used_In_Edit_Distance < Mem_Allocated ) {
			Max_Memory_Used_In_Edit_Distance = Mem_Allocated;
		}
		delete [] SR;
		delete [] OP;
		delete [] TR;
		SR = OP = TR = NULL;
	}	// end of all the test cases

	// deallocate storage for input strings
	delete [] SS;
	delete [] TS;

	// deallocate storage for the cost table
	for ( int i = 0; i <= SN; i++ ) {
	  delete [] Table[i];
	}
	delete [] Table;

	finish = clock();
	cmpt = ((double)(finish - start)) / (double)CLK_TCK;
	if ( test_max_mem_size < Max_Memory_Used_In_Edit_Distance ) {
		printf("Warning: memory used(%d bytes) is larger than required(%d bytes).\n",
			Max_Memory_Used_In_Edit_Distance, test_max_mem_size);
	}
	printf("\nTotal Elapsed Time = %.3f sec, Max Memory Used = %d bytes", cmpt,
		Max_Memory_Used_In_Edit_Distance);
}

int Test_Edit ( char *SS, char *TS, char *SR, char *TR, char *OP, int ED_cost, int MAX_STR_SIZE,
			   int ins_cost, int del_cost, int sub_cost) {
	char *TRS;
	int ON = strlen(OP);
	int s, t, cost = 0;
	int mem_size;
	bool flag;

	// check if SS and SR have the same char string
	for ( s = 0, t = 0, flag = true; t < (int)strlen(SR); t++ ) {
		if ( SR[t] == '*' ) {
			continue;
		}
		if ( SS[s] != SR[t] ) {
			flag = false;
			break;
		}
		s++;
	}
	if ( flag == false ) {
		printf("ERROR: The original input S != SR\n");
	}

	// check if TS and TR have the same char string
	for ( s = 0, t = 0, flag = true; t < (int)strlen(TR); t++ ) {
		if ( TR[t] == '*' ) {
			continue;
		}
		if ( TS[s] != TR[t] ) {
			flag = false;
			break;
		}
		s++;
	}
	if ( flag == false ) {
		printf("ERROR: The original input T != TR\n");
	}

	TRS = new char [MAX_STR_SIZE + 1];
	if ( TRS == NULL ) {
		Error_Exit("Memory Allocation Error");
	}
	flag = true;
	for ( s = 0, t = 0; s < ON; s++ ) {
		if ( OP[s] == '.' ) {
			if ( SR[s] != TR[s] ) { // must be the same
				flag = false;
				break;
			}
			TRS[t] = SR[s];
			t++;
		}
		else if ( OP[s] == 'i' ) {
			if ( !(SR[s] == '*' && TR[s] != '*') ) { // SR[s] must be '*'
				flag = false;
				break;
			}
			TRS[t] = TR[s];
			cost += ins_cost;
			t++;
		}
		else if ( OP[s] == 'd' ) {
			if ( !(SR[s] != '*' && TR[s] == '*') ) { // TR[s] must be '*'
				flag = false;
				break;
			}
			cost += del_cost;
			// no copy
		}
		else if ( OP[s] == 's' ) {
			if ( !(SR[s] != '*' && TR[s] != '*' && SR[s] != TR[s]) ) { // SR[s], TR[s] must not be '*' and
				flag = false;										   // SR[s] != TR[s]
				break;
			}
			TRS[t] = TR[s];
			cost += sub_cost;
			t++;
		}
	}
	TRS[t] = '\0';
	if ( flag == false ) {
		printf("ERROR: Wrong edit sequence(SR, TR or OP!\n");
	}
	if ( strcmp(TS, TRS) ) {
		printf("ERROR: Cannot convert S to T using the edit sequnce.\n");
		printf("  Converted string = \"%s\"\n",TRS);
	}
	if ( cost != ED_cost ) {
		printf("ERROR: Calculated edit distance != edit distance from OP[].\n");
	}

	delete [] TRS;

	mem_size = (int)(strlen(SR) + strlen(TR) + strlen(OP)) + 3;

	return mem_size;
}

void Error_Exit ( char *s ) {
  printf("%s\n", s);
  exit(-1);
}