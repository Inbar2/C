
/* 
 * File: heapGenMainTests.c
 * Main for Generic Heap module Tests Automation.
 */ 

#include <stdio.h>
#include <stdlib.h>

#include "heapGenTest.h"

/* compare element data with context */
int	LessComparatorFunc(const void* _left, const void* _right)
{
	
	if(NULL == _left || NULL == _right)
	{
		return 0;
	}
	
	printf("left:%d\t right:%d\n", *((int*)_left), *((int*)_right));
	return ((*((int*)_left) > *((int*)_right)) ? TRUE: FALSE);
}


/*context is the node data and _element is the node to print
the key is not allowed to be changed*/
int	ActionFunc(const void* _elem, void* _context)
{
	if(_elem == NULL)
	{
		return FALSE;
	}
	
	printf("%d, ", *((int*) _elem));
	return TRUE;
}


int main()
{
	Heap_ResultCode status;
	
	PrintTestsResult("Heap_BuildTest1", Heap_BuildTest1(&status), &status);
	PrintTestsResult("Heap_BuildTest2", Heap_BuildTest2(&status), &status);
	
	PrintTestsResult("Heap_DestroyTest1", Heap_DestroyTest1(&status), &status);
	PrintTestsResult("Heap_DestroyTest2", Heap_DestroyTest2(&status), &status);
	

	PrintTestsResult("Heap_InsertTest1", Heap_InsertTest1(&status), &status);
	PrintTestsResult("Heap_InsertTest2", Heap_InsertTest2(&status), &status);
	PrintTestsResult("Heap_ForEachTest1", Heap_ForEachTest1(&status), &status);



	return 0;
}
