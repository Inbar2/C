/*labelAnalysis.c*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "labelAnalysis.h"
#include "errors.h"
#include "directives.h"
#include "instructions.h"
#include "generalFuncs.h"

static bool isReservedName(const char* _word);
static int skipSpacesBackward(const char* _str, int _pos);

void printLabelError(sourceLine_info* _lineInfo, const char* _word, label_error _labelError)
{
	switch(_labelError)
	{
		case NO_LABEL_NAME_ERR: printLineError(ERROR, _lineInfo,"Missing label name before \':\'."); break;
		case ALL_ALPHANUM_CHARS_ERR: printLineError(ERROR, _lineInfo, "Label \"%s\" isn't consisted of alphanumeric characters only.", _word); break;
		case RESERVED_WORD_ERR: printLineError(ERROR, _lineInfo, "Label \"%s\" is a reserved word in the assembly language.", _word); break;
		case ALPAHBETIC_FIRST_CHAR_ERR: printLineError(ERROR, _lineInfo, "Label \"%s\" has a non-alphabetic first char in the name.", _word); break;
		case WORD_LENGTH_ERR: printLineError(ERROR, _lineInfo, "Label \"%s\" length is larger than %d characters.", _word, LABEL_MAX_LENGTH); break;
		case LABEL_COLON_ERR: printLineError(ERROR, _lineInfo, "Label \"%s\" doesn't have ':' after it.", _word); break;
		case LABEL_ALREADY_DEFINED_ERR: printLineError(ERROR, _lineInfo, "Label \"%s\" is already defined.", _word); break;
		case NONE_ERR: break;
	}	
}

void extractLabelWord(const char* _line, int* _pos, char* _str)
{
	int i=0;
	*_pos = skipSpaces(_line, *_pos); /*if there are spaces in the beginning of the line ignore them*/
	
	while(*_pos <= MAX_LINE_LENGTH && _line[*_pos] != '\0' && _line[*_pos] != '\n' && _line[*_pos] != ':')
	{
		_str[i] = _line[*_pos];
		(*_pos)++;
		i++;
	}
	_str[i] = '\0'; /*end of string*/
}

bool extractOperandLabelWord(sourceLine_info* _lineInfo, int* _pos, char* _word, symbolTable* _table)
{
	int tempPos;
	label_error labelError;
	char* str = _lineInfo->m_sourceLine;
	char* c=NULL;
	int i=0;

	*_pos = skipSpaces(str, *_pos);
	
	/*no ',', get only single string word*/
	c = strchr(str+*_pos, ',');
	while(str[*_pos] != '\0' && str[*_pos] != '\n')
	{
		if(NULL!=c && str[*_pos] == ',')
		{
			break;
		}
		_word[i] = str[*_pos];
		(*_pos)++;
		i++;
	}
	tempPos = *_pos;
	*_pos = skipSpacesBackward(str, *_pos);
	i = i-(tempPos-(*_pos));
	_word[i] = '\0'; /*end of string*/
	if(!isValidLabelName(_lineInfo, _word, _table, &labelError))
	{
		printLabelError(_lineInfo, _word, labelError);
		return FALSE;
	}
	return TRUE;
}

bool isValidLabel(sourceLine_info* _lineInfo, int _pos, const char* _str, symbolTable* _table, label_error* _labelError)
{
	/*label must ends with ':' without spaces befor it*/
	if(_lineInfo->m_sourceLine[_pos] == ':')
	{
		/*word has a label format*/
		return isValidLabelName(_lineInfo, _str, _table, _labelError); /*check if string is a valid label*/
	}
	
	*_labelError = LABEL_COLON_ERR;
	return FALSE;
}

/*check if label name is valid - label should be consisted of 32 characters at most, starts with alphabetic character and has only alphanumeric characters, is not a reserved word*/
bool isValidLabelName(sourceLine_info* _lineInfo, const char* _str, symbolTable* _table, label_error* _labelError)
{
	int i=0;
	int size=strlen(_str);

	if(size==0)
	{
		/*no label name*/
		*_labelError = NO_LABEL_NAME_ERR;
		return FALSE;
	}
	
	if(size<LABEL_MAX_LENGTH)
	{
		if(!isReservedName(_str) /*&& !(_str[i]=='.' && isDirectiveName(_str+1))*/) /*check if label is a reserved word*/
		{
			if(isalpha(_str[i])) /*check if first character is alphabetic only*/
			{
				while(++i<size)
				{
					if(!isalnum(_str[i])) /*check if all label characters are alphanumeric. Also if word contains spaces - an error will be arised*/
					{
						*_labelError = ALL_ALPHANUM_CHARS_ERR;
						return FALSE;
					}
				}
				return TRUE;/*word is a valid label name*/
			}
			else {

				*_labelError = ALPAHBETIC_FIRST_CHAR_ERR;
				return FALSE;
			}
		}
		else {
			*_labelError = RESERVED_WORD_ERR;
			return FALSE;
		}
	}
	*_labelError = WORD_LENGTH_ERR;
	return FALSE; 
}

/*skip spaces backward from a giving position*/
static int skipSpacesBackward(const char* _str, int _pos)
{	
	while(isspace(_str[_pos-1]))
	{
		_pos--;
	}
	return _pos;
}

/*check if label is a reserved word (an instruction or a directive name*/
static bool isReservedName(const char* _word)
{
	return (isDirectiveName(_word)==TRUE || isInstructionName(_word)==TRUE);
}

