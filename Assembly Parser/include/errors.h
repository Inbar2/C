#ifndef _errors_h_
#define _errors_h_

#include "constants.h"

#define WARNING 0
#define ERROR 1

/**
 * Prints an informative error message to the stdout.
 * The message format includs the file name and the line number in which the error occurred.
 * @param _lineInfo A pointer to the source line details including file name, source line text and line number.
 * @param _text The text to print in the error message.
 * @param ... Extra variables values to include in the error message.
 */
void printLineError(int _messageType, sourceLine_info* _lineInfo, char* text, ...);

#endif /* errors.h */
