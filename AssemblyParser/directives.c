/*directives.c*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "directives.h"
#include "errors.h"
#include "labelAnalysis.h"
#include "generalFuncs.h"

const char* reservedDirectiveWordsArr[] = {"db", "dh", "dw", "asciz", "entry", "extern", NULL}; /*reserved words*/

bool isDirectiveName(const char* _word)
{
	int i=0;
	while(NULL != reservedDirectiveWordsArr[i])
	{
		if(!strcmp(_word, reservedDirectiveWordsArr[i]))
		{
			return TRUE;
		}
		i++;
	}
	return FALSE; /*word is not a directive*/
}

static dataImg* createDataImgItem(const char* _name, long* _dc, long _num, num_byte_size _bytesNum)
{
	dataImg* dataImg;
	
	dataImg = malloc(sizeof(dataImg));
	if(NULL == dataImg)
	{
		printf("\nFatal Error: Memory allocation failed for data image item. Program will be terminated.");
		exit(1);
	}
	
	dataImg->m_value = _num;
	dataImg->m_size = _bytesNum * CHAR_BIT; /*save num of bits*/
	dataImg->m_address = *_dc;
	(*_dc)+=_bytesNum;
	
	return dataImg;
}

bool processAscizDir(sourceLine_info* _lineInfo, const char* _name, int _pos, long* _dc, list* _dataImgList)
{
	int lastPos = _pos;
	dataImg* dataImgItem;
	char operand[MAX_LINE_LENGTH];
	char* next=_lineInfo->m_sourceLine; /*pointer to location on line*/

	if(extractOperandString(_lineInfo->m_sourceLine, _pos, operand)==0) /*check if there is any operand after directive name*/
	{
		printLineError(ERROR, _lineInfo, "The '.asciz' directive doesn't have an operand.");
		return FALSE;
	}

	/*skip spaces between directive and operand*/
	_pos = skipSpaces(_lineInfo->m_sourceLine, _pos);
	/*check if string operand is inside '"'*/
	if(_lineInfo->m_sourceLine[_pos] != '"') 
	{
		printLineError(ERROR, _lineInfo, "In '.asciz' directive operand: %s - doesn't have an openning \'\"\'.", operand);
		return FALSE;
	}

	next=strrchr(next+_pos+1, '"'); /*check for the last occurrence of '"' in line from given location*/
	if(NULL==next)
	{
		printLineError(ERROR, _lineInfo, "In '.asciz' directive operand: %s - doesn't have a closing \'\"\'.", operand);
		return FALSE;
	}
	
	lastPos = strlen(_lineInfo->m_sourceLine) - strlen(next); /*find current position on line of the '"'*/
	next = _lineInfo->m_sourceLine+_pos; /*go to the start of the operand string*/

	/*check if there is a single operand - if after closing '"' there is no more text except space-type characters*/			
	if(!(isStrOnlySpaces(_lineInfo->m_sourceLine, lastPos+1)))
	{
			printLineError(ERROR, _lineInfo, "Multiple operands for '.asciz' directive: %s", operand);
			return FALSE;
	}
			
	/*there is a valid operand format - check if characters are valid*/
	while(_pos+1<lastPos)
	{
		if(isprint(_lineInfo->m_sourceLine[_pos+1]))
		{
			dataImgItem = createDataImgItem(_name, _dc, _lineInfo->m_sourceLine[_pos+1], DB);
			pushListTail(_dataImgList, dataImgItem);
			_pos++;
		}
		else
		{
			printLineError(ERROR, _lineInfo, "In '.asciz' directive operand string: %s has a not printable character: %c", operand, _lineInfo->m_sourceLine[_pos+1]);
			return FALSE;
		}
	}
	dataImgItem = createDataImgItem(_name, _dc, '\0', DB); /*add also terminition mark to end of the string in the data image array*/
	pushListTail(_dataImgList, dataImgItem);
	return TRUE;
}

bool processDataDir(sourceLine_info* _lineInfo, const char* _name, int _pos, long* _dc, list* _dataImgList)
{
	long num;
	bool isInRange;
	dataImg* dataImgItem;
	char operand[MAX_LINE_LENGTH];
	char* str = _lineInfo->m_sourceLine; /*pointer to location on line*/
	num_byte_size numBytes;
	
	if(!preOperandSyntaxCheck(_lineInfo, _name, &_pos, "directive"))
	{
		return FALSE;
	}
	
	while(str[_pos] != '\n' && str[_pos] != '\0') /*go over string of operands and verify each operand if it's a valid num*/
	{
		isInRange=FALSE;
		if(!extractOperandNum(_lineInfo, &_pos, operand))
		{
			return FALSE;
		}
		
		if(convertStringToLong(_lineInfo, operand, &num))
		{
			if(!strcmp(_name, "db")) /* 1 bytes*/
			{
				numBytes = DB;
			}
			else if(!strcmp(_name, "dh")) /* 2 bytes*/
			{
				numBytes = DH;
			}
			else {
				numBytes = DW; /* 4 bytes*/
			}	
			isInRange = isNumInRange(num, numBytes);
		}
		if(!isInRange)
		{
			printLineError(ERROR, _lineInfo, "Operand num: %s isn't in required range for a %s directive.", operand, _name);		
			return FALSE;
		}
		dataImgItem = createDataImgItem(_name, _dc, num, numBytes);
		pushListTail(_dataImgList, dataImgItem); /*insert directive to data image list*/
			
		if(!postOperandSyntaxCheck(_lineInfo, &_pos, operand, TRUE))
		{
			return FALSE;
		}
	}
	return TRUE;
}

bool processExternalDir(sourceLine_info* _lineInfo, symbolTable* _table, const char* _name, int _pos)
{
	label_error labelError;
	symbol* newSymbol;
	char operand[MAX_LINE_LENGTH]; /*maximum string size for optional operand label*/

	if(!isOperandTextExist(_lineInfo->m_sourceLine, &_pos)) /*skip spaces and check*/
	{
		printLineError(ERROR, _lineInfo, "Directive \'%s\' has no operand.", _name); /*no operand after directive*/
		return FALSE;
	}

	extractWord(_lineInfo->m_sourceLine, &_pos, operand); /*get the operand string*/
	
	/*check if there is single operand for directive line*/
	if(!isStrOnlySpaces(_lineInfo->m_sourceLine, _pos))
	{
		printLineError(ERROR, _lineInfo, "Directive \'%s\' has multiple operands instead of a single operand.", _name);
		return FALSE;
	}

	/*check if operand is a valid label name*/
	if(isValidLabelName(_lineInfo, operand, _table, &labelError))
	{
		if(isSymbolExist(_table, operand))
		{	
			if(!isValidAttToAdd(_table, operand, EXTERNAL))
			{
				/*symbol with with the same name and other attribute is already exist*/
				printLineError(ERROR, _lineInfo, "Symbol \'%s\' is already exist in table with other attribute and can't be defined as 'external'.", operand);
				return FALSE;
			}
			return TRUE; /*no need to add symbol, it is already exist in table with external attribute*/
		}
		else {
			/*add symbol with the external attribute to symbol table*/
			newSymbol = createSymbol(operand, 0, EXTERNAL);
			addSymbolToTable(_table, newSymbol);
		}
	}
	else {
		printLabelError(_lineInfo, operand, labelError); /*operand is not a valid label name*/
		return FALSE;
	}	
	return TRUE; /*external line is valid*/
}

void updateDataImgAddresses(const list* _dataImgList, long _icf)
{
	dataImg* data;
	Node* temp = getListItrBegin(_dataImgList); /*first node on list*/
	Node* endList = getListItrEnd(_dataImgList);
	while(temp != endList)
	{
		data = (dataImg*)getListNodeData(temp);
		data->m_address += _icf;
		temp = getListItrNext(temp);
	}
}

/*pointer to function to destroy each node data of the data image*/
void elementDataDestroy(void* _item)
{
	if(NULL == _item)
	{
		return;
	}
	
	free(_item);
	_item = NULL;
}

