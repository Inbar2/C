/*
	vectorGenericFunc.c
	Functions for Generic vector.
	Author: Inbar Barhum
	inbarba22@gmail.com
	last update: 16.12.2019
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "vectorGeneric.h"

#define INCREASE_SIZE 2

struct Vector
{
	void** m_items;
	size_t m_originalSize;
	size_t m_size;
	size_t m_nItems;
	size_t m_blockSize;
};

Vector* Vector_Create(size_t _initialCapacity, size_t _blockSize)
{
	Vector* vectorPtr;
	
	
	if(_initialCapacity == 0 && _blockSize == 0)
	{
		return NULL; 
	}

	vectorPtr = (Vector*) malloc(sizeof(Vector));
	if(vectorPtr == NULL)
	{
		return NULL;
	}

	vectorPtr->m_items = (void**) malloc((_initialCapacity+1)*sizeof(void*)); /* m_items pointer to void* array start from 1 index */
	
	if((vectorPtr->m_items) == NULL) /* allocation failed */
	{	
		free(vectorPtr);
		return NULL;
	}

	vectorPtr->m_originalSize = _initialCapacity;
	vectorPtr->m_size = _initialCapacity;
	vectorPtr->m_nItems = 0;
	vectorPtr->m_blockSize = _blockSize;

	return vectorPtr;
}


void Vector_Destroy(Vector** _vector,  _elementDestroy _ptrFuncDes) /* pointer to pointer in order to free also the vector pointer */
{
	int i;
	if (NULL == _vector || NULL == *_vector)
	{
		return;
	}
	if(_ptrFuncDes != NULL) /* check ptr is not null */
	{
		for(i=1; i<=(*_vector)->m_nItems; i++)
		{
			printf("num = %lu, i = %d", (*_vector)->m_nItems, i);
			_ptrFuncDes((*_vector)->m_items[i]);
		}
	}

	free((*_vector)->m_items);
	free(*_vector);
	*_vector = NULL;
}


static Vector_Result VectorResiz(Vector* _vector)
{
	if(_vector->m_blockSize>0)
	{
		_vector->m_items = (void**) realloc(_vector->m_items, (_vector->m_size + _vector->m_blockSize)*sizeof(void*)); /* m_items pointer to int array */
		
		if (_vector->m_items == NULL)
		{
			return VECTOR_ALLOCATION_ERROR;
		}
		_vector->m_size = _vector->m_size + _vector->m_blockSize;
		return VECTOR_SUCCESS;
	}
	return VECTOR_OVER_FLOW_ERROR;
}


Vector_Result Vector_Append(Vector* _vector, void* _item)
{
	Vector_Result resizeResult = VECTOR_SUCCESS;
			
	if (_vector == NULL)
	{
		return VECTOR_UNITIALIZED_ERROR;
	}

	if (NULL == _item)
	{
		return VECTOR_OUTPUT_PARAMETER_ERROR;
	}

	if(_vector->m_size == _vector->m_nItems)
	{
		resizeResult = VectorResiz(_vector);
		if(resizeResult != VECTOR_SUCCESS)
		{
			return resizeResult;
		}	
	}

	_vector->m_nItems++;
	_vector->m_items[_vector->m_nItems] = _item;
	
	return VECTOR_SUCCESS;
}


Vector_Result Vector_Remove(Vector* _vector, void** _pValue)
{
	if (_vector == NULL)
	{
		return VECTOR_UNITIALIZED_ERROR;
	}

	if (NULL == _pValue)
	{
		return VECTOR_OUTPUT_PARAMETER_ERROR;
	}

	if (_vector->m_nItems == 0)
	{
		return VECTOR_UNDER_FLOW_ERROR;
	}
	
	*_pValue = _vector->m_items[_vector->m_nItems];
	_vector->m_nItems--;
	
	if(_vector->m_size - _vector->m_nItems >= (_vector->m_blockSize * INCREASE_SIZE) && _vector->m_originalSize < (_vector->m_size - _vector->m_blockSize))
	{
		_vector->m_items = (void**) realloc(_vector->m_items, _vector->m_nItems * sizeof(void*));
		if(_vector->m_items == NULL)
		{
			return VECTOR_ALLOCATION_ERROR;
		}
		_vector->m_size = _vector->m_size - _vector->m_blockSize;
		
	}
	return VECTOR_SUCCESS;
}


Vector_Result Vector_Get(const Vector* _vector, size_t _index, void** _pValue)
{
	if (_vector == NULL)
	{
		return VECTOR_UNITIALIZED_ERROR;
	}
	
	if (NULL == _pValue)
	{
		return VECTOR_OUTPUT_PARAMETER_ERROR;
	}
	
	
	if(_index > _vector->m_nItems || _index < 1)
	{
		return VECTOR_INDEX_OUT_OF_BOUNDS_ERROR;
	}

	*_pValue = _vector->m_items[_index];
	
	return VECTOR_SUCCESS;
}


Vector_Result Vector_Set(Vector* _vector, size_t _index, void*  _value)
{
	if (_vector == NULL)
	{
		return VECTOR_UNITIALIZED_ERROR;
	}

	if(_index > _vector->m_nItems || _index < 1)
	{
		return VECTOR_UNDER_FLOW_ERROR;
	}

	if(_index > _vector->m_nItems || _index < 1)
	{
		return VECTOR_INDEX_OUT_OF_BOUNDS_ERROR;
	}
	
	_vector->m_items[_index] = _value;
	
	return VECTOR_SUCCESS;
}


size_t Vector_Size(const Vector* _vector)
{
	if (_vector == NULL)
	{
		return 0;
	}

	return _vector->m_nItems;
}


size_t Vector_Capacity(const Vector* _vector)
{
	if (_vector == NULL)
	{
		return 0;
	}

	return _vector->m_size;
}


size_t Vector_ForEach(const Vector* _vector, VectorElementAction _action, void* _context)
{
	int i;
	
	if(NULL == _vector || NULL == _action)
	{
		return 0;
	}
	
	for(i = 1; i < _vector->m_nItems; i++)
	{
		if(_action(_vector->m_items[i], i, _context) == 0)
		{
			break;
		}

	}
	return i;
}


void VectorPrint(Vector const* _vector)
{
	int i;
	if (_vector == NULL)
	{
 		printf("\nVector is uninitialized!\n");
 		return;
	}
	
	if (_vector->m_nItems == 0)
	{
		printf("\nVector is empty!\n");
	}	
	else if (_vector->m_nItems > 0)
	{
		printf("Items: ");
		for(i = 1; i < _vector->m_nItems; i++)
		{
			printf("%p, ", _vector->m_items[i]);
		}

		printf("%p\n", _vector->m_items[i]);		
		/*
		printf("Vector number of items is: %d\n", (int) _vector->m_nItems);
		printf("Vector original size is: %d\n", (int) _vector->m_originalSize);		
		printf("Vector size is: %d\n", (int) _vector->m_size);
		printf("Vector block size is: %d\n", (int) _vector->m_blockSize);
		*/
	}
}

void PrintInt(void* _element, void* _context)
{
	if(_element == NULL)
	{
		return;
	}

	printf("%d, ", *(int*) _context);
}



