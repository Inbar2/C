/**
 * @brief Create a Generic Vector data type.
 * that stores pointer to user provided elements of generic type
 * The Vector is heap allocated and can grow and shrink on demand.
 * 
 * @author: Inbar Barhum
 * @lastUpdate: 10.1.2019
 */ 

#ifndef __VECTORGENERICHEAD_H__
#define __VECTORGENERICHEAD_H__

#include <stddef.h>  /* size_t */

typedef struct Vector Vector;

typedef int	(*VectorElementAction)(void* _element, size_t _index, void* _context);

typedef enum Vector_Result {
	VECTOR_SUCCESS,
	VECTOR_UNITIALIZED_ERROR,				/**< Uninitialized vector error 	*/
	VECTOR_ALLOCATION_ERROR,				/**< realloc error on grow/shrink   */
	VECTOR_INDEX_OUT_OF_BOUNDS_ERROR,
	VECTOR_OUTPUT_PARAMETER_ERROR,
	VECTOR_OVER_FLOW_ERROR,
	VECTOR_UNDER_FLOW_ERROR
} Vector_Result;

typedef void (*_elementDestroy)(void* _item);
/*
 * @description: Dynamically create a new vector object of given capacity.
 * @param[in]: _initialCapacity - initial capacity, number of elements that can be stored initially.
 * @param[in]: _blockSize - the vector will grow or shrink on demand by this size. 
 * @return: Vector* - on success, otherwise return NULL.
 *
 * @warning: if _blockSize is 0 the vector will be of fixed size. 
 * @warning: if both _initialCapacity and _blockSize are zero function will return NULL.
 */
Vector* Vector_Create(size_t _initialCapacity, size_t _blockSize);

/*
 * @description: Dynamically deallocate a previously allocated vector. 
 * @param[in]: _vector - Vector to be deallocated, created from Vector_Create.
 * @param[in]: _elementDestroy - A pointer to function to be used to destroy all elements in the vector, or a null if no such destroy is required.
 * @return void.
 */
void Vector_Destroy(Vector** _vector,  _elementDestroy _ptrFuncDes);

/* 
 * @description: Add an Item to the back of the Vector.  
 * @param[in]: _vector - Vector to append integer to, created from Vector_Create.
 * @param[in]: _item - Item to add.
 * @return: success or error code.
 * @retval: VECTOR_SUCCESS on success 
 * @retval: VECTOR_UNITIALIZED_ERROR
 *	VECTOR_OUTPUT_PARAMETER_ERROR
 *	VECTOR_OVER_FLOW_ERROR
 *	VECTOR_ALLOCATION_ERROR
 */
Vector_Result Vector_Append(Vector* _vector, void* _item);

/*
 * @description: Delete an Element from the back of the Vector.  
 * @param[in] _vector - Vector to delete integer from, created from Vector_Create.
 * @param[out] _pValue - pointer to variable that will receive deleted item value
 * @return success or error code 
 * @retval VECTOR_SUCCESS on success 
 * @retval: VECTOR_UNITIALIZED_ERROR
 *	VECTOR_ALLOCATION_ERROR
 *	VECTOR_UNDER_FLOW_ERROR
 * @warning _item can't be null. this will be assertion violation
 */
Vector_Result Vector_Remove(Vector* _vector, void** _pValue);

/*
 * @description: Get value of item at specific index from the the Vector 
 * @param[in] _vector - Vector to use, created from Vector_Create.
 * @param[in] _index - index of item to get value from. the index of first elemnt is 1
 * @param[out] _pValue - pointer to variable that will recieve the item's value.
 * @return success or error code 
 * @retval VECTOR_SUCCESS on success 
 * @retval: VECTOR_UNITIALIZED_ERROR
 *	VECTOR_OUTPUT_PARAMETER_ERROR
 *	VECTOR_INDEX_OUT_OF_BOUNDS_ERROR
 * @warning Index starts from 1.
 */
Vector_Result Vector_Get(const Vector* _vector, size_t _index, void** _pValue);

/*
 * @description: Set an item at specific index to a new value.
 * @param[in] _vector - Vector to use, created from Vector_Create.
 * @param[in] _index - index of an existing item.
 * @param[in] _value - new value to set.
 * @return success or error code 
 * @retval VECTOR_SUCCESS on success
 * @retval: VECTOR_UNITIALIZED_ERROR
 *	VECTOR_OUTPUT_PARAMETER_ERROR 
 *	VECTOR_INDEX_OUT_OF_BOUNDS_ERROR
 * @warning Index starts from 1.
 */
Vector_Result Vector_Set(Vector* _vector, size_t _index, void*  _value);

/*
 * @description: Get the number of actual items currently in the vector.
 * @param[in] _vector - Vector to use, created from Vector_Create.
 * @return: number of items on success, if vector is empty or invalid returns 0.
 */
size_t Vector_Size(const Vector* _vector);

/*
 * @description: Get the current capacity of the vector.
 * @param[in]: _vector - Vector to use, created from Vector_Create.
 * @return: capacity of vector.	
 */
size_t Vector_Capacity(const Vector* _vector);


/* 
 * @description: Iterate over all elements in the vector.
 * @details: The user provided _action function will be called for each element.
 *          if _action return a zero for an element the iteration will stop.  
 * @param[in]: _vector - vector to iterate over, created from Vector_Create.
 * @param[in]: _action - User provided function pointer to be invoked for each element.
 * @param[in]: _context - User provided context, will be sent to _action. user sends a parameter to action.
 * @return: number of times the user functions was invoked.
 			if vector is NULL or _action is NULL return 0.
 * equevallent to:
 *      for(i = 1; i < Vector_Size(v); ++i){
 *             Vector_Get(v, i, &elem);
 *             if(_action(elem, i, _context) == 0)
 *					break;	
 *      }
 *		return i;
 */
size_t Vector_ForEach(const Vector* _vector, VectorElementAction _action, void* _context);

void VectorPrint(Vector const* _vector);

#endif /* __vectorGenericHead_H__ */

