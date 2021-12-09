#ifndef _fileProcess_h_
#define _fileProcess_h_

#include "constants.h"

#define FILE_SUFFIX ".as" /*assembler input file type*/

/**
 * @description: Function recieves a file name, check if there are no mistakes in the file type and name, opens it and processes it.
 * @param _fileStr The file name.
 * @param _suffixSize The size of the file suffix.
 */
void processFile(char* _fileStr, int _suffixSize);

#endif /* fileProcess.h> */
