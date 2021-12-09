/*generalParsingFuncs.c*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h> /*for using errno*/
#include <math.h> /*for using pow*/

#include "generalFuncs.h"
#include "errors.h"

#define DECIMAL_BASE 10
#define MIN_REG_NUM 0
#define MAX_REG_NUM 31

static bool isValidRegisterFormat(const char* _operand);

void extractWord(const char* _line, int* _pos, char* _word)
{
	int i=0;
	/*if there are spaces in the beginning of the line ignore them*/
	*_pos = skipSpaces(_line, *_pos);

	while(*_pos <= MAX_LINE_LENGTH && _line[*_pos] != '\0' && !isspace(_line[*_pos]))
	{
		_word[i] = _line[*_pos];
		(*_pos)++;
		i++;
	}
	_word[i] = '\0'; /*end of string*/
}

bool extractOperandString(const char* _line, int _pos, char* _word)
{
	int i=0;
	/*if there are spaces in the beginning of the line ignore them*/
	_pos = skipSpaces(_line, _pos);
	
	while(_line[_pos] != '\0' && _line[_pos] != '\n')
	{
		_word[i] = _line[_pos];
		_pos++;
		i++;
	}
	_word[i] = '\0'; /*end of string*/

	return strlen(_word)!=0; /*if no operand will return FLASE*/
}

/*check if the string from recieved position consists only spaces*/
bool isStrOnlySpaces(const char* _str, int _pos)
{
	while(_str[_pos] != '\0')
	{
		if(!isspace(_str[_pos]))
		{
			return FALSE;
		}
		_pos++;
	}
	return TRUE;
}

int skipSpaces(const char* _str, int _pos)
{
	while(_str[_pos] != '\0' && _str[_pos] != '\n' && isspace(_str[_pos]))
	{
		_pos++;
	}
	return _pos;
}

bool isOperandTextExist(const char* _line, int* _pos)
{
	*_pos = skipSpaces(_line, *_pos);
	if(_line[*_pos]=='\n' || _line[*_pos]=='\0')
	{
		return FALSE;
	}
	return TRUE;
}

bool isNumInRange(long _num, num_byte_size _size)
{
	long maxVal;
	int totalBits = _size*CHAR_BIT;
	
	maxVal = pow(2, totalBits)/2;
	return((_num <= maxVal-1) && (_num >= -maxVal));
}

bool isValidIntNum(const char* _operand)
{
	if(*_operand=='-' || *_operand=='+') /*check if operand has a +/- mark*/
	{
		_operand++;
	}
	if(strlen(_operand)==0)
	{
		return FALSE; /*no number in opernad*/
	}

	while(*_operand != '\n' && *_operand != '\0')
	{
		if(!isdigit(*_operand))
		{
			return FALSE;
		}
		_operand++;
	}
	return TRUE;
}

/*converting string to long integer with result check*/
bool convertStringToLong(sourceLine_info* _lineInfo, char* _operand, long* _num)
{
	char *endptr = NULL; /*for strtol result validation*/	
    errno = 0; /*reset errno to 0 before calling strtol*/
	*_num = strtol(_operand, &endptr, DECIMAL_BASE); /*convert the string num to decimal long type number*/	

	if(errno != 0 && *_num == 0L)
	{
		/*printLineError(ERROR, _lineInfo, "No valid conversion for operand: %s could be performed.", _operand);*/
		return FALSE;
	}
	if(!(errno == 0 && _operand && !*endptr)) /*strtol function result validation*/
	{
		/*printLineError(ERROR, _lineInfo, "Operand: %s is out of the range of representable values by a long int.", _operand);*/
		return FALSE;
	}
	return TRUE;
}

bool extractOperandNum(sourceLine_info* _lineInfo, int* _pos, char* _operand)
{
	int i;
	char* str = _lineInfo->m_sourceLine;

	for(i=0; str[*_pos] != ',' && !isspace(str[*_pos]) && str[*_pos] != '\0'; i++, (*_pos)++)
	{
		_operand[i] = str[*_pos]; /*copy operand's chars*/
	}	
	_operand[i] = '\0'; /*end of operand*/

	/*check if operand is a legal integer number*/
	if(!isValidIntNum(_operand))
	{
		printLineError(ERROR, _lineInfo, "Invalid integer operand: \'%s\'.", _operand);
		return FALSE;
	}

	return TRUE;
}

bool extractRegisterNum(sourceLine_info* _lineInfo, int* _pos, char* _operand, long* _reg)
{
	int i;
	char* str = _lineInfo->m_sourceLine;
	
	for(i=0; str[*_pos] != ',' && !isspace(str[*_pos]) && str[*_pos] != '\0'; i++, (*_pos)++)
	{
		_operand[i] = str[*_pos]; /*copy register's chars*/
	}
	
	_operand[i] = '\0'; /*end of operand*/
	/*check if operand is a legal register*/
	if (!isValidRegisterFormat(_operand))
	{
		printLineError(ERROR, _lineInfo, "Invalid register operand format: \'%s\'.", _operand);
		return FALSE;
	}

	if(!convertStringToLong(_lineInfo, ++_operand, _reg) || *_reg < MIN_REG_NUM || *_reg > MAX_REG_NUM)
	{
		printLineError(ERROR, _lineInfo, "Invalid register number: \'%s\'. Must be between %d-%d.", _operand, MIN_REG_NUM, MAX_REG_NUM);
		return FALSE;
	}
	return TRUE;
}

bool preOperandSyntaxCheck(sourceLine_info* _lineInfo, const char* _name, int* _pos, char* _lineType)
{
	char* str = _lineInfo->m_sourceLine; /*pointer to location on line*/
	if(isStrOnlySpaces(str, *_pos)) /*skip spaces and check if there is any text after directive name*/
	{
		printLineError(ERROR, _lineInfo, "The %s \'%s\' doesn't have operand text after it on line.", _lineType, _name);
		return FALSE;
	}
	/*skip spaces between directive and operand*/
	*_pos = skipSpaces(str, *_pos);
	
	if(str[*_pos] == ',') /*check there is no comma before parameters displayed*/
	{
		printLineError(ERROR, _lineInfo, "Illegal comma after %s name: \'%s\'.", _lineType, _name);
		return FALSE;
	}
	return TRUE;
}

bool postOperandSyntaxCheck(sourceLine_info* _lineInfo, int* _pos, char* _operand, bool _isLast)
{
	char* str = _lineInfo->m_sourceLine; /*pointer to location on line*/
	*_pos = skipSpaces(str, *_pos); 					

	/*check next comma after operand*/
	if(str[*_pos] == ',')
	{
		*_pos = skipSpaces(str, *_pos+1);
		if(str[*_pos] == ',')
		{
			printLineError(ERROR, _lineInfo, "Multiple consecutive commas.");		
			return FALSE;
		}
		if(!isOperandTextExist(str, _pos) && _isLast) /*check if there is another operand or only spaces*/
		{
			printLineError(ERROR, _lineInfo, "Invalid comma at the end of operand text.");		
			return FALSE;
		}
	}
	else if(isOperandTextExist(str, _pos) || !_isLast) /*check if there are only space-type characters to end of line*/
	{
		printLineError(ERROR, _lineInfo, "Missing comma after operand: \'%s\'.", _operand);		
		return FALSE;
	}
	return TRUE; /*no more operands on line to read*/
}


/*check if the received operand string is a holds a valid register format*/
static bool isValidRegisterFormat(const char* _operand)
{
	int count = 0;
	if(*_operand=='$') /*check if operand has a +/- mark*/
	{
		count++;
		_operand++;
	}
	
	if(count != 1)
	{
		return FALSE;
	}
	
	if(strlen(_operand)==0)
	{
		return FALSE; /*no register number in opernad*/
	}

	while(*_operand != '\n' && *_operand != '\0')
	{
		if(!isdigit(*_operand))
		{
			return FALSE;
		}
		_operand++;
	}
	return TRUE;
}

