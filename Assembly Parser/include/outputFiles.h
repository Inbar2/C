/*outputFiles.h */

#ifndef _outputFiles_h_
#define _outputFiles_h_

#include "constants.h"
#include "symbolTable.h"
#include "genericListDS.h"

/**
 * @description: Function creates the output files.
 * OB file is created always, Entry file and External file are only created if there are data to display.
 * @param _table the symbol table that holds the entries.
 * @param _extList the external list with the externals.
 * @param _codeImgList the code image list.
 * @param _dataImgList the data image list.
 * @param _fileName the file name to create.
 * @param _icf the icf value for the OB file.
 * @param _dcf the dcf value for the OB file.
 * @param _isEntry Boolean value that indicates if there are entries to create the file.
 */
void createOutputFiles(symbolTable* _table, list* _extList, list* _codeImgList, list* _dataImgList, const char* _fileName, long _icf, long _dcf, bool _isEntry);

#endif /* outputFiles.h */
