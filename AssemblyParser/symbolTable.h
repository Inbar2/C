#ifndef _symbolTable_h_
#define _symbolTable_h_

#include "constants.h"

typedef struct symbolTable symbolTable;

typedef enum attributes {CODE=1, DATA, ENTRY, EXTERNAL} attributes;

typedef struct symbolAttNode symbolAttNode; /*attributes list's node*/

/*symbol details*/
typedef struct symbol {
						char* m_name;
						long m_address;
						symbolAttNode* m_attTypeList;
					} symbol;

/**
 * @description: Create a new symbol table.
 * @return a new pointer to the created symbol table.
 */
symbolTable* createSymbolTable(void);

/** 
 * @description: free the symbol table and its elements. Destroys the list completely.
 * @param _table A pointer to previously created List returned via createSymbolTable on completion *_pList will be NULL.
 */
void destroySymbolTable(symbolTable** _table);

/**
 * @description: Function checks if symbol is exist in table.
 * @param _table The symbol table to look in.
 * @param _word The symbol name to search in table. 
 * @return TRUE if name exist, otherwise FALSE.
 */
bool isSymbolExist(symbolTable* _table, const char* _word);/*check if symbol name exist in table*/

/**
 * @description: Add symbol to the table's tail
 * @param _table The symbol table to look in.
 * @param _symbol the symbol to add to the table.
 */
void addSymbolToTable(symbolTable* _table, symbol* _symbol);

/**
 * @description: Create a new symbol element.
 * @param _name The symbol name to search in table.
 * @param _address The symbol address value
 * @return a new symbol element
 */
symbol* createSymbol(char* _name, long _address, attributes _symbolType);

/** 
 * @description: destroy the received symbol.
 * @param _symbol A pointer to the symbol item. On completion _symbol will be NULL.
 */
void destroySymbol(symbol** _symbol);

/**
 * @description: Function checks if symbol has received attribute value type.
 * @param _symbol The symbol.
 * @param _type The attribute type to check if exist in symbols attributes. 
 * @return TRUE if symbol defined with the received type, otherwise FALSE.
 */
bool isSymbolTypeExist(symbol* _symbol, attributes _type);

/**
 * @description: Function get by name the requested symbol from table if it exist.
 * @param _table The symbol table to look in.
 * @param _name The symbol name to search in table.
 * @return the the symbol element, if not found return NULL.
 */
symbol* getSymbolByName(symbolTable* _table, const char* _name);

/**
 * @description: Function check if it's legal to add attribute type to symbol on table.
  * compare received attribute type to the alreay defined attribute to symbol.
 * @param _table The symbol table to look in.
 * @param _type The attribute type to compare to symbols attributes exist. 
 * @return TRUE if it's valid to add the type, otherwise return FALSE. If symbol doesn't exist return TRUE.
 */
bool isValidAttToAdd(symbolTable* _table, const char* _name, attributes _type);

/**
 * @description: Function updates the symbol attributes - if valid add the attribute type.
 * @param _table The symbol table.
 * @param _name The symbol name to search in table.
 * @param _type The attribute type to add to symbols attributes list. 
 */
void updateSymbolAttributes(symbolTable* _table, const char* _name, attributes _type);

/**
 * @description: Function updates the address of all DATA type symbols on list by adding ICF value to each.
 * @param _table The symbol table.
 * @param _icf The value to add to symbol address.
 */
void updateSymbolsAdresses(symbolTable* _table, long _icf);


#endif /* symbolTable.h */


