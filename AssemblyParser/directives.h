#ifndef _directives_h_
#define _directives_h_

#include "constants.h"
#include "symbolTable.h"
#include "genericListDS.h"


/*pointer to function to destroy each node data of the data image*/
void elementDataDestroy(void* _item);

/**
 * @description: Function checks if received string word is a valid directive name.
 * @param _word The string to verify.
 * @return TRUE if name is valid, otherwise FALSE.
 */
bool isDirectiveName(const char* _word);

/**
 * @description: Function process an asciz directive line.
 * @param _lineInfo A pointer to the source line details.
 * @param _name The name of the directive.
 * @param _pos The position on line to start the processing from.
 * @param _dc A return param a pointer to the directive address counter to update.
 * @param _dataImgList The data image list.
 * @return TRUE if successful line synthax processing, otherwise return FALSE.
 */
bool processAscizDir(sourceLine_info* _lineInfo, const char* _name, int _pos,  long* _dc, list* _dataImgList);

/**
 * @description: Function process an data directive line.
 * @param _lineInfo A pointer to the source line details.
 * @param _name The name of the directive.
 * @param _pos The position on line to start the processing from.
 * @param _dc A return param a pointer to the directive address counter to update.
 * @param _dataImgList The data image list.
 * @return TRUE if successful line synthax processing, otherwise return FALSE.
 */
bool processDataDir(sourceLine_info* _lineInfo, const char* _name, int _pos, long* _dc, list* _dataImgList);

/**
 * @description: Function process an external directive line.
 * @param _lineInfo A pointer to the source line details.
 * @param _name The name of the directive.
 * @param _table the symbol table.
 * @param _pos The position on line to start the processing from.
 * @return TRUE if successful line synthax processing, otherwise return FALSE.
 */
bool processExternalDir(sourceLine_info* _lineInfo, symbolTable* _table, const char* _name, int _pos);

/**
 * @description: Function that updates in the 2nd pass the address of each data according to relativly to the end of instructions address.
 * @param _lineInfo A pointer to the line details.
 * @param _codeImgList the code image list.
 * @param _icf The instruction last address.
 */
void updateDataImgAddresses(const list* _dataImgList, long _icf);

#endif /* directives.h */
