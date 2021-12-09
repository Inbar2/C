/*errors.c*/

#include <stdio.h>
#include <stdarg.h> /*for argument list*/

#include "errors.h"

/*an error format for printing errors message*/
void printLineError(int _messageType, sourceLine_info* _lineInfo, char* _text, ...)
{
	va_list ap; /*argument pointer*/
	if(_messageType)
	{
		printf("\nError: ");
	}
	else
	{
		printf("\nWarning: ");
	}
	printf("In file \"%s\", line: %ld, ", _lineInfo->m_fileName, _lineInfo->m_lineNum);
	va_start(ap, _text);
	vprintf(_text, ap);
	va_end(ap);
}
