/*
	genHeapFunc.c
	Functions for Generic Heap.
	Author: Inbar Barhum
	inbarba22@gmail.com
	last update: 16.12019
*/

#include <stdio.h>
#include <stdlib.h>

#include "genHeap.h"
#include "vectorGeneric.h"

#define PARENT(i) ((i)/2)
#define LEFT_SON(i) (2*(i))
#define RIGHT_SON(i) (2*(i)+1)


struct Heap
{
	Vector* m_vec;
	size_t m_heapSize;
	LessThanComparator m_heapLess;
};

static void RecShiftUp(Heap* _heap, size_t _i);
static int Swap(Vector* _vec, size_t _index, void* _father, void* _maxSon);
static int FindFatherAndSwap(Heap* _heap, size_t _i, void* _father);
static Heap_ResultCode Heapify(Heap* _heap, size_t _i);



/* @warning allocating and freeing the underlying vector is user responsibility. 
 * as long as vector is under the heap control, user should not access the vector directly*/
Heap* Heap_Build(Vector* _vector, LessThanComparator _pfLess)
{
	Heap* newHeap;
	int i;
	Heap_ResultCode stat;
	
	if(_vector == NULL)
	{
		return NULL;
	}
	
	newHeap = (Heap*) malloc(sizeof(Heap));
	if(newHeap == NULL)
	{
		return NULL;
	}
	
	newHeap->m_vec = _vector;
	newHeap->m_heapLess = _pfLess;
	newHeap->m_heapSize =  Vector_Size(_vector);

	for(i=(newHeap->m_heapSize)/2; i>0 ; i--)
	{
		if((stat=Heapify(newHeap, i)) != HEAP_HEAPIFY_SUCCESS)
		{
			printf("\nstat: %d", stat);
			return NULL;
		}
	}

	return newHeap;
}


/* Frees the heap but not the underlying vector */
/* There is no destroying the vactor */
Vector* Heap_Destroy(Heap** _heap)
{
	Vector* tempVector; 
	if(_heap == NULL || *_heap == NULL)
	{
		return NULL;
	}
	
	tempVector = (*_heap)->m_vec;
	free(*_heap);
	*_heap = NULL;
	return tempVector;
}


Heap_ResultCode Heap_Insert(Heap* _heap, void* _element)
{
	Vector_Result stat;
	if(_heap == NULL)
	{
		return HEAP_NOT_INITIALIZED;
	}
	
	if(_element == NULL)
	{
		return HEAP_OUTPUT_PARAMETER_NULL;
	}
	
	/* add _element to end of array */
	if((stat = Vector_Append(_heap->m_vec, _element)) == VECTOR_SUCCESS)
	{
		_heap->m_heapSize++;
		RecShiftUp(_heap, _heap->m_heapSize);
	}
	else
	{
		if(stat == VECTOR_ALLOCATION_ERROR)
		{
			return HEAP_REALLOCATION_FAILED;
		}
		if(stat == VECTOR_OVER_FLOW_ERROR)
		{
			return HEAP_OVER_FLOW;
		}
	}
	/*TODO REMOVE AFTER TESTS*/
	printf("\n*****vector after append:\n");
	VectorPrint(_heap->m_vec);
	
	return HEAP_SUCCESS;
}


/*foreach must not change key value*/
size_t Heap_ForEach(const Heap* _heap, ActionFunction _act, void* _context) 
{
	int i;
	void* item;
	
	if(NULL == _heap || NULL == _act)
	{
		return 0;
	}
	
	for(i = 1; i <= _heap->m_heapSize; i++)
	{
		Vector_Get(_heap->m_vec, i, &item);
		if(_act(item, _context) == 0)
		{
			break;
		}
	}
	return i;
}


/**  
 * @brief Sort a given vector of elments using a heap sort.
 * @param[in] _vector - vector to sort.
 * @param[in] _pfLess
 */
void Heap_Sort(Vector* _vec, LessThanComparator _pfLess) /*TODO*/
{
	Heap* newHeap;
	
	if(NULL == _vec || NULL == _pfLess)
	{
		return;
	}
	
	if((newHeap = Heap_Build(_vec, _pfLess)) == NULL)
	{
		return;
	}
/*finish function*/
}


const void* Heap_Peek(const Heap* _heap)
{
	void* tempData;
	if(_heap == NULL)
	{
		return NULL;
	}

	if(_heap->m_heapSize == 0)
	{
		return NULL;
	}
	
	if(Vector_Get(_heap->m_vec, 1, &tempData) == VECTOR_SUCCESS)
	{
		return tempData;
	}
	return NULL;
}


void* Heap_Extract(Heap* _heap)
{
	void* movedItem;
	void* extItem;

	if(_heap == NULL || _heap->m_heapSize == 0)
	{
		return NULL;
	}
	
	if (Heap_Peek(_heap) != NULL)
	{
		Vector_Get(_heap->m_vec, 1, &extItem);
		Vector_Remove(_heap->m_vec, &movedItem); /* moved last item in heap to top and heapify */
		Vector_Set(_heap->m_vec, 1, movedItem); /* set */
		_heap->m_heapSize--;
		Heapify(_heap, 1);
	}
	
	return extItem;
}


size_t Heap_Size(const Heap* _heap)
{
	if(_heap == NULL)
	{
		return 0;
	}
	
	return _heap->m_heapSize;	
}


/* ------------------------------- */
/* ******* Static Function ******* */
/* ------------------------------- */
static void RecShiftUp(Heap* _heap, size_t _i)
{
	/* the Heap starts from index = 1 */
	void* father, *son;
	
	if(_i == 1)
	{
		return;
	}

	Vector_Get(_heap->m_vec, PARENT(_i), &father);
	Vector_Get(_heap->m_vec, _i, &son);

	/*check if son should be the father*/
	if(_heap->m_heapLess(son, father))
	{
		Swap(_heap->m_vec, _i, father, son);
		RecShiftUp(_heap, PARENT(_i));
	}	
}


static int Swap(Vector* _vec, size_t _index, void* _father, void* _maxSon)
{
	Vector_Result resA, resB;
	resA = Vector_Set(_vec, PARENT(_index), _maxSon);
	resB = Vector_Set(_vec, _index, _father);
	if(resA != VECTOR_SUCCESS && resB != VECTOR_SUCCESS)
	{
		return -1;
	}
	return 1;	
}


/*swap 2 nodes data to get the tree heapify*/
static int FindFatherAndSwap(Heap* _heap, size_t _i, void* _father)
{
	void* right, *left;
	size_t index = 0;
	Vector_Result resA, resB;

	resA = Vector_Get(_heap->m_vec, LEFT_SON(_i), &left);
	resB = Vector_Get(_heap->m_vec, RIGHT_SON(_i), &right);
	if(resA != VECTOR_SUCCESS || resB != VECTOR_SUCCESS)
	{
		return -1;
	}
	 
	 /*if there is a swap index will get the index of the node that has been swapped with max, otherwise will remain the same */
	if(_heap->m_heapLess(_father, left) && _heap->m_heapLess(_father, right)) /*sub tree is heap sorted, father remain the same*/
	{
		index = _i;
		return index;
	}

	if(_heap->m_heapLess(left, right)) /*left node needs to be the father*/
	{
		if(Swap(_heap->m_vec, LEFT_SON(_i), _father, left))  /*swap data, left is the father*/
		{
			index = LEFT_SON(_i);
		}
		else
		{
			return -1;
		}
	}
	
	/*swap data, right needs to be the father*/
	else
	{
		if(Swap(_heap->m_vec, RIGHT_SON(_i), _father, right))
		{
			index = RIGHT_SON(_i);
		}
		else
		{
			return -1;
		}
	}
	
	return index;
}

static Heap_ResultCode Heapify(Heap* _heap, size_t _i)
{
	void* father, *left;
	Vector_Result res;
	size_t index = 0;
	father = NULL;
	left = NULL;
	
	/* if index is larger then heap size than exit function */
	if(_i > _heap->m_heapSize)
	{
		return HEAP_SUCCESS;
	}
	
	/*get the element in the index that has been given*/
	res = Vector_Get(_heap->m_vec, _i, &father);
	if(res != VECTOR_SUCCESS)
	{
		return HEAP_INSERT_ERROR;
	}
	
	/* father has 2 sons */
	if(RIGHT_SON(_i) <= _heap->m_heapSize)
	{
		index = FindFatherAndSwap(_heap, _i, father);
		if(index != _i && index != -1)
		{
			Heapify(_heap, index); /* there has been a swap, continue to heapify from the index */
		}
	}
	
	/* father has only one son - can only be the left */
	if(LEFT_SON(_i) == _heap->m_heapSize) 
	{		
		res = Vector_Get(_heap->m_vec, LEFT_SON(_i), &left);
		if(res != VECTOR_SUCCESS)
		{
			return HEAP_INSERT_ERROR;
		}

		if(_heap->m_heapLess(left, father))
		{
			Swap(_heap->m_vec, LEFT_SON(_i), father, left);
		}
	}
	return HEAP_HEAPIFY_SUCCESS;
}




