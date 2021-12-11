
#include <stdio.h>
#include <stdlib.h>

#include "genericListDS.h"
#define TRUE 1
#define FALSE 0

struct Node
{
	void* m_data;
	Node* m_next;
	Node* m_prev;
};

struct list
{
	Node m_head;
	Node m_tail; /*ability to go directly to end of the list*/
};

static void addNodeToList(Node* _nextNode, Node* _node);
static Node* createNode(void* _item);

list* createList(void)
{
 	list* newList;

	newList = (list*) malloc (sizeof (list));
	if(newList == NULL)
	{
		return NULL;
	}

	(newList->m_head).m_next = &(newList->m_tail);
	(newList->m_head).m_prev = NULL;
	(newList->m_tail).m_next = NULL;
	(newList->m_tail).m_prev = &(newList->m_head);		
	return newList;
}

void destroyList(list** _pList, void (*elementDestroyFunc)(void*))
{
	Node* tempNode;

	if(NULL == _pList || NULL == *_pList)
	{
		return;	
	}

	tempNode = ((*_pList)->m_head).m_next;
	while(tempNode != &((*_pList)->m_tail)) /* go over the list until the address of the tail! */
	{
		if(NULL != elementDestroyFunc) /*check pointer to function is not NULL*/
		{
			elementDestroyFunc(tempNode->m_data); /*destroy also what's inside the node*/
		}
		tempNode = tempNode->m_next;
		free(tempNode->m_prev); /* free also each node */
		tempNode->m_prev = NULL;
	}
	free(*_pList);
	*_pList = NULL;
}

void pushListTail(list* _list, void* _item)
{
	Node* newNode;

	if(_list == NULL)
	{
		printf("\nFatal Error: list or itmem to push are NULL. Program will be terminated.");
		exit(1);
	}	
	newNode = createNode(_item);	
	addNodeToList(&(_list->m_tail), newNode);
}


Node* getListItrBegin(const list* _list)
{
	if(NULL == _list)
	{
		return NULL;
	}
	return _list->m_head.m_next;
}


Node* getListItrEnd(const list* _list)
{
	if(NULL == _list)
	{
		return NULL;
	}
	return (Node*)&_list->m_tail;
}

Node* getListNodeData(void* _itr)
{
	if(NULL == _itr || ((Node*)_itr)->m_next == NULL) /* if node is tail return null - no data in tail */
	{
		return NULL;
	}
	return ((Node*)_itr)->m_data;
}

Node* getListItrNext(Node* _itr)
{
	if(NULL == _itr)
	{
		return NULL;
	}
	return _itr->m_next;
}

int isEmptyList(list* _list)
{
	if(_list == NULL)
	{
		return TRUE;
	}
	if((_list->m_head).m_next == &(_list->m_tail)) /* another check: if (ListCountItems == 0)*/
	{
		return TRUE;
	}
	return FALSE;
}

/*go over items in the list and perform the action on each element before printing it to file*/
void doForEach(list* _list, listActionFunc _action, void* _context)
{
	void* nData;
	Node* temp = getListItrBegin(_list); /*first node on list*/
	Node* endList = getListItrEnd(_list);

	while(temp != endList)
	{
		nData = getListNodeData(temp);
		_action(_context, nData); /*execute action function*/
		temp = getListItrNext(temp);
	}
}

static void addNodeToList(Node* _nextNode, Node* _node)
{
	_node->m_prev = _nextNode->m_prev;
	_node->m_next = _nextNode;
	_nextNode->m_prev->m_next = _node;
	_nextNode->m_prev = _node;
}
	

static Node* createNode(void* _item)
{
	Node* newNode;
	newNode = (Node*) malloc (sizeof(Node));
	if(NULL==newNode)
	{
		printf("\nFatal Error: Memory allocation failed for new list node. Program will be terminated.");
		exit(1);
	}
	newNode->m_data = _item;
	return newNode;
}

