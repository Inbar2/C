/* 
 * File: genHeapMain.c
 * Main for Generic Heap module.
 * Author: Inbar Barhum 
 * Date: 16.1.2019
 */ 


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "genHeap.h"
#define FALSE 0
#define TRUE 1

/* Run path: ~/work/advenceC/genericHeap$ gcc -g -Wall -ansi -pedantic -I../includeGen ../genericVector/vectorGenericFunc.c genHeapFunc.c genHeapMain.c */


int	LessComparatorFunc(const void* _left, const void* _right)
{
	
	
	if(NULL == _left || NULL == _right)
	{
		return 0;
	}
	
	printf("left:%d\t right:%d\n", *((int*)_left), *((int*)_right));
	return (*((int*)_left) > *((int*)_right) ? TRUE: FALSE);
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
	Vector* myVec;
	Heap* myHeap;
	const void* topItem, *extrItem;
	
	int num;
	size_t numOfItems, i, heapSize;

	int arr[] = {7, 102, 3, 56, 20, 8, 45, 6, 3, 25};
	numOfItems = 10;
	
	myVec = Vector_Create(numOfItems, 0);
	
	for(i=0; i<numOfItems; i++) 
	{
		Vector_Append(myVec, &arr[i]); /*need to check result error*/
	}
	
	VectorPrint(myVec);
		printf("\nVecSize: %ld", Vector_Size(myVec));
	myHeap = Heap_Build(myVec, LessComparatorFunc);
	heapSize = Heap_Size(myHeap);
	printf("\nVecSize: %ld", Vector_Size(myVec));
		printf("\nHeapSize: %ld", heapSize);
	
	Heap_Destroy(&myHeap);
	
	myHeap = Heap_Build(myVec, LessComparatorFunc);
	
	topItem = Heap_Peek(myHeap);
		printf("\nTop: %d", *(int*)topItem);
	extrItem = Heap_Extract(myHeap);
		printf("\nExtract: %d", *(int*)extrItem);
	
	num = 21;
	Heap_Insert(myHeap, &num);

	Heap_Destroy(&myHeap);

return 0;
}
