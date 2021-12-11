/* 
 * File: heapGenTestHead.h
 * Signatures for Automation Tests for Periodic Task Executor project.
 */ 

#ifndef __HEAPGENTESTHEAD_H__
#define __HEAPGENTESTHEAD_H__

#include <stdio.h>
#include <stdlib.h>

#include "AutoTestsEnum.h"
#include "genHeap.h"

#define TRUE 1
#define FALSE 0


int	LessComparatorFunc(const void* _left, const void* _right);

int	ActionFunc(const void*_elem, void* _context);

void PrintTestsResult(char _name[], AutoTestResults _testResult, Heap_ResultCode* _status);


/*---------------------------------------------------------------------------------------------------------------*/
AutoTestResults Heap_BuildTest1(Heap_ResultCode* _status); /* build heap */
AutoTestResults Heap_BuildTest2(Heap_ResultCode* _status); /* build heap with NULL vector */

/*---------------------------------------------------------------------------------------------------------------*/
AutoTestResults Heap_DestroyTest1(Heap_ResultCode* _status); /* heap destroy */
AutoTestResults Heap_DestroyTest2(Heap_ResultCode* _status); /* heap destroy twice*/

/*---------------------------------------------------------------------------------------------------------------*/
AutoTestResults Heap_InsertTest1(Heap_ResultCode* _status); /* insert node to heap */
AutoTestResults Heap_InsertTest2(Heap_ResultCode* _status); /* insert severl nodes to heap */
AutoTestResults Heap_ForEachTest1(Heap_ResultCode* _status); /* print heap after build with foreach */

#endif /* "heapGenTestHead.h" include */
