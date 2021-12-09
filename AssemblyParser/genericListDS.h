#ifndef _genericListDS_h_
#define _genericListDS_h_

#include <stddef.h>  /* size_t */

typedef struct Node Node; /*node in the list*/
typedef struct list list;

/** 
 * @description: pointer to a destroy function to operate on element in the list.
 * @param _element element to operate on.
 */
typedef void (*elementDestroyFunc)(void* _element);

/** 
 * @description: pointer to action function to operate on an element in the list.
 * @param _element element to operate on.
 * @param _context other context to use in func.
 */
typedef void (*listActionFunc)(void* _context, void* _element);

/**
 * @description: Create a list.
 * @return a new pointer to the created list.
 */
list* createList(void);

/** 
 * @description: Destroy list.
 * Destroys the list completely. Optionally destroys elements using user provided function.
 * @param _pList A pointer to previously created List returned via createList on completion *_pList will be null
 * @param _listElementDestroy A function pointer to be used to destroy elements inserted into the list or a null if no such destroy is required
 */
void destroyList(list** _pList, void (*_elementDestroy)(void*));

/**
 * @description: Add element to the list's tail
 * @param _list 
 * @param _item item to push
 */
void pushListTail(list* _list, void* _item);

/**
 * @description: check if list is empty or contain nodes
 * @param _list the list to check
 * @return TRUE(1) when list is empty or NULL, otherwise return FALSE (0)
 */
int isEmptyList(list* _list);

/**
 * @Description: Get node itertator to the list's beginning.
 * @param _list - list to return begin iterator, pointing to first element of the list, or to the end of list(tail) if list is empty.
 * @return iterator pointing at the list's beginning. If list is NULL return NULL.
 */
Node* getListItrBegin(const list* _list);

/**
 * @Description: Get node itertator to the list end.
 * @param _list - list to return end iterator.
 * @return a node iterator pointing at the list's end (tail). If list is NULL return NULL.
 */
Node* getListItrEnd(const list* _list);

/**
 * @Description: Get node itertator to the next element from a given node iterator.
 * @param _b - pointer to iterator.
 * @return next node. If _itr is end iterator it will be returned. If _itr is NULL return NULL.
 */
Node* getListItrNext(Node* _itr);

/**
 * @Description: Perform the action function for all elements in the list.
 * @param _list list to go overpointer to iterator.
 * @param _action function that is provided for execution.
 * @param _context (optional) parameters for the function
 */
void doForEach(list* _list, listActionFunc _action, void* _context);

/**
 * @Description: Get data of the received list node.
 * @param _itr: A list node.
 * @return the data the iterator is pointing to (node's data). If iterator pointing to the end return NULL.
 */
Node* getListNodeData(void* _itr);

#endif /* genericListDS.h */

