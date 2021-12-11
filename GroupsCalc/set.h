#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>

#define MAX_BITS 128
#define MAX_SET_NUM 3
#define ARR_SIZE MAX_BITS/CHAR_BIT
#define MAX_LINE_LENGTH 80
#define TERM_OP "-1" /*termination operand*/

typedef struct set{
	unsigned char m_group[ARR_SIZE];
} set;

typedef struct sets{
	char* m_name;
	set* m_set;
} sets;
	
/*initiates the set in the sets array to the empty group*/
void initiatSets(sets* _setsArr);

/*update the set to the empty group*/
void resetSet(set* _set);

/*read the numbers(bits positions) to the set and update these position to value 1*/
void read_set(set* _set, int* const _params, int _numParams);

/*print the given set in a 16digits row format. If the set (group) is empty prints "The set is empty"*/
void print_set(set* _set);

/*unioun group A and group B and store the output in group C*/
void union_set(set* _setA, set* _setB, set* _setC);

/*intersect group A and group B and store the output in group C*/
void intersect_set(set* _setA, set* _setB, set* _setC);

/*subtract group B from group A and store the output in group C*/
void sub_set(set* _setA, set* _setB, set* _setC);

/*symdiff group A and group B and store the output in group C*/
void symdiff_set(set* _setA, set* _setB, set* _setC);

/*stop the program - exit*/
void stop();

