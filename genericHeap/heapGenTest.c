/* 
 * File: heapGenFuncTests.c
 * Functions for Generic Heap module. Tests Automation.
 */ 

#include <stdio.h>
#include <stdlib.h>


#include "heapGenTest.h"

#define TRUE 1
#define FALSE 0

/*
enum Heap_ResultCode:

	HEAP_SUCCESS = 0,
	HEAP_NOT_INITIALIZED,
	HEAP_REALLOCATION_FAILED,
	HEAP_UNDER_FLOW,
	HEAP_HEAPIFY_SUCCESS,
	HEAP_INSERT_ERROR
*/


void PrintTestsResult(char _name[], AutoTestResults _testResult, Heap_ResultCode* _status)
{
	printf("\nTest name: %s \tResult: %s \tstatus: %d\n", _name, (_testResult == SUCCESS)? "Passed" : "Failed!", *_status);					
}


Vector* createVectorForHeap(size_t _blockSize)
{
	Vector* myVec;
	int i = 0;
	int arr[] = {52, 15, 6, 20, 8, 45, 6, 3, 25, 37};
	size_t numOfItems = 10;
	
	myVec = Vector_Create(numOfItems, _blockSize);
	
	for(i=0; i<numOfItems; i++) 
	{
		Vector_Append(myVec, &arr[i]); /*need to check result error*/
	}
	printf("\n*********\n");
		VectorPrint(myVec);
	return myVec;
}


Vector* CreateRandomVector(size_t _blockSize)
{
	int i;
	int myArr[10];
	Vector* vector;

	vector = Vector_Create(10,_blockSize);
	for (i=0 ; i<10 ; i++)
	{
		myArr[i] = rand()/100000000;
		Vector_Append(vector, myArr+i);	
	}
	return vector;
}


/*---------------------------------------------------------------------------------------------------------------*/	
AutoTestResults Heap_BuildTest1(Heap_ResultCode* _status) /* build heap */
{
	Vector* myVec;
	Heap* myHeap;
	
	size_t numOfItems, i, heapSize;

	int arr[] = {52, 15, 6, 20, 8, 45, 6, 3, 25, 37};
	numOfItems = 10;
	
	myVec = Vector_Create(numOfItems, 0);
	
	for(i=0; i<numOfItems; i++) 
	{
		Vector_Append(myVec, &arr[i]); /*need to check result error*/
	}
	
	myHeap = Heap_Build(myVec, LessComparatorFunc);
	heapSize = Heap_Size(myHeap);
	
	if(myHeap != NULL && heapSize == numOfItems)
	{
		*_status = HEAP_SUCCESS;
		Heap_Destroy(&myHeap);
		return SUCCESS;
	}

	return FAIL;
}


AutoTestResults Heap_BuildTest2(Heap_ResultCode* _status) /* build heap with NULL vector */
{
	Heap* myHeap;
	size_t heapSize;

	myHeap = Heap_Build(NULL, LessComparatorFunc);
	heapSize = Heap_Size(myHeap);
	
	if(myHeap == NULL && heapSize == 0)
	{
		*_status = HEAP_SUCCESS;
		Heap_Destroy(&myHeap);
		return SUCCESS;
	}

	return FAIL;
}


/*---------------------------------------------------------------------------------------------------------------*/	
AutoTestResults Heap_DestroyTest1(Heap_ResultCode* _status) /* heap destroy */
{
	Vector* myVec;
	Heap* myHeap;
	size_t numOfItems, i;
	size_t heapSizeBefore;
	size_t heapSizeAfter;

	int arr[] = {52, 15, 6, 20, 8, 45, 6, 3, 25, 37};
	numOfItems = 10;
	
	myVec = Vector_Create(numOfItems, 0);
	
	for(i=0; i<numOfItems; i++) 
	{
		Vector_Append(myVec, &arr[i]); /*need to check result error*/
	}
	myHeap = Heap_Build(myVec, LessComparatorFunc);
	heapSizeBefore = Heap_Size(myHeap);
	Heap_Destroy(&myHeap);
	heapSizeAfter = Heap_Size(myHeap);
	
	if(myHeap == NULL && heapSizeBefore == 10 && heapSizeAfter == 0)
	{
		return SUCCESS;
	}

	return FAIL;
}


AutoTestResults Heap_DestroyTest2(Heap_ResultCode* _status) /* heap destroy twice*/
{
	Heap* myHeap;
	size_t heapSizeBefore;
	size_t heapSizeAfter;
	Vector* myVec = NULL;

	myVec = createVectorForHeap(0);
	myHeap = Heap_Build(myVec, LessComparatorFunc);
		printf("\n*********heap\n");
	heapSizeBefore = Heap_Size(myHeap);
	Heap_Destroy(&myHeap);
	Heap_Destroy(&myHeap);
	heapSizeAfter = Heap_Size(myHeap);
	
	if(myHeap == NULL && heapSizeBefore == 10 && heapSizeAfter == 0)
	{
		return SUCCESS;
	}

	return FAIL;
}


/*---------------------------------------------------------------------------------------------------------------*/	

AutoTestResults Heap_InsertTest1(Heap_ResultCode* _status) /* insert node to heap */
{
	Vector* myVec;
	Heap* myHeap;
	size_t heapSizeBefore;
	size_t heapSizeAfter;
	int item;
	int* result = NULL;
	item = 23;
	
	myVec = Vector_Create(10, 2);
	myHeap = Heap_Build(myVec, LessComparatorFunc);
	heapSizeBefore = Heap_Size(myHeap);
	Heap_Insert(myHeap, (void*) &item);
	printf("\nPrint foreach: ");
	Heap_ForEach(myHeap, ActionFunc, NULL);
	heapSizeAfter = Heap_Size(myHeap);
	result = (int*) Heap_Peek(myHeap);
	printf("\nmy heap: %d, result: %d, before %ld, after %ld", myHeap, *result, heapSizeBefore, heapSizeAfter);
	if(myHeap != NULL && heapSizeBefore == 0 && heapSizeAfter == 1 && *result == 23)
	{
		Heap_Destroy(&myHeap);
		return SUCCESS;
	}

	return FAIL;
}


AutoTestResults Heap_InsertTest2(Heap_ResultCode* _status) /* insert severl nodes to heap */
{
	Vector* myVec;
	Heap* myHeap;
	size_t heapSizeBefore;
	size_t heapSizeAfter;
	int itemA, itemB, itemC, itemD, itemE, itemF;
	int* result = NULL;
	itemA = 23;
	itemB = 12;
	itemC = 68;
	itemD = 7;
	itemE = 50;
	itemF = 32;
	
	myVec = Vector_Create(10, 2);
	myHeap = Heap_Build(myVec, LessComparatorFunc);
	heapSizeBefore = Heap_Size(myHeap);
	Heap_Insert(myHeap, (void*) &itemA);
	Heap_Insert(myHeap, (void*) &itemB);
	Heap_Insert(myHeap, (void*) &itemC);
	Heap_Insert(myHeap, (void*) &itemD);
	Heap_Insert(myHeap, (void*) &itemE);
	Heap_Insert(myHeap, (void*) &itemF);
	printf("\nPrint foreach: ");
	Heap_ForEach(myHeap, ActionFunc, NULL);
	heapSizeAfter = Heap_Size(myHeap);	
	result = (int*) Heap_Peek(myHeap);
	
	if(myHeap != NULL && heapSizeBefore == 0 && heapSizeAfter == 6 && *result == itemC)
	{
		Heap_Destroy(&myHeap);
		return SUCCESS;
	}

	return FAIL;
}

AutoTestResults Heap_ForEachTest1(Heap_ResultCode* _status) /* print heap after build with for each */
{
	Vector* myVec;
	Heap* myHeap;
	size_t heapSizeBefore;
	size_t heapSizeAfter;
int itemA;
	itemA = 45;
	myVec = createVectorForHeap(2);
	VectorPrint(myVec);
	myHeap = Heap_Build(myVec, LessComparatorFunc);
		Heap_Insert(myHeap, (void*) &itemA);
	heapSizeBefore = Heap_Size(myHeap);
	printf("\nPrint foreach: ");
	Heap_ForEach(myHeap, ActionFunc, NULL);
	Heap_Destroy(&myHeap);
	heapSizeAfter = Heap_Size(myHeap);
	
	if(myHeap == NULL && heapSizeBefore == 11 && heapSizeAfter == 0)
	{
		return SUCCESS;
	}

	return FAIL;
}
