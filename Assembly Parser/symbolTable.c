/*symbolTable.c*/

#include "symbolTable.h"
/*Note: I don't make NULL validation check of the pointers in all functions, because any allocation that doesn't end successfuly the program is terminated.*/
/*at first i only planned to implement the symbol table as a list, but later i change the DS for the img code to list also and created the list class, but
because lack of time i didn't implement this table as well using the list class, but it is totally possible and better for encapsulation and unrepeating code reasons.*/

/*implementing symbol table via double linked list*/
typedef struct Node Node;

struct Node
{
	symbol* m_symbol;
	Node* m_next;
	Node* m_prev;
};

struct symbolTable /*symbol table structre*/
{
	Node m_head;
	Node m_tail;
};

/*the attributes information for each symbol is kept in a single linked list DS (linked att nodes to each other)*/
struct symbolAttNode
{
	attributes m_type;
	symbolAttNode* m_next;
};

static void destroyAttList(symbolAttNode** _attList);
static void addAttribute(symbolAttNode** _listHead, attributes _type);
static symbolAttNode* createSymbolAtt(attributes _type);
static Node* createSymbolNode(symbol* _symbol);
static void addNodeToTable(Node* _nextNode, Node* _node);

symbolTable* createSymbolTable(void)
{
 	symbolTable* newTable;
	newTable = (symbolTable*) malloc(sizeof (symbolTable));
	if(newTable == NULL)
	{
		printf("\nFatal Error: Memory allocation failed for symbol table. Program will be terminated.");
		exit(1);
	}

	(newTable->m_head).m_next = &(newTable->m_tail);
	(newTable->m_head).m_prev = NULL;
	(newTable->m_tail).m_next = NULL;
	(newTable->m_tail).m_prev = &(newTable->m_head);
	return newTable;
}

void destroySymbolTable(symbolTable** _table)
{
	Node* node;
	if(NULL == _table || NULL == *_table)
	{
		return;	
	}
	
	node = ((*_table)->m_head).m_next;
	while(node != &((*_table)->m_tail)) /* go over the list until the address of the tail */
	{
		destroySymbol(&(node->m_symbol)); /*free symbol*/
		node = node->m_next;
		free(node->m_prev); /*free also the node*/
		node->m_prev = NULL;
	}
	free(*_table);
	*_table = NULL;
}

bool isSymbolExist(symbolTable* _table, const char* _word) /*check if symbol name exist in table*/
{
	Node* node;

	if(NULL == _table)
	{
		printf("\nFatal Error: No Memory allocation for symbol table was done. Program will be terminated.");
		exit(1);
	}

	node = (_table->m_head).m_next; /*first node*/

	while(node != &(_table->m_tail))
	{
		if(!strcmp(node->m_symbol->m_name, _word))
		{
			return TRUE;
		}
		node = node->m_next;
	}
	return FALSE;
}

void addSymbolToTable(symbolTable* _table, symbol* _symbol)
{
	Node* newNode;

	if(NULL==_table)
	{
		printf("\nFatal Error: Memory allocation for symbol table hasn't been done. Program will be terminated.");
		exit(1);
	}
	
	newNode = createSymbolNode(_symbol); /*insert the symbol data to a node on the table list*/
	
	addNodeToTable(&(_table->m_tail), newNode);
}

symbol* createSymbol(char* _name, long _address, attributes _symbolType)
{

	symbol* newSymbol;
	newSymbol = (symbol*) malloc(sizeof(symbol));
	if(NULL==newSymbol)
	{
		/*failed to create new symbol*/
		printf("\nFatal Error: Memory allocation failed for new symbol. Program will be terminated.");
		exit(1);
	}

	newSymbol->m_name = (char*) malloc((strlen(_name)+1) * sizeof(char)); /*symbol name allocation*/	
	if(NULL==newSymbol->m_name)
	{
		free(newSymbol);
		printf("\nFatal Error: Memory allocation failed for new symbol name. Program will be terminated.");
		exit(1);
	}

	strcpy(newSymbol->m_name, _name);
	newSymbol->m_address = _address; /*symbol start position in code*/
	newSymbol->m_attTypeList = createSymbolAtt(_symbolType); /*the symbol type*/

	return newSymbol;
}

void destroySymbol(symbol** _symbol)
{
	if(NULL == _symbol || NULL == *_symbol)
	{
		return;	
	}
	
	free((*_symbol)->m_name); /*deallocate symbol name*/
	(*_symbol)->m_name = NULL;
	destroyAttList(&((*_symbol)->m_attTypeList)); /*deallocate attributes list*/
	free(*_symbol); /*deallocate symbol item*/
	*_symbol = NULL;
}

bool isSymbolTypeExist(symbol* _symbol, attributes _type)
{
	symbolAttNode* attTemp = _symbol->m_attTypeList;
	
	if(NULL == attTemp)
	{
		return FALSE;
	}
	
	while(NULL != attTemp) /*go over attributes list*/
	{
		if(attTemp->m_type == _type)
		{
			return TRUE;
		}
		attTemp = attTemp->m_next;
	}
	return FALSE;
}

symbol* getSymbolByName(symbolTable* _table, const char* _name)
{
	Node* node = (_table->m_head).m_next; /*first node*/
	while(node != &(_table->m_tail))
	{
		if(!strcmp(node->m_symbol->m_name, _name))
		{
			return node->m_symbol;
		}
		node = node->m_next;
	}
	return NULL; /*Symbol doesn't exist*/
}

/*check if it is ok to add attribute to symbol, or that adding the type to symbol is illegal*/
bool isValidAttToAdd(symbolTable* _table, const char* _name, attributes _type)
{
	Node* node = (_table->m_head).m_next; /*first node (symbol on table)*/
	
	if(NULL==_table)
	{
		return FALSE;
	}
	
	while(node != &(_table->m_tail)) /*go over symbol table items*/
	{
		if(!strcmp(node->m_symbol->m_name, _name))
		{
			switch(_type)
			{ /*combination to verify*/
				case ENTRY: {if(isSymbolTypeExist(node->m_symbol, EXTERNAL)) {return FALSE;} break;};
				case EXTERNAL: {if(isSymbolTypeExist(node->m_symbol, ENTRY) 
				|| isSymbolTypeExist(node->m_symbol, DATA) 
				|| isSymbolTypeExist(node->m_symbol, CODE)) {return FALSE;} break;};
				case CODE: {if(isSymbolTypeExist(node->m_symbol, DATA)) {return FALSE;} break;};
				case DATA: {if(isSymbolTypeExist(node->m_symbol, CODE)) {return FALSE;} break;};
			}
			return TRUE; /*attribute symbol type is valid*/
		}
		node = node->m_next; /*next symbol*/
	}
	return TRUE; /*Symbol doesn't exist*/
}

void updateSymbolAttributes(symbolTable* _table, const char* _name, attributes _type)
{
	Node* node = (_table->m_head).m_next; /*first node*/
	while(node != &(_table->m_tail))
	{
		if(!strcmp(node->m_symbol->m_name, _name))
		{
			/*add requested attribute to symbol if not already exist*/
			if(!isSymbolTypeExist(node->m_symbol, _type))
			{
				addAttribute(&(node->m_symbol->m_attTypeList), _type);
			}
			return;			
		}
		node = node->m_next;
	}
}

void updateSymbolsAdresses(symbolTable* _table, long _icf)
{
	Node* node = (_table->m_head).m_next; /*first node (symbol on table)*/
	symbolAttNode* att;
	/*go over symbols list and add icf value to the address value of each symbol that has a DATA attribute*/
	while(node != &(_table->m_tail))
	{
		att = node->m_symbol->m_attTypeList;
		while(NULL != att)
		{
			if(att->m_type == DATA)
			{
				node->m_symbol->m_address += _icf;
				break; /*no need to check other attributes*/
			}
			att = att->m_next;
		}
		node = node->m_next;
	}
}

/*destroy symbol's attributes list*/
static void destroyAttList(symbolAttNode** _attList)
{
	symbolAttNode* next;
	symbolAttNode* curr = *_attList; /*current node to free*/
	
	if(NULL == _attList || NULL == *_attList)
	{
		return;	
	}
	while(NULL != curr) /* go over the list until null*/
	{
		next = curr->m_next;
		free(curr); /*deallocate list's node. list's items are of 'int' type (Primitive Data Types) so no need to free them*/
		curr = next;
	}
}

/*add attribute to symbol attributes list*/
static void addAttribute(symbolAttNode** _listHead, attributes _type)
{
	symbolAttNode* newAtt;

	if(NULL==*_listHead)
	{
		printf("\nFatal Error: Memory allocation failed for symbol attribute. Program will be terminated.");
		exit(1);
	}
	
	newAtt = createSymbolAtt(_type); /*creating attribute node*/
	
	newAtt->m_next = *_listHead; /*make the linkage between nodes, the new node is added to the start of the list*/
	*_listHead = newAtt; /*new node is the head of the list*/
}

/*creating attribute node*/
static symbolAttNode* createSymbolAtt(attributes _type)
{
 	symbolAttNode* newAtt;
	
	newAtt = (symbolAttNode*) malloc(sizeof (symbolAttNode));
	if(NULL == newAtt)
	{
		printf("\nFatal Error: Memory allocation failed for symbol attribute. Program will be terminated.");
		exit(1);
	}

	newAtt->m_type = _type;
	newAtt->m_next = NULL;
	return newAtt;
}

/*creating symbol node*/
static Node* createSymbolNode(symbol* _symbol)
{
	Node* newNode;
	newNode = (Node*) malloc(sizeof(Node));
	if(NULL == newNode)
	{
		printf("\nFatal Error: Memory allocation failed for new symbol node in table. Program will be terminated.");
		exit(1);
	}
	newNode->m_symbol = _symbol;
	
	return newNode;
}

/*make the linkage between nodes*/
static void addNodeToTable(Node* _nextNode, Node* _node)
{
	_node->m_prev = _nextNode->m_prev;
	_node->m_next = _nextNode;
	_nextNode->m_prev->m_next = _node;
	_nextNode->m_prev= _node;
}
