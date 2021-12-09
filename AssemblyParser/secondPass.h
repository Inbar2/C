#ifndef _secondPass_h_
#define _secondPass_h_

#include "constants.h"
#include "genericListDS.h"
#include "symbolTable.h"

/**
 * @description: function that executes the second pass on the file source.
 * @param _lineInfo A pointer to the line details.
 * @param _table The symbol table.
 * @param _ic A pointer to the instruction address counter.
 * @param _codeImgList The code image list.
 * @param _externalsList The externals list.
 * @param _isEntry Boolean param indicates if there is an entry on the file.
 * @return TRUE if the second passing ended successfully, otherwise return FALSE
 */
bool performSecondPass(sourceLine_info* _lineInfo, symbolTable* _table, long* _ic, list* _codeImgList, list* _externalsList, bool* _isEntry);

#endif /* secondPass.h */
