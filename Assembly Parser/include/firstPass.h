#ifndef _firstPass_h_
#define _firstPass_h_

#include "constants.h"
#include "symbolTable.h"
#include "genericListDS.h"

/**
 * @description: Function performs a first passing over a single line from the source file.
 * @param _lineInfo A pointer to the source line details including file name, source line text and line number.
 * @param _table A pointer to the symbols table.
 * @param _IC A pointer to the instruction counter value.
 * @param _DC A pointer to the data counter value.
 * @param _codeImgArr A pointer to the code image array.
 * @param _dataImgArr A pointer to the data image array.
 * @return TRUE if the first passing ended successfully, otherwise return FALSE.
 */
bool performFirstPass(sourceLine_info* _lineInfo, symbolTable* _table, long* _IC, long* _DC, list* _codeImgList, list* dataImgList);

#endif /* firstPass.h */
